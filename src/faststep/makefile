CPP=g++
CPPFLAGS=-Wall -O3 -std=c++11

SOURCES=utils/graph.cpp GradientN2.cpp BinaryDecomposition.cpp ApproxGradient.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: quadraticdecomposition approxdecomposition 2recursive
	mkdir -p communities

2recursive: $(OBJECTS) 2recursive.cpp
	$(CPP) $(CPPFLAGS) 2recursive.cpp -o 2recursive $(OBJECTS)

quadraticdecomposition: $(OBJECTS) quadraticDecomposition.cpp
	$(CPP) $(CPPFLAGS) quadraticDecomposition.cpp -o quadraticdecomposition $(OBJECTS)

approxdecomposition: $(OBJECTS) approxDecomposition.cpp
	$(CPP) $(CPPFLAGS) approxDecomposition.cpp -o approxdecomposition $(OBJECTS)

findComms: $(OBJECTS) findCommunities.cpp
	$(CPP) $(CPPFLAGS) findCommunities.cpp $(OBJECTS) -o findCommunities

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) -o $@ $<

clean:
	rm -rf $(OBJECTS) approxdecomposition quadraticdecomposition findCommunity testSampling testGradients
