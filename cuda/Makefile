CXX = g++
NVCC = nvcc
MAGICK_FLAGS = `Magick++-config --cxxflags --libs`
MAGICK_FLAGS_NO_OMP = `Magick++-config --cxxflags --libs | xargs -d' ' -n 1 | grep -v "openmp"`
CPU_CXXFLAGS = $(MAGICK_FLAGS) -O2 -g -std=gnu++17 -lstdc++fs -Wno-unused-result
GPU_CXXFLAGS = $(MAGICK_FLAGS_NO_OMP) -O2 -g -std=c++14 --expt-relaxed-constexpr -arch compute_52

gpu: Mandelbrot.h gpu.cu
	$(NVCC) $(GPU_CXXFLAGS) -c --compiler-options '-fPIC' -o gpu.o gpu.cu
	$(NVCC) -shared -o libgpu.so gpu.o

mandelbrot: Mandelbrot.h Mandelbrot.cpp
	$(CXX) $(CPU_CXXFLAGS) -c -o Mandelbrot.o Mandelbrot.cpp

md-gpu: gpu mandelbrot
	$(CXX) $(CPU_CXXFLAGS) -o md-gpu Mandelbrot.o -L. -lgpu

all: md-gpu
