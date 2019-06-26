/*
 * =====================================================================================
 *
 *       Filename:  cpu.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/11/2019 09:40:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Mandelbrot.h"

// Calculate the number of iterations needed for a point to begin to approach
// infinity, up to a given threshold.
int32_t iterations(Point coords, uint32_t threshold) {
	auto z = Point(0, 0);

	for (uint32_t i = 0; i < threshold; i++) {
		z = z*z + coords;
		if (std::abs(z) > 2.0) return i;
	}
	return -1;
}

// Map a set of pixel coordinates to a point in the given bounds.
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

// Calculate the bounds needed for a certain frame in the zoomed image.
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

// Render an image of the Mandelbrot set into the given canvas,
// using the given bounds and threshold.
void generate_image(Image &image, Bounds bounds, uint32_t threshold) {
	auto width = image.columns();
	auto height = image.rows();

	// Lock image for pixel modification
	image.modifyImage();

	// Retrieve the raw pixel data
	Pixels pixel_cache(image);
	auto pixels = pixel_cache.get(0, 0, width, height);

	// Define color information
	Color black(0, 0, 0, 0);
	auto color_scale = ((Real) QuantumRange / threshold);

	std::vector<int> thresholds(width * height);

#pragma omp simd
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			// Identify the coordinates for this point
			Point coords = coord_map(bounds, Coord(width, height), Coord(x, y));

			// Find the number of iterations needed for this point
			thresholds[width * y + x] = iterations(coords, threshold);
		}
	}


	// Iterate over every pixel in the image
#pragma omp parallel for
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			int depth = thresholds[width * y + x];
			// Set the pixel's color based on this value
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

	// Save the raw pixel data
	pixel_cache.sync();
}
