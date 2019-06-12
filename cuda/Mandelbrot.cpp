#include <filesystem>
#include <cstdio>
#include <random>
#include <iostream>

namespace fs = std::filesystem;

#include "Mandelbrot.h"

int main(int argc, char **argv) {
	// Get parameters of job
	auto first_frame = std::stoi(argv[1]);
	auto last_frame = std::stoi(argv[2]);
	auto job_number = std::stoi(argv[3]);

	// Generate a random angle
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 359);
	auto angle_deg = dist(rd);
	std::cout << angle_deg << std::endl;
	Real angle = angle_deg * M_PI / 180;

	// Find coordinates for that angle
	Real r = (1 - cos(angle)) / 2.0;
	Real x = r * cos(angle) + 0.25;
	Real y = r * sin(angle);
	std::cout << r << ' ' << x << ' ' << y << ' ' << angle << std::endl;

	std::vector<std::string> filenames;

	if (!fs::is_directory("frames")) fs::create_directories("frames");

	std::vector<Image> frames;

	for (auto i = first_frame; i <= last_frame; i++) {
		printf("Starting generation of frame %d...\n", i);

		int my_threshold = threshold * std::pow(threshold_increase, i);
		auto bounds = frame_bounds(
				Bounds(Point(-2, 1.1), Point(1, -1.1)),
				Point(x, y), i, 0.1);

		auto image = Image("640x480", Color(MaxRGB, MaxRGB, MaxRGB, 0));

		generate_image(image, bounds, my_threshold);

		image.magick("png");
		image.animationDelay(5);
		image.gaussianBlur(1, 1);
		image.scale(Geometry("320x240"));

		printf("Generated frame %d\n", i);

		char* filename;
		asprintf(&filename, "frames/output_%d.png", i);
		image.write(filename);

		frames.push_back(image);

		printf("Saved frame %d\n", i);
	}

	printf("Stitching animation...\n");

	char* filename;
	asprintf(&filename, "output_%d.gif", job_number);

	writeImages(frames.begin(), frames.end(), filename);

	printf("Job %d complete.\n", job_number);
	return 0;
}

// vim: ts=4:sw=4:noet:
