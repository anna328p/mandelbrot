project('mandelbrot', 'cpp',
	version: 'a0.01',
	default_options: [
		'cpp_std=c++17'
	])

src = ['Mandelbrot.cpp', 'cpu.cpp']

cxx = meson.get_compiler('cpp')

mpp = dependency('Magick++')
boost = dependency('boost')
omp = dependency('openmp')

executable('md-cpu',
	sources: src,
	dependencies: [
		mpp,
		boost,
		omp
	])
