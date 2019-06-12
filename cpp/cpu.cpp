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
