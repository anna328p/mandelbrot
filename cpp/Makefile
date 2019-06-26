CXX := g++
MAGICK_FLAGS := $(shell Magick++-config --cxxflags)
MAGICK_LIBS := $(shell Magick++-config --libs)
OPTIMIZATIONS = -O4 -flto -fno-fat-lto-objects -march=native -Ofast
CXXFLAGS := $(MAGICK_FLAGS) $(OPTIMIZATIONS) -std=gnu++17 -lstdc++fs -fopenmp -Wno-unused-result -Wall -Wextra -pedantic
LDFLAGS := $(MAGICK_LIBS) $(OPTIMIZATIONS) -std=gnu++17 -lstdc++fs -fopenmp -Wno-unused-result -Wall -Wextra -pedantic

all: md-cpu

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

md-cpu: cpu.o Mandelbrot.o
	$(CXX) $(LDFLAGS) -o md-cpu Mandelbrot.o cpu.o

clean:
	rm -f cpu.o Mandelbrot.o md-cpu
