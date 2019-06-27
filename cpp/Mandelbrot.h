/*
 * =====================================================================================
 *
 *       Filename:  Mandelbrot.h
 *
 *    Description:  Header file for Mandelbrot
 *
 *        Created:  06/11/2019 09:40:52 PM
 *
 * =====================================================================================
 */

// Includes

#include <vector>
#include <complex>
#include <cstdint>
#include <utility>
#include <cmath>
#include <string>

#include <Magick++.h>

#include <omp.h>

// Type definitions

using Real = double;

using Point = std::complex<Real>;
using Bounds = std::pair<Point, Point>;
using Coord = std::pair<uint32_t, uint32_t>;

// Configuration

enum OutputType { gif, mp4 };
struct Options {
	int32_t first_frame;
	int32_t last_frame;
	uint32_t job_number;

	bool position_is_random;
	Point center_point;

	std::pair<Point,Point> starting_view;
	Real zoom_factor;

	Real threshold;
	Real threshold_increase;

	std::string original_dimensions;
	std::string final_dimensions;

	uint32_t fps;

	std::string output_dir;
	std::string output_filename;
	OutputType output_type;
};

constexpr const char *const original_dimensions = "512x512";
constexpr const char *const final_dimensions = "128x128";

constexpr const uint32_t threshold = 100;
constexpr const uint32_t delay = 5;
constexpr const uint32_t fps = (int) (100 / delay);
constexpr const Real threshold_increase = 1.025;
constexpr const Real zoom_factor = 0.95;

// Function declarations

void generate_image(Magick::Image &image, Bounds bounds, uint32_t threshold);

Bounds frame_bounds( Bounds bounds, Point center, int frame, Real reduction);
