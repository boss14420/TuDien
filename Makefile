all: tudien

CXX = g++
CXXFLAGS = -pipe -Wall -O2 -march=native

tudien: main.o tu_dien.o
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cc tu_dien.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

tu_dien.o: tu_dien.cc tu_dien.hh bang_bam.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
