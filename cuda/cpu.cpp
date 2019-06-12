#include <iostream>
#include <tuple>
#include <vector>
#include <complex>
#include <cstdint>
#include <utility>
#include <cmath>
#include <string>
#include <random>
#include <filesystem>
#include <cstdio>

#include <Magick++.h>

#include <omp.h>

using namespace Magick;
namespace fs = std::filesystem;

using Real = long double;

using Point = std::complex<Real>;
using Bounds = std::pair<Point, Point>;
using Coord = std::pair<uint32_t, uint32_t>;

const uint32_t img_width = 640;
const uint32_t img_height = 480;

const uint32_t threshold = 25;
const Real threshold_increase = 1.025;

int32_t iterations(Point coords, uint32_t threshold) {
	auto z = Point(0, 0);

	for (int i = 0; i < threshold; i++) {
		z = z*z + coords;
		if (std::abs(z) > 2.0) return i;
	}
	return -1;
}

Point coord_map(Bounds bounds, Coord size, Coord location) {
	Real x1 = bounds.first.real();
	Real y1 = bounds.first.imag();
	Real x2 = bounds.second.real();
	Real y2 = bounds.second.imag();

	Real x_dim = x2 - x1;
	Real y_dim = y2 - y1;

	Real x_coord = x1 + x_dim * (((double) location.first) / size.first);
	Real y_coord = y1 + y_dim * (((double) location.second) / size.second);

	return Point(x_coord, y_coord);
}

Bounds frame_bounds(
		Bounds bounds, Point center,
		int frame, Real reduction
		) {
	Real width  = (bounds.second.real() - bounds.first.real()) * pow((1 - reduction), frame);
	Real height = (bounds.second.imag() - bounds.first.imag()) * pow((1 - reduction), frame);

	Real x1 = center.real() - (width / 2);
	Real x2 = center.real() + (width / 2);
	Real y1 = center.imag() - (height / 2);
	Real y2 = center.imag() + (height / 2);
	return Bounds(Point(x1, y1), Point(x2, y2));
}

void generate_image(Image &image, Bounds bounds, uint32_t threshold) {
	auto width = image.columns();
	auto height = image.rows();

	image.modifyImage();

	Pixels pixel_cache(image);
	auto pixels = pixel_cache.get(0, 0, width, height);

	Color black(0, 0, 0, 0);
	auto color_scale = ((Real) QuantumRange / threshold);

	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			Point coords = coord_map(bounds, Coord(width, height), Coord(x, y));
			auto depth = iterations(coords, threshold);
			if (depth == -1) {
				*(pixels + width * y + x) = black;
			} else {
				*(pixels + width * y + x) = Color(
							color_scale * depth,
							color_scale * depth / 2.0,
							QuantumRange - color_scale * depth
							);
			}
		}
	}

	pixel_cache.sync();
}

int main(int argc, char **argv) {
	// Get parameters of job
	auto first_frame = std::stoi(argv[1]);
	auto last_frame = std::stoi(argv[2]);
	auto job_number = std::stoi(argv[3]);

	// Generate a random angle
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 359);
	auto angle_deg = dist(rd);
	Real angle = (Real) (M_PI / 180) * angle_deg;

	// Find coordinates for that angle
	Real r = (cos(angle)) / 2.0;
	Real x = r * cos(angle) + 0.25;
	Real y = r * sin(angle);
	std::cout << r << ' ' << x << ' ' << y << ' ' << angle << std::endl;

	std::vector<std::string> filenames;

	if (!fs::is_directory("frames")) fs::create_directories("frames");

#pragma omp parallel for schedule(dynamic, 2)
	for (auto i = first_frame; i <= last_frame; i++) {
		printf("Starting generation of frame %d...\n", i);

		int my_threshold = threshold * std::pow(threshold_increase, i);
		auto bounds = frame_bounds(
				Bounds(Point(-2, 1.1), Point(1, -1.1)),
				Point(x, y), i, 0.1);

				bounds.second.real(), bounds.second.imag(),
		printf("(%llf,%llf), (%llf,%llf) @ (%llf,%llf)\n",
				bounds.first.real(), bounds.first.imag(),
				bounds.second.real(), bounds.second.imag(),
				bounds.first.real() + (bounds.second.real() - bounds.first.real()) / 2,
				bounds.first.imag() + (bounds.second.imag() - bounds.first.imag()) / 2
			  );
		auto image = Image("640x480", "white");

		generate_image(image, bounds, my_threshold);

		image.magick("png");

		char* filename;
		asprintf(&filename, "frames/output_%d.png", i);
		image.write(filename);

		printf("Generated frame %d\n", i);
	}

	return 0;
}

/*
   puts "Splicing..."
   CryMagick::Tool::Convert.build do |c|
   c.loop 0
   c.delay 5
   filenames.each do |f|
   c << f
   end
   c.resize "#{IMG_WIDTH / 2}x#{IMG_HEIGHT / 2}"
   c << "output_#{ARGV[2]}.gif"
   end

   puts "Job #{ARGV[2]} complete"
   */
// vim: ts=4:sw=4:noet:
