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

const uint32_t img_width = 256;
const uint32_t img_height = 256;

const uint32_t threshold = 100;
const Real threshold_increase = 1.05;

// Function declarations

void generate_image(Image &image, Bounds bounds, uint32_t threshold);

Bounds frame_bounds( Bounds bounds, Point center, int frame, Real reduction);
