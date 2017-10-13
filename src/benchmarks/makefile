CC=g++
LOP=-o
LOPT=-O3 -funroll-loops

MAIN=./Sources/benchm
TAG=benchmark

all: parseBenchmark $(TAG)

$(TAG): $(MAIN).o
	$(CC) $(LOPT) $(LOP) $(TAG) $(MAIN).cpp

parseBenchmark: parseBenchmark.cpp
	$(CC) parseBenchmark.cpp -o parseBenchmark