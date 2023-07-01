#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"
#include "../include-shared/circuit_graph.hpp"
#include <iostream>
#include <fstream>

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

    // static qcircuit mod3(13);

    // for(int i = 0; i < 13; i++) {
    //   mod3.H(i);
    // }

    // for(int i = 1; i < 12; i+=2) {
    //   mod3.CZ(i, i+1);
    // }

    // for(int i = 0; i < 12; i+=2) {
    //   mod3.CZ(i, i+1);
    // }

    // mod3.H(0);
    // mod3.H(0);
    // mod3.Measure(0);

    // mod3.Spacer(1, 4);
    // mod3.H(1);
    // mod3.Spacer(1, 2);
    // mod3.H(1);
    // mod3.Measure(1);

    // mod3.Spacer(2, 1);
    // mod3.H(2);
    // mod3.Measure(2);

    // mod3.Spacer(3, 5);
    // mod3.H(3);
    // mod3.Spacer(3, 1);
    // mod3.H(3);
    // mod3.Measure(3);

    // mod3.Spacer(4, 1);
    // mod3.H(4);
    // mod3.Measure(4);

    // mod3.Spacer(5, 6);
    // mod3.H(5);
    // mod3.H(5);
    // mod3.Measure(5);

    // mod3.Spacer(6, 10);
    // mod3.H(6);
    // mod3.H(6);
    // mod3.Measure(6);

    // mod3.Spacer(7, 14);
    // mod3.H(7);
    // mod3.Spacer(7, 2);
    // mod3.H(7);
    // mod3.Measure(7);

    // mod3.Spacer(8, 11);
    // mod3.H(8);
    // mod3.Measure(8);

    // mod3.Spacer(9, 15);
    // mod3.H(9);
    // mod3.Spacer(9, 1);
    // mod3.H(9);
    // mod3.Measure(9);

    // mod3.Spacer(10, 11);
    // mod3.H(10);
    // mod3.Measure(10);

    // mod3.Spacer(11, 16);
    // mod3.H(11);
    // mod3.H(11);
    // mod3.Measure(11);

    // mod3.Spacer(12, 20); 
    // mod3.H(12);
    // mod3.H(12);
    // mod3.Measure(12);

    //qubit reduction:
    //4->3, 5->4, 6-> 4, 7-> 5, 8->5, 9->5, 10->5, 11->5, 12->6, 13->6, 14->6, 15->7, 16->7, 17 -> 7, 18->7, 19->7, 20->8, 30->10

    //depth
    // 10->8, 12->8, 18->12, 24->14, 30->16

    //roughly log base 1.5
    int clusterstatesize = 25;
    // std::vector<std::vector<std::pair<int, int>>> minimize = circuit_graph::ThreadedMinimizeClusterState(clusterstatesize, 11);
    // FILE * fileWriteout;
    // for(int j = 0; j < minimize.size(); j++) {
    //   qcircuit clust = qcircuit::clusterState(clusterstatesize);
    //   int totalCausal = 0;
    //   for(std::set<int> s : clust.CircuitCausalCone()) {
    //     totalCausal+=s.size();
    //   }
    //   fprintf(fileWriteout, "(%i) ", totalCausal);
    //   for(int i = 0; i < minimize[0].size(); i++) {
    //     clust.Reuse(minimize[j][i].first, minimize[j][i].second);
    //     totalCausal = 0;
    //     for(std::set<int> s : clust.CircuitCausalCone()) {
    //       totalCausal+=s.size();
    //     }
    //     fprintf(fileWriteout, "%i %i (%i) ", minimize[j][i].first, minimize[j][i].second, totalCausal);
    //   }
    //   totalCausal = 0;
    //   for(std::set<int> s : clust.CircuitCausalCone()) {
    //     totalCausal+=s.size();
    //   }
    //   fprintf(fileWriteout, "depth: %i, cc: %i\n", clust.CircuitDepth(), totalCausal);
    // }
    // fclose(fileWriteout);
    // printf("can reduce to %lu qubits\n", clusterstatesize - minimize[0].size());
    // printf("there are %lu ways to do this\n", minimize.size());

    qcircuit cluster9 = qcircuit::clusterState(clusterstatesize);
    std::vector<std::vector<std::pair<int, int>>> minimizeHeuristic = circuit_graph::CausalConeHeuristicReduction(cluster9.CircuitCausalCone());


    for(int j = 0; j < minimizeHeuristic.size(); j++) {
      qcircuit clust = qcircuit::clusterState(clusterstatesize);
      int totalCausal = 0;
      for(std::set<int> s : clust.CircuitCausalCone()) {
        totalCausal+=s.size();
      }
      printf("(%i) ", totalCausal);
      for(int i = 0; i < minimizeHeuristic[j].size(); i++) {
        clust.Reuse(minimizeHeuristic[j][i].first, minimizeHeuristic[j][i].second);
        totalCausal = 0;
        for(std::set<int> s : clust.CircuitCausalCone()) {
          totalCausal+=s.size();
        }
        printf("%i %i (%i) ", minimizeHeuristic[j][i].first, minimizeHeuristic[j][i].second, totalCausal);
      }
      totalCausal = 0;
      for(std::set<int> s : clust.CircuitCausalCone()) {
        totalCausal+=s.size();
      }
      printf("depth: %i, cc: %i\n", clust.CircuitDepth(), totalCausal);
    }
    printf("total reductions: %lu\n", minimizeHeuristic.size());

    qcircuit clusty = qcircuit::clusterState(clusterstatesize);
    clusty.Reuse(16, 19);
    clusty.Reuse(14, 18);
    clusty.Reuse(12, 17);
    clusty.Reuse(11, 15);
    clusty.Reuse(10, 13);
    clusty.Reuse(8, 14);
    clusty.Reuse(3, 0);
    clusty.Reuse(4, 0);
    clusty.Reuse(5, 1);
    clusty.Reuse(6, 0);
    clusty.Reuse(2, 6);
    clusty.Reuse(0, 8);

    circuit_graph::OutputCircuit(clusty, "output_circuit3");


    return 0;
}