/*
 * =====================================================================================
 *
 *       Filename:  gpu.cu
 *
 *    Description:  GPU-dependent code for Mandelbrot generator
 *
 *        Version:  1.0
 *        Created:  06/11/2019 09:40:39 PM
 *       Revision:  none
 *       Compiler:  nvcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Mandelbrot.h"
#include <iostream>

__host__ __device__
int32_t iterations(Point coords, uint32_t threshold) {
	Real z_r = 0;
	Real z_i = 0;

	Real old_z_r = 0;
	Real old_z_i = 0;

	auto coords_r = coords.real();
	auto coords_i = coords.imag();

	for (int i = 0; i < threshold; i++) {
		old_z_r = z_r;
		old_z_i = z_i;

		z_r = old_z_r * old_z_r - old_z_i * old_z_i + coords_r;
		z_i = 2 * old_z_r * old_z_i + coords_i;

		if (sqrt(z_r * z_r + z_i * z_i) > 2.0) return i;
	}
	return -1;
}

__host__ __device__
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

__host__
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

__global__
void populate_row(int *p, uint32_t y, uint32_t width, uint32_t height, Bounds bounds, uint32_t threshold) {

	for (auto x = 0; x < width; x++) {
		Point coords = coord_map(bounds, Coord(width, height), Coord(x, y));
		p[x] = iterations(coords, threshold);
	}
}

__host__
void generate_image(Image &image, Bounds bounds, uint32_t threshold) {
	auto width = image.columns();
	auto height = image.rows();

	image.modifyImage();

	Pixels pixel_cache(image);
	auto pixels = pixel_cache.get(0, 0, width, height);

	Color black(0, 0, 0, 0);
	auto color_scale = ((Real) MaxRGB / threshold);

	int32_t *p;

	for (auto y = 0; y < height; y++) {
		cudaMallocManaged(&p, width * sizeof(int32_t));

		populate_row<<<1, 1>>>(p, y, width, height, bounds, threshold);

		for (auto i = 0; i < width; i++) {
			int32_t depth = p[i];
			if (depth == -1) {
				*(pixels + width * y + i) = black;
			} else {
				*(pixels + width * y + i) = Color(
						color_scale * depth,
						color_scale * depth / 2.0,
						MaxRGB - color_scale * depth);
			}
		}

		cudaFree(p);
	}

	cudaDeviceSynchronize();

	pixel_cache.sync();
}

