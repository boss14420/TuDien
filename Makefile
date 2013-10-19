all: tudien

CXX = g++
CXXFLAGS = -g -Wall

tudien: main.o tu_dien.o
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cc tu_dien.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

tu_dien.o: tu_dien.cc tu_dien.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
