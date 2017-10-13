CPP=g++
CPPFLAGS=-Wall -O3 -std=c++11
OBJ=nmi.o jaccard.o clusterUtils.o ../overlapping-nmi/onmi.o

all: compareClusters comparisonMatrix parseGraph parseGraphTxt countClusters
	$(MAKE) -C ../overlapping-nmi

countClusters: countClusters.cpp
	$(CPP) countClusters.cpp -o countClusters $(CPPFLAGS)

comparisonMatrix: comparisonMatrix.cpp $(OBJ)
	$(CPP) comparisonMatrix.cpp nmi.o ../overlapping-nmi/onmi.o jaccard.o clusterUtils.o -o comparisonMatrix $(CPPFLAGS)

compareClusters: compareClusters.cpp $(OBJ)
	$(CPP) nmi.o jaccard.o ../overlapping-nmi/onmi.o clusterUtils.o compareClusters.cpp -o compareClusters $(CPPFLAGS)

../overlapping-nmi/onmi.o: ../overlapping-nmi/onmi.cpp ../overlapping-nmi/cmdline.c
	$(MAKE) -C ../overlapping-nmi

commonStuff: nmi.cpp jaccard.cpp clusterUtils.cpp
	$(CPP) nmi.cpp jaccard.cpp clusterUtils.cpp -c $(CPPFLAGS)

parseGraph: parseGraph.cpp
	$(CPP) parseGraph.cpp -o parseGraph $(CPPFLAGS)

parseGraphTxt: parseGraphTxt.cpp
	$(CPP) parseGraphTxt.cpp -o parseGraphTxt $(CPPFLAGS)

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) -o $@ $<

clean:
	rm -rf $(OBJ) compareClusters comparisonMatrix parseGraph

