CXX = g++
MAGICK_FLAGS = `Magick++-config --cxxflags --libs`
CPU_CXXFLAGS = $(MAGICK_FLAGS) -O2 -g -std=gnu++17 -lstdc++fs -fopenmp -Wno-unused-result

cpu: Mandelbrot.h cpu.cpp
	$(CXX) $(CPU_CXXFLAGS) -c -o cpu.o cpu.cpp

mandelbrot: Mandelbrot.h Mandelbrot.cpp
	$(CXX) $(CPU_CXXFLAGS) -c -o Mandelbrot.o Mandelbrot.cpp

md-cpu: cpu mandelbrot
	$(CXX) $(CPU_CXXFLAGS) -o md-cpu Mandelbrot.o cpu.o

all: md-cpu
