CXX = g++
CPU_CXXFLAGS = -O2 -g -std=gnu++17 -lstdc++fs -fopenmp `Magick++-config --cppflags --cxxflags --ldflags --libs`

cpu: cpu.cpp
	$(CXX) $(CPU_CXXFLAGS) -o cpu cpu.cpp
