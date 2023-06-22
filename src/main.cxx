#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"
#include "../include-shared/circuit_graph.hpp"
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
    // printf("circuit is being built\n");
    // static qcircuit simplecircuit(3);
    // printf("now adding h gate\n");
    // simplecircuit.H(0);
    // printf("now adding cnot gate\n");
    // simplecircuit.CNOT(0, 1);
    // printf("now adding measure gate\n");
    // simplecircuit.Measure(0);
    // printf("now adding h gate\n");
    // simplecircuit.H(1);
    // printf("now adding cnot gate\n");
    // simplecircuit.CNOT(1, 2);
    // printf("now adding measure gate\n");
    // simplecircuit.Measure(1);
    // printf("now adding measure gate\n");
    // simplecircuit.Measure(2);
    // printf("now doing reuse\n");
    // //bool works = simplecircuit.Reuse(0, 2);
    // //printf("%s, and the number of roots is %lu\n", works ? "true" : "false", simplecircuit.getroots()->size());

    static qcircuit mod3(13);

    for(int i = 0; i < 13; i++) {
      mod3.H(i);
    }

    for(int i = 1; i < 12; i+=2) {
      mod3.CZ(i, i+1);
    }

    for(int i = 0; i < 12; i+=2) {
      mod3.CZ(i, i+1);
    }

    mod3.H(0);
    mod3.H(0);
    mod3.Measure(0);

    mod3.Spacer(1, 4);
    mod3.H(1);
    mod3.Spacer(1, 2);
    mod3.H(1);
    mod3.Measure(1);

    mod3.Spacer(2, 1);
    mod3.H(2);
    mod3.Measure(2);

    mod3.Spacer(3, 5);
    mod3.H(3);
    mod3.Spacer(3, 1);
    mod3.H(3);
    mod3.Measure(3);

    mod3.Spacer(4, 1);
    mod3.H(4);
    mod3.Measure(4);

    mod3.Spacer(5, 6);
    mod3.H(5);
    mod3.H(5);
    mod3.Measure(5);

    mod3.Spacer(6, 10);
    mod3.H(6);
    mod3.H(6);
    mod3.Measure(6);

    mod3.Spacer(7, 14);
    mod3.H(7);
    mod3.Spacer(7, 2);
    mod3.H(7);
    mod3.Measure(7);

    mod3.Spacer(8, 11);
    mod3.H(8);
    mod3.Measure(8);

    mod3.Spacer(9, 15);
    mod3.H(9);
    mod3.Spacer(9, 1);
    mod3.H(9);
    mod3.Measure(9);

    mod3.Spacer(10, 11);
    mod3.H(10);
    mod3.Measure(10);

    mod3.Spacer(11, 16);
    mod3.H(11);
    mod3.H(11);
    mod3.Measure(11);

    mod3.Spacer(12, 20);
    mod3.H(12);
    mod3.H(12);
    mod3.Measure(12);



    // std::vector<std::set<int>> causalcones;
    // for(int i = 0; i < 13; i++) {
    //   causalcones[i] = mod3.CausalCone(i);
    // }
    // std::sort(causalcones.begin(), causalcones.end(), [](const std::set<int> & a, const std::set<int> & b){ return a.size() < b.size(); });

    int clusterstatesize = 9;
    // qcircuit cluster = qcircuit::clusterState(clusterstatesize);
    // printf("done building now time for reuse. qubit 0 has cone of size %lu\n", cluster.CausalCone(0).size());
    // bool reusing = true;
    // std::vector<int> correct;
    // naive search
    // for(int i = 0; i < 8; i++) {
    //   for(int j = 1; j < clusterstatesize; j++) {
    //     for(int k = 0; k < clusterstatesize - 1; k++) {
    //       for(int l = 0; l < clusterstatesize - 1; l++) {
    //         for(int m = 0; m < clusterstatesize - 2; m++) {
    //           for(int n = 0; n < clusterstatesize - 2; n++) {
    //             for(int o = 0; o < clusterstatesize - 3; o++) {
    //               for(int p = 0; p < clusterstatesize - 3; p++) {
    //                 printf("i:%i,j:%i,k:%i,l:%i,m:%i,n:%i,o:%i,p:%i\n", i, j, k, l, m, n, o, p);
    //                 cluster = clusterState(clusterstatesize);
    //                 if(cluster.Reuse(i, j) && cluster.Reuse(k, l) && cluster.Reuse(m, n) && cluster.Reuse(o, p)) {
    //                   correct.push_back(10000000*i + 1000000*j + 100000*k + 10000*l + 1000*m + 100*n + 10*o + p);
    //                 }
    //               }
    //             }
    //           }
    //         }
    //       }
    //     }
    //   }
    // }


    //search which gives up on the first layer that fails
    // for(int i = 0; i < clusterstatesize; i++) {
    //   for(int j = 1; j < clusterstatesize; j++) {
    //     cluster = qcircuit::clusterState(clusterstatesize);
    //     if(cluster.Reuse(i, j)) {
    //       for(int k = 0; k < clusterstatesize - 1; k++) {
    //         for(int l = 0; l < clusterstatesize - 1; l++) {
    //           if(cluster.Reuse(k, l)) {
    //             for(int m = 0; m < clusterstatesize - 2; m++) {
    //               for(int n = 0; n < clusterstatesize - 2; n++) {
    //                 if(cluster.Reuse(m, n)) {
    //                   for(int o = 0; o < clusterstatesize - 3; o++) {
    //                     for(int p = 0; p < clusterstatesize - 3; p++) {
    //                       if(cluster.Reuse(o, p)) {
    //                         //printf("i:%i,j:%i,k:%i,l:%i,m:%i,n:%i,o:%i,p:%i\n", i, j, k, l, m, n, o, p);
    //                         correct.push_back(10000000*i + 1000000*j + 100000*k + 10000*l + 1000*m + 100*n + 10*o + p);
    //                       }
    //                       cluster = qcircuit::clusterState(clusterstatesize);
    //                       cluster.Reuse(i, j);
    //                       cluster.Reuse(k, l);
    //                       cluster.Reuse(m, n);
    //                     }
    //                   }
    //                 }
    //                 cluster = qcircuit::clusterState(clusterstatesize);
    //                 cluster.Reuse(i, j);
    //                 cluster.Reuse(k, l);
    //               }
    //             }
    //           }
    //           cluster = qcircuit::clusterState(clusterstatesize);
    //           cluster.Reuse(i, j);
    //         }
    //       }
    //     }
    //   }
    // }




    // printf("AAAAAAAAAAAAAAAAA %s, and the number of roots is %lu\n", reusing ? "true" : "false", cluster.getroots()->size());
    //OutputCircuit(mod3, "output-manual");
    //circuit_graph::OutputCircuit(cluster, "output-cluster3");
    // printf("CORRECT:\n");
    // for(int i : correct) {
    //   printf("%i\n", i);
    // }
    // printf("count: %lu\n", correct.size());

    // printf("in to the real stuff\n");


    std::vector<std::vector<std::pair<int, int>>> minimize8 = circuit_graph::MinimizeClusterState(9);
    printf("can reduce to %lu qubits\n", 9 - minimize8[0].size());
    for(int i = 0; i < minimize8[0].size(); i++) {
      printf("%i %i ", minimize8[0][i].first, minimize8[0][i].second);
    }
    printf("\n");
    //04153545

    // qcircuit cluster9 = qcircuit::clusterState(clusterstatesize);
    // cluster9.Reuse(0, 4);
    // cluster9.Reuse(1, 5);
    // cluster9.Reuse(3, 5);
    // //cluster9.Reuse(4, 5);

    // circuit_graph::OutputCircuit(cluster9, "step2");
    

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