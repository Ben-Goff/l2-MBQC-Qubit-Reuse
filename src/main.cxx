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
	randomSimpleGraph(G, 10, 20);

	DfsAcyclicSubgraph DAS;
	DAS.callAndReverse(G);

	GraphIO::write(G, "output-acyclic-graph.gml", GraphIO::writeGML);



    return 0;
}