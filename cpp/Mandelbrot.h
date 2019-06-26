/*
 * =====================================================================================
 *
 *       Filename:  Mandelbrot.h
 *
 *    Description:  Header file for Mandelbrot
 *
 *        Version:  1.0
 *        Created:  06/11/2019 09:40:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
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

using namespace Magick;

// Type definitions

using Real = long double;

using Point = std::complex<Real>;
using Bounds = std::pair<Point, Point>;
using Coord = std::pair<uint32_t, uint32_t>;

// Configuration

constexpr const char *const original_dimensions = "2048x2048";
constexpr const char *const final_dimensions = "512x512";

constexpr uint32_t threshold = 100;
constexpr uint32_t delay = 5;
constexpr uint32_t frame_rate = (int) (100 / delay);
constexpr Real threshold_increase = 1.025;
constexpr Real zoom_factor = 0.05;

// Function declarations

void generate_image(Image &image, Bounds bounds, uint32_t threshold);

Bounds frame_bounds( Bounds bounds, Point center, int frame, Real reduction);
