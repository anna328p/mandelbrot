CXX = g++
CXXFLAGS = -O2 -g `pkg-config --cflags --libs gtkmm-3.0`

gui: gui.cpp
	$(CXX) gui.cpp -o gui $(CXXFLAGS)

all: gui
