#include "qcircuit.cxx"
#include "../src-shared/gate.cxx"
#include <iostream>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
 
using namespace ogdf;

int main(int argc, char* argv[]) {
    printf("circuit is being built\n");
    static qcircuit simplecircuit(3);
    printf("now adding h gate\n");
    simplecircuit.H(0);
    printf("now adding cnot gate\n");
    simplecircuit.CNOT(0, 1);
    printf("now adding measure gate\n");
    simplecircuit.Measure(0);
    printf("now adding h gate\n");
    simplecircuit.H(1);
    printf("now adding cnot gate\n");
    simplecircuit.CNOT(1, 2);
    printf("now adding measure gate\n");
    simplecircuit.Measure(1);
    printf("now adding measure gate\n");
    simplecircuit.Measure(2);
    printf("now doing reuse\n");
    bool works = simplecircuit.Reuse(0, 2);
    printf("%s, and the number of roots is %lu\n", works ? "true" : "false", simplecircuit.getroots()->size());




    Graph G;
    GraphAttributes GA(G,
      GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);
 
    const int LEN = 11;
    for(int i = 1; i < LEN; ++i) {
        node left = G.newNode();
        GA.x(left) = -5*(i+1);
        GA.y(left) = -20*i;
        GA.width(left) = 10*(i+1);
        GA.height(left) = 15;
 
        node bottom = G.newNode();
        GA.x(bottom) = 20*(LEN-i);
        GA.y(bottom) = 5*(LEN+1-i);
        GA.width(bottom) = 15;
        GA.height(bottom) = 10*(LEN+1-i);
 
        edge e = G.newEdge(left,bottom);
        DPolyline &p = GA.bends(e);
        p.pushBack(DPoint(10,-20*i));
        p.pushBack(DPoint(20*(LEN-i),-10));
    }
 
    GraphIO::write(GA, "output-manual.gml", GraphIO::writeGML);
    GraphIO::write(GA, "output-manual.svg", GraphIO::drawSVG);



    return 0;
}