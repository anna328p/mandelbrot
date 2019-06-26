#include <iostream>
#include <random>
#include <filesystem>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

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

	// Create folder for frames
	char *folder_name;
	asprintf(&folder_name, "frames_%d", job_number);
	if (!fs::is_directory(folder_name)) fs::create_directories(folder_name);

	char* frame_name_format;
	asprintf(&frame_name_format, "%s/output_%%0%dd.png", folder_name, (int) ceil(log10(last_frame)));

	// Generate the frames
	std::vector<Image> frames;

#pragma omp parallel for ordered shared(frames) schedule(dynamic, 1)
	for (auto f = first_frame; f <= last_frame; f++) {
		printf("Starting generation of frame %d...\n", f);

		// Find boundaries of frame
		int my_threshold = threshold * std::pow(threshold_increase, f);
		auto bounds = frame_bounds(
				Bounds(Point(-1, 1.1), Point(1, -1.1)),
				Point(x, y), f, zoom_factor);

		// Create canvas for frame
		auto image = Image("2048x2048", Color(MaxRGB, MaxRGB, MaxRGB, 0));

		// Perform the actual generation
		generate_image(image, bounds, my_threshold);

		// Convert and resize image
		image.magick("png");
		image.animationDelay(delay);
		image.gaussianBlur(1, 1);
		image.scale(Geometry(final_dimensions));

		printf("Generated frame %d\n", f);

		// Save image
		char* filename;
		asprintf(&filename, frame_name_format, f);

		image.write(filename);

		// Add image to list
#pragma omp ordered
		//frames.push_back(image);

		printf("Saved frame %d\n", f);
	}

	// Stitch frames into animation
	printf("Stitching animation...\n");

	char *filename;
	asprintf(&filename, "output_%d.mp4", job_number);

	auto bad_itoa = [] (auto s) { return strdup(s.c_str()); };
	auto str_first_frame = bad_itoa(std::to_string(first_frame));
	auto str_nr_frames = bad_itoa(std::to_string(last_frame - first_frame));

	//writeImages(frames.begin(), frames.end(), filename);
	char *const ffmpeg_args[] = {
				"ffmpeg",
				"-r", "20",
				"-f", "image2",
				"-s", strdup(final_dimensions),
				"-i", strdup(frame_name_format),
				"-start_number", str_first_frame,
				"-vframes", str_nr_frames,
				"-c:v", "libx264rgb",
				"-qp", "0",
				"-preset", "placebo",
				filename, "-y", nullptr
			};
	execvp("ffmpeg", ffmpeg_args);
}

// vim: ts=4:sw=4:noet:
