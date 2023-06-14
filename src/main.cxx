#include "qcircuit.cxx"
#include "../src-shared/gate.cxx"
#include "../src-shared/graph.cxx"
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
    //bool works = simplecircuit.Reuse(0, 2);
    //printf("%s, and the number of roots is %lu\n", works ? "true" : "false", simplecircuit.getroots()->size());

    static qcircuit basic(3);
    basic.CNOT(0, 2);
    basic.H(1);
    basic.Measure(1);
    basic.Measure(0);
    basic.Measure(2);


    OutputCircuit(basic, "output-manual");




    // Graph G;
    // GraphAttributes GA(G, GraphAttributes::all);
    //     node left = G.newNode();
    //     GA.x(left) = 0;
    //     GA.y(left) = 0;
    //     GA.width(left) = 10;
    //     GA.height(left) = 10;
    //     //GA.strokeColor(left) = ogdf::Color::Name::Orange;
    //     //GA.label(left) = "+";
    //     //GA.zLabel(left) = 50;
    //     GA.fillPattern(left) = ogdf::FillPattern::DiagonalCross;
    //     GA.fillBgColor(left) = ogdf::Color::Name::Green;
        

    //     // node bottom = G.newNode();
    //     // GA.x(bottom) = 500;
    //     // GA.y(bottom) = -200;
    //     // GA.width(bottom) = 10;
    //     // GA.height(bottom) = 10;
    //     // GA.fillColor(bottom) = ogdf::Color::Name::Purple;
    //     // GA.fillPattern(bottom) = ogdf::FillPattern::BackwardDiagonal;
    //     // GA.shape(bottom) = ogdf::Shape::Rect;
    //     // GA.fillBgColor(left) = ogdf::Color::Name::Green;
    //     // //GA.label(bottom) = "H";
    //     // //GA.yLabel(bottom) = 1;
 
    //     // edge e = G.newEdge(left,bottom);
    //     // // DPolyline &p = GA.bends(e);
    //     // // p.pushBack(DPoint(10,-20*i));
    //     // // p.pushBack(DPoint(20*(LEN-i),-10));

    // GraphIO::write(GA, "output-manual.gml", GraphIO::writeGML);
    // GraphIO::write(GA, "output-manual.svg", GraphIO::drawSVG);




    return 0;
}