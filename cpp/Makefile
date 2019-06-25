CXX := g++
MAGICK_FLAGS := $(shell Magick++-config --cxxflags)
MAGICK_LIBS := $(shell Magick++-config --libs)
CXXFLAGS := $(MAGICK_FLAGS) -O4 -std=gnu++17 -lstdc++fs -fopenmp -Wno-unused-result -flto -fno-fat-lto-objects
LDFLAGS := $(MAGICK_LIBS) -O4 -std=gnu++17 -lstdc++fs -fopenmp -Wno-unused-result -flto -fno-fat-lto-objects -fwhole-program

all: md-cpu

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

md-cpu: cpu.o Mandelbrot.o
	$(CXX) $(LDFLAGS) -o md-cpu Mandelbrot.o cpu.o

clean:
	rm cpu.o Mandelbrot.o md-cpu
