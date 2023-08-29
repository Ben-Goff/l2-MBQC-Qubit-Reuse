#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"
#include "../include-shared/circuit_graph.hpp"
#include "../include-shared/chp_simulation.hpp"
#include <iostream>
#include <fstream>
extern "C" {
  #include "../include/chp.h"
  #include <time.h>
}

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
    int clusterstatesize = 20;
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

    // qcircuit cluster9 = qcircuit::clusterState(clusterstatesize);
    // std::vector<std::vector<std::pair<int, int>>> minimizeHeuristic = circuit_graph::CausalConeHeuristicReduction(cluster9.CircuitCausalCone());


    // for(int j = 0; j < minimizeHeuristic.size(); j++) {
    //   qcircuit clust = qcircuit::clusterState(clusterstatesize);
    //   int totalCausal = 0;
    //   for(std::set<int> s : clust.CircuitCausalCone()) {
    //     totalCausal+=s.size();
    //   }
    //   printf("(%i) ", totalCausal);
    //   for(int i = 0; i < minimizeHeuristic[j].size(); i++) {
    //     clust.Reuse(minimizeHeuristic[j][i].first, minimizeHeuristic[j][i].second);
    //     totalCausal = 0;
    //     for(std::set<int> s : clust.CircuitCausalCone()) {
    //       totalCausal+=s.size();
    //     }
    //     printf("%i %i (%i) ", minimizeHeuristic[j][i].first, minimizeHeuristic[j][i].second, totalCausal);
    //   }
    //   totalCausal = 0;
    //   for(std::set<int> s : clust.CircuitCausalCone()) {
    //     totalCausal+=s.size();
    //   }
    //   printf("depth: %i, cc: %i\n", clust.CircuitDepth(), totalCausal);
    // }
    // printf("total reductions: %lu\n", minimizeHeuristic.size());

    ogdf::Graph DAG;
    ogdf::GraphAttributes DAGA(DAG, ogdf::GraphAttributes::all);
    ogdf::node q1 = DAG.newNode();
    DAGA.x(q1) = 0;
    DAGA.y(q1) = 0;
    DAGA.width(q1) = 15;
    DAGA.height(q1) = 15;
    DAGA.strokeType(q1) = ogdf::StrokeType::Solid;
    DAGA.strokeWidth(q1) = 2;
    DAGA.fillColor(q1) = ogdf::Color::Name::Red;
    DAGA.label(q1) = "Q1";

    ogdf::node q2 = DAG.newNode();
    DAGA.x(q2) = 30;
    DAGA.y(q2) = 0;
    DAGA.width(q2) = 15;
    DAGA.height(q2) = 15;
    DAGA.strokeType(q2) = ogdf::StrokeType::Solid;
    DAGA.fillColor(q2) = ogdf::Color::Name::Green;
    DAGA.label(q2) = "Q2";

    ogdf::node q3 = DAG.newNode();
    DAGA.x(q3) = 60;
    DAGA.y(q3) = 0;
    DAGA.width(q3) = 15;
    DAGA.height(q3) = 15;
    DAGA.strokeType(q3) = ogdf::StrokeType::Solid;
    DAGA.fillColor(q3) = ogdf::Color::Name::Blue;
    DAGA.label(q3) = "Q3";

    ogdf::node cz1 = DAG.newNode();
    DAGA.x(cz1) = 15;
    DAGA.y(cz1) = 35;
    DAGA.width(cz1) = 30;
    DAGA.height(cz1) = 15;
    DAGA.strokeType(cz1) = ogdf::StrokeType::Solid;
    DAGA.label(cz1) = "CZ";

    ogdf::node h1 = DAG.newNode();
    DAGA.x(h1) = 60;
    DAGA.y(h1) = 35;
    DAGA.width(h1) = 15;
    DAGA.height(h1) = 15;
    DAGA.strokeType(h1) = ogdf::StrokeType::Solid;
    DAGA.label(h1) = "H";

    ogdf::node cz2 = DAG.newNode();
    DAGA.x(cz2) = 45;
    DAGA.y(cz2) = 70;
    DAGA.width(cz2) = 30;
    DAGA.height(cz2) = 15;
    DAGA.strokeType(cz2) = ogdf::StrokeType::Solid;
    DAGA.label(cz2) = "CZ";

    ogdf::node h2 = DAG.newNode();
    DAGA.x(h2) = 0;
    DAGA.y(h2) = 70;
    DAGA.width(h2) = 15;
    DAGA.height(h2) = 15;
    DAGA.strokeType(h2) = ogdf::StrokeType::Solid;
    DAGA.label(h2) = "H";

    ogdf::node m1 = DAG.newNode();
    DAGA.x(m1) = 0;
    DAGA.y(m1) = 105;
    DAGA.width(m1) = 15;
    DAGA.height(m1) = 15;
    DAGA.strokeType(m1) = ogdf::StrokeType::Solid;
    DAGA.fillColor(m1) = ogdf::Color::Name::Red;
    DAGA.label(m1) = "M";

    ogdf::node m2 = DAG.newNode();
    DAGA.x(m2) = 30;
    DAGA.y(m2) = 105;
    DAGA.width(m2) = 15;
    DAGA.height(m2) = 15;
    DAGA.strokeType(m2) = ogdf::StrokeType::Solid;
    DAGA.fillColor(m2) = ogdf::Color::Name::Green;
    DAGA.label(m2) = "M";

    ogdf::node m3 = DAG.newNode();
    DAGA.x(m3) = 60;
    DAGA.y(m3) = 105;
    DAGA.width(m3) = 15;
    DAGA.height(m3) = 15;
    DAGA.strokeType(m3) = ogdf::StrokeType::Solid;
    DAGA.fillColor(m3) = ogdf::Color::Name::Blue;
    DAGA.label(m3) = "M";

    ogdf::edge q1cz1 = DAG.newEdge(q1, cz1);
    DAGA.arrowType(q1cz1) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(q1cz1) = ogdf::Color::Name::Red;

    ogdf::edge q2cz1 = DAG.newEdge(q2, cz1);
    DAGA.arrowType(q2cz1) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(q2cz1) = ogdf::Color::Name::Green;

    ogdf::edge q3h1 = DAG.newEdge(q3, h1);
    DAGA.arrowType(q3h1) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(q3h1) = ogdf::Color::Name::Blue;

    ogdf::edge cz1h2 = DAG.newEdge(cz1, h2);
    DAGA.arrowType(cz1h2) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(cz1h2) = ogdf::Color::Name::Red;

    ogdf::edge cz1cz2 = DAG.newEdge(cz1, cz2);
    DAGA.arrowType(cz1cz2) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(cz1cz2) = ogdf::Color::Name::Green;

    ogdf::edge h1cz2 = DAG.newEdge(h1, cz2);
    DAGA.arrowType(h1cz2) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(h1cz2) = ogdf::Color::Name::Blue;

    ogdf::edge h2m1 = DAG.newEdge(h2, m1);
    DAGA.arrowType(h2m1) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(h2m1) = ogdf::Color::Name::Red;

    ogdf::edge cz2m2 = DAG.newEdge(cz2, m2);
    DAGA.arrowType(cz2m2) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(cz2m2) = ogdf::Color::Name::Green;

    ogdf::edge cz2m3 = DAG.newEdge(cz2, m3);
    DAGA.arrowType(cz2m3) = ogdf::EdgeArrow::Last;
    DAGA.strokeColor(cz2m3) = ogdf::Color::Name::Blue;


    ogdf::GraphIO::write(DAGA, "poster-dag.svg", ogdf::GraphIO::drawSVG);

    qcircuit poster2(4);
    poster2.CZ(0, 2);
    poster2.CZ(1, 3);
    poster2.H(0);
    poster2.H(1);
    poster2.H(2);
    poster2.H(3);
    poster2.Measure(0);
    poster2.Measure(1);
    poster2.Measure(2);
    poster2.Measure(3);
    circuit_graph::OutputCircuit(poster2, "poster-circuit-2");
    poster2.Reuse(0, 3);
    circuit_graph::OutputCircuit(poster2, "poster-circuit-3");




    //(3 23) (5 23) (1 24) (2 6) (4 0) (21 23) (5 21) (20 21) (3 6) (4 18) (10 13) (7 12) (8 13) (6 15) (0 13) (2 12) (1 7) (10 11) (8 6) depth: 50
    //(7 12) (4 11) (6 9) (1 9) (3 8) (0 7) (2 5) depth: 21

    // qcircuit mod32 = qcircuit::mod3n(2);
    // printf("initial depth: %i\n", mod32.CircuitDepth());
    // mod32.SafeReuse(7, 12);
    // mod32.SafeReuse(4, 11);
    // mod32.SafeReuse(6, 9);
    // mod32.SafeReuse(1, 9);
    // mod32.SafeReuse(3, 8);
    // mod32.SafeReuse(0, 7);
    // mod32.SafeReuse(2, 5);
    // circuit_graph::OutputCircuit(mod32, "mod32-sussy");
    // printf("final depth: %i\n", mod32.CircuitDepth());


    qcircuit mod32reduced = qcircuit(6);
    mod32reduced.H(0);
    mod32reduced.H(1);
    mod32reduced.H(2);
    mod32reduced.H(3);
    mod32reduced.H(4);
    mod32reduced.H(5);
    mod32reduced.CZ(1, 2);
    mod32reduced.CZ(3, 4);
    mod32reduced.CZ(0, 1);
    mod32reduced.CZ(2, 3);
    mod32reduced.Spacer(0, 2);
    mod32reduced.Spacer(1, 1);
    mod32reduced.Spacer(2, 1);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod32reduced.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod32reduced.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod32reduced.H(0);
    mod32reduced.H(1);
    mod32reduced.Spacer(1, 2);
    mod32reduced.H(2);
    mod32reduced.Spacer(2, 2);
    mod32reduced.H(3);
    mod32reduced.Spacer(3, 3);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod32reduced.Spacer(0, 1);
    mod32reduced.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod32reduced.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod32reduced.Spacer(2, 1);
    mod32reduced.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod32reduced.Reset(0);
    mod32reduced.Reset(1);
    mod32reduced.Reset(2);
    mod32reduced.Reset(3);
    mod32reduced.H(0);
    mod32reduced.H(1);
    mod32reduced.H(2);
    mod32reduced.H(3);
    mod32reduced.CZ(2, 5);
    mod32reduced.CZ(0, 3);
    mod32reduced.CZ(2, 4);
    mod32reduced.CZ(0, 5);
    mod32reduced.Spacer(0, 2);
    mod32reduced.Spacer(2, 2);
    mod32reduced.Spacer(4, 2);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "π");
    mod32reduced.CustomLabelOneQubitGate(2, LabelGate, "π");
    mod32reduced.CustomLabelOneQubitGate(5, LabelGate, "2α");
    mod32reduced.Spacer(0, 3);
    mod32reduced.H(0);
    mod32reduced.H(2);
    mod32reduced.H(4);
    mod32reduced.Spacer(4, 3);
    mod32reduced.H(5);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "M8");
    mod32reduced.CustomLabelOneQubitGate(2, LabelGate, "M6");
    mod32reduced.CustomLabelOneQubitGate(4, LabelGate, "M5");
    mod32reduced.Spacer(5, 2);
    mod32reduced.CustomLabelOneQubitGate(5, LabelGate, "M7");
    mod32reduced.Reset(0);
    mod32reduced.Reset(4);
    mod32reduced.Reset(5);
    mod32reduced.H(0);
    mod32reduced.Spacer(0, 1);
    mod32reduced.H(4);
    mod32reduced.H(5);
    mod32reduced.CZ(1, 5);
    mod32reduced.CZ(0, 4);
    mod32reduced.CZ(3, 5);
    mod32reduced.CZ(1, 4);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod32reduced.Spacer(0, 1);
    mod32reduced.CustomLabelOneQubitGate(4, LabelGate, "π");
    mod32reduced.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod32reduced.H(0);
    mod32reduced.Spacer(1, 1);
    mod32reduced.H(1);
    mod32reduced.Spacer(3, 4);
    mod32reduced.H(3);
    mod32reduced.H(4);
    mod32reduced.H(5);
    mod32reduced.Spacer(0, 2);
    mod32reduced.CustomLabelOneQubitGate(0, LabelGate, "M13");
    mod32reduced.CustomLabelOneQubitGate(1, LabelGate, "M11");
    mod32reduced.CustomLabelOneQubitGate(3, LabelGate, "M9");
    mod32reduced.Spacer(4, 1);
    mod32reduced.CustomLabelOneQubitGate(4, LabelGate, "M12");
    mod32reduced.Spacer(5, 4);
    mod32reduced.CustomLabelOneQubitGate(5, LabelGate, "M10");
    circuit_graph::OutputCircuit(mod32reduced, "mod32-good");

    qcircuit mod32 = qcircuit(13);
    for(int i=0; i <13;i++) {
      mod32.H(i);
    }
    for(int i = 1; i < 12; i+=2) {
      mod32.CZ(i, i+1);
    }
    for(int i = 0; i < 12; i+=2) {
      mod32.CZ(i, i+1);
    }
    mod32.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod32.H(0);
    mod32.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod32.Spacer(1, 3);
    mod32.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod32.Spacer(1, 2);
    mod32.H(1);
    mod32.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod32.Spacer(2, 1);
    mod32.H(2);
    mod32.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod32.Spacer(3, 4);
    mod32.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod32.Spacer(3, 1);
    mod32.H(3);
    mod32.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod32.Spacer(4, 1);
    mod32.H(4);
    mod32.CustomLabelOneQubitGate(4, LabelGate, "M5");
    mod32.Spacer(5, 5);
    mod32.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod32.H(5);
    mod32.CustomLabelOneQubitGate(5, LabelGate, "M6");
    mod32.Spacer(6, 8);
    mod32.CustomLabelOneQubitGate(6, LabelGate, "2α");
    mod32.H(6);
    mod32.CustomLabelOneQubitGate(6, LabelGate, "M7");
    mod32.Spacer(7, 11);
    mod32.CustomLabelOneQubitGate(7, LabelGate, "π");
    mod32.Spacer(7, 2);
    mod32.H(7);
    mod32.CustomLabelOneQubitGate(7, LabelGate, "M8");
    mod32.Spacer(8, 9);
    mod32.H(8);
    mod32.CustomLabelOneQubitGate(8, LabelGate, "M9");
    mod32.Spacer(9, 12);
    mod32.CustomLabelOneQubitGate(9, LabelGate, "π");
    mod32.Spacer(9, 1);
    mod32.H(9);
    mod32.CustomLabelOneQubitGate(9, LabelGate, "M10");
    mod32.Spacer(10, 9);
    mod32.H(10);
    mod32.CustomLabelOneQubitGate(10, LabelGate, "M11");
    mod32.Spacer(11, 13);
    mod32.CustomLabelOneQubitGate(11, LabelGate, "π");
    mod32.H(11);
    mod32.CustomLabelOneQubitGate(11, LabelGate, "M12");
    mod32.Spacer(12, 17);
    mod32.CustomLabelOneQubitGate(12, LabelGate, "α");
    mod32.H(12);
    mod32.CustomLabelOneQubitGate(12, LabelGate, "M13");

    circuit_graph::OutputCircuit(mod32, "mod32-sus");

    //(3 13) (2 5) (3 14) (5 13) (2 7) (0 3) (1 7) (3 7) (8 6) (6 5) depth: 44, noise strength: 7, eveness: 3
    //(3 13) (2 5) (3 14) (5 13) (2 10) (0 3) (1 7) (3 6) (8 6) (6 5) depth: 45, noise strength: 7, eveness: 3
    //(1 11) (7 15) (3 11) (9 13) (5 11) (4 7) (1 10) (2 7) (0 4) (7 5) depth: 40, noise strength: 7, eveness: 3

    qcircuit mod33noise = qcircuit::mod3n(3);
    mod33noise.SafeReuse(1, 11);
    mod33noise.SafeReuse(7, 15);
    mod33noise.SafeReuse(3, 11);
    mod33noise.SafeReuse(9, 13);
    mod33noise.SafeReuse(5, 11);
    mod33noise.SafeReuse(4, 7);
    mod33noise.SafeReuse(1, 10);
    mod33noise.SafeReuse(2, 7);
    mod33noise.SafeReuse(0, 4);
    mod33noise.SafeReuse(7, 5);
    circuit_graph::OutputCircuit(mod33noise, "mod33-noise");

    //(7 13) (1 11) (9 11) (1 11) (7 12) (3 11) (6 10) (4 8) (2 5) (0 4) depth: 53, noise strength: 6, eveness: 2
    qcircuit mod33even = qcircuit::mod3n(3);
    mod33even.SafeReuse(7, 13);
    mod33even.SafeReuse(1, 11);
    mod33even.SafeReuse(9, 11);
    mod33even.SafeReuse(1, 11);
    mod33even.SafeReuse(7, 12);
    mod33even.SafeReuse(3, 11);
    mod33even.SafeReuse(6, 10);
    mod33even.SafeReuse(4, 8);
    mod33even.SafeReuse(2, 5);
    mod33even.SafeReuse(0, 4);
    circuit_graph::OutputCircuit(mod33even, "mod33-even");

    //(6 13) (5 11) (1 13) (3 12) (7 10) (4 8) (6 10) (2 7) (8 7) (0 7) depth: 26, noise strength: 6, eveness: 2
    qcircuit mod33shallow = qcircuit::mod3n(3);
    mod33shallow.SafeReuse(6, 13);
    mod33shallow.SafeReuse(5, 11);
    mod33shallow.SafeReuse(1, 13);
    mod33shallow.SafeReuse(3, 12);
    mod33shallow.SafeReuse(7, 10);
    mod33shallow.SafeReuse(4, 8);
    mod33shallow.SafeReuse(6, 10);
    mod33shallow.SafeReuse(2, 7);
    mod33shallow.SafeReuse(8, 7);
    mod33shallow.SafeReuse(0, 7);
    circuit_graph::OutputCircuit(mod33shallow, "mod33-shallow");

    //(5 12) (3 14) (1 12) (7 11) (6 9) (4 8) (2 7) (6 8) (7 8) (0 7) depth: 26, noise strength: 6, eveness: 2
    qcircuit mod33best = qcircuit::mod3n(3);
    mod33best.SafeReuse(5, 12);
    mod33best.SafeReuse(3, 14);
    mod33best.SafeReuse(1, 12);
    mod33best.SafeReuse(7, 11);
    mod33best.SafeReuse(6, 9);
    mod33best.SafeReuse(4, 8);
    mod33best.SafeReuse(2, 7);
    mod33best.SafeReuse(6, 8);
    mod33best.SafeReuse(7, 8);
    mod33best.SafeReuse(0, 7);
    circuit_graph::OutputCircuit(mod33best, "mod33-best");

    qcircuit mod33 = qcircuit::mod3n(3);
    circuit_graph::OutputCircuit(mod33, "mod33-original");


    qcircuit mod33poster(17);
    for(int i = 0; i < 17; i++) {
      mod33poster.H(i);
    }
    for(int i = 1; i < 16; i+=2) {
      mod33poster.CZ(i, i + 1);
    }
    for(int i = 0; i < 16; i+=2) {
      mod33poster.CZ(i, i + 1);
    }

    mod33poster.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33poster.H(0);
    mod33poster.CustomLabelOneQubitGate(0, LabelGate, "M1");

    mod33poster.Spacer(1, 3);
    mod33poster.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33poster.Spacer(1, 3);
    mod33poster.H(1);
    mod33poster.CustomLabelOneQubitGate(1, LabelGate, "M2");

    mod33poster.Spacer(2, 1);
    mod33poster.H(2);
    mod33poster.CustomLabelOneQubitGate(2, LabelGate, "M3");

    mod33poster.Spacer(3, 4);
    mod33poster.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33poster.Spacer(3, 2);
    mod33poster.H(3);
    mod33poster.CustomLabelOneQubitGate(3, LabelGate, "M4");

    mod33poster.Spacer(4, 1);
    mod33poster.H(4);
    mod33poster.CustomLabelOneQubitGate(4, LabelGate, "M5");

    mod33poster.Spacer(5, 5);
    mod33poster.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33poster.Spacer(5, 1);
    mod33poster.H(5);
    mod33poster.CustomLabelOneQubitGate(5, LabelGate, "M6");

    mod33poster.Spacer(6, 1);
    mod33poster.H(6);
    mod33poster.CustomLabelOneQubitGate(6, LabelGate, "M7");

    mod33poster.Spacer(7, 6);
    mod33poster.CustomLabelOneQubitGate(7, LabelGate, "π");
    mod33poster.H(7);
    mod33poster.CustomLabelOneQubitGate(7, LabelGate, "M8");

    mod33poster.Spacer(8, 9);
    mod33poster.CustomLabelOneQubitGate(8, LabelGate, "2α");
    mod33poster.H(8);
    mod33poster.CustomLabelOneQubitGate(8, LabelGate, "M9");


    mod33poster.Spacer(9, 12);
    mod33poster.CustomLabelOneQubitGate(9, LabelGate, "π");
    mod33poster.Spacer(9, 3);
    mod33poster.H(9);
    mod33poster.CustomLabelOneQubitGate(9, LabelGate, "M10");

    mod33poster.Spacer(10, 10);
    mod33poster.H(10);
    mod33poster.CustomLabelOneQubitGate(10, LabelGate, "M11");

    mod33poster.Spacer(11, 13);
    mod33poster.CustomLabelOneQubitGate(11, LabelGate, "π");
    mod33poster.Spacer(11, 2);
    mod33poster.H(11);
    mod33poster.CustomLabelOneQubitGate(11, LabelGate, "M12");


    mod33poster.Spacer(12, 10);
    mod33poster.H(12);
    mod33poster.CustomLabelOneQubitGate(12, LabelGate, "M13");


    mod33poster.Spacer(13, 14);
    mod33poster.CustomLabelOneQubitGate(13, LabelGate, "π");
    mod33poster.Spacer(13, 1);
    mod33poster.H(13);
    mod33poster.CustomLabelOneQubitGate(13, LabelGate, "M14");

    mod33poster.Spacer(14, 10);
    mod33poster.H(14);
    mod33poster.CustomLabelOneQubitGate(14, LabelGate, "M15");


    mod33poster.Spacer(15, 15);
    mod33poster.CustomLabelOneQubitGate(15, LabelGate, "π");
    mod33poster.H(15);
    mod33poster.CustomLabelOneQubitGate(15, LabelGate, "M16");

    mod33poster.Spacer(16, 19);
    mod33poster.CustomLabelOneQubitGate(16, LabelGate, "α");
    mod33poster.H(16);
    mod33poster.CustomLabelOneQubitGate(16, LabelGate, "M17");

    circuit_graph::OutputCircuit(mod33poster, "mod33-poster");


    qcircuit mod33evenp(7);
    for(int i = 0; i < 7; i++) {
      mod33evenp.H(i);
    }
    mod33evenp.CZ(1, 2);
    mod33evenp.CZ(0, 1);


    mod33evenp.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33evenp.H(0);
    mod33evenp.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod33evenp.Spacer(0, 3);
    mod33evenp.Reset(0);
    mod33evenp.H(0);
    
    mod33evenp.Spacer(1, 3);
    mod33evenp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33evenp.H(1);
    mod33evenp.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod33evenp.Reset(1);
    mod33evenp.H(1);

    mod33evenp.CZ(0, 3);
    mod33evenp.CZ(2, 3);


    mod33evenp.H(2);
    mod33evenp.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod33evenp.Spacer(2, 3);
    mod33evenp.Reset(2);
    mod33evenp.H(2);


    mod33evenp.Spacer(3, 2);
    mod33evenp.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33evenp.H(3);
    mod33evenp.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod33evenp.Reset(3);
    mod33evenp.H(3);

    mod33evenp.CZ(2, 4);
    mod33evenp.CZ(0, 2);

    mod33evenp.H(0);
    mod33evenp.CustomLabelOneQubitGate(0, LabelGate, "M5");
    mod33evenp.Spacer(0, 1);
    mod33evenp.Reset(0);
    mod33evenp.H(0);
    mod33evenp.Spacer(0, 1);

    mod33evenp.Spacer(2, 2);
    mod33evenp.CustomLabelOneQubitGate(2, LabelGate, "π");
    mod33evenp.H(2);
    mod33evenp.Spacer(2, 1);
    mod33evenp.CustomLabelOneQubitGate(2, LabelGate, "M6");

    mod33evenp.CZ(0, 5);
    mod33evenp.CZ(4, 5);

    mod33evenp.H(4);
    mod33evenp.CustomLabelOneQubitGate(4, LabelGate, "M7");
    mod33evenp.Spacer(4, 3);
    mod33evenp.Reset(4);
    mod33evenp.H(4);

    mod33evenp.Spacer(5, 2);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33evenp.H(5);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "M8");
    mod33evenp.Reset(5);
    mod33evenp.H(5);

    mod33evenp.CZ(4, 6);
    mod33evenp.CZ(0, 6);

    mod33evenp.CustomLabelOneQubitGate(0, LabelGate, "2α");
    mod33evenp.H(0);
    mod33evenp.CustomLabelOneQubitGate(0, LabelGate, "M9");

    mod33evenp.Spacer(6, 3);
    mod33evenp.CustomLabelOneQubitGate(6, LabelGate, "π");
    mod33evenp.H(6);
    mod33evenp.CustomLabelOneQubitGate(6, LabelGate, "M10");
    mod33evenp.Reset(6);
    mod33evenp.H(6);

    mod33evenp.CZ(1, 6);
    mod33evenp.CZ(1, 4);

    mod33evenp.Spacer(1, 2);
    mod33evenp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33evenp.H(1);
    mod33evenp.CustomLabelOneQubitGate(1, LabelGate, "M12");
    mod33evenp.Reset(1);
    mod33evenp.H(1);

    mod33evenp.H(4);
    mod33evenp.CustomLabelOneQubitGate(4, LabelGate, "M11");

    mod33evenp.CZ(1, 5);
    mod33evenp.CZ(5, 6);

    mod33evenp.Spacer(5, 2);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33evenp.H(5);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "M14");
    mod33evenp.Reset(5);
    mod33evenp.H(5);

    mod33evenp.H(6);
    mod33evenp.CustomLabelOneQubitGate(6, LabelGate, "M13");

    mod33evenp.CZ(3, 5);
    mod33evenp.CZ(1, 3);

    mod33evenp.H(1);
    mod33evenp.CustomLabelOneQubitGate(1, LabelGate, "M15");

    mod33evenp.Spacer(3, 2);
    mod33evenp.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33evenp.H(3);
    mod33evenp.CustomLabelOneQubitGate(3, LabelGate, "M16");

    mod33evenp.Spacer(5, 6);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "α");
    mod33evenp.H(5);
    mod33evenp.CustomLabelOneQubitGate(5, LabelGate, "M17");

    circuit_graph::OutputCircuit(mod33evenp, "mod33even-poster");





    qcircuit mod33noisep(7);
    for(int i = 0; i < 7; i++) {
      mod33noisep.H(i);
    }

    mod33noisep.CZ(1, 2);
    mod33noisep.CZ(0, 1);
    mod33noisep.CZ(5, 6);

    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33noisep.H(0);
    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod33noisep.Spacer(0, 1);
    mod33noisep.Reset(0);
    mod33noisep.H(0);

    mod33noisep.Spacer(1, 3);
    mod33noisep.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33noisep.H(1);
    mod33noisep.Spacer(1, 7);
    mod33noisep.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod33noisep.Reset(1);
    mod33noisep.H(1);

    mod33noisep.CZ(0, 3);
    mod33noisep.CZ(2, 3);

    mod33noisep.H(2);
    mod33noisep.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod33noisep.Spacer(2, 3);
    mod33noisep.Reset(2);
    mod33noisep.H(2);

    mod33noisep.Spacer(3, 2);
    mod33noisep.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33noisep.H(3);
    mod33noisep.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod33noisep.Reset(3);
    mod33noisep.H(3);

    mod33noisep.CZ(1, 3);
    mod33noisep.CZ(1, 6);

    mod33noisep.Spacer(1, 2);
    mod33noisep.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33noisep.H(1);
    mod33noisep.CustomLabelOneQubitGate(1, LabelGate, "M12");
    mod33noisep.Spacer(1, 4);
    mod33noisep.Reset(1);
    mod33noisep.Spacer(1, 1);
    
    mod33noisep.H(1);

    mod33noisep.H(6);
    mod33noisep.CustomLabelOneQubitGate(6, LabelGate, "M11");
    mod33noisep.Reset(6);
    mod33noisep.H(6);

    mod33noisep.CZ(4, 6);
    mod33noisep.CZ(0, 4);

    mod33noisep.H(0);
    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "M5");
    mod33noisep.Spacer(0, 1);
    mod33noisep.Reset(0);
    mod33noisep.Spacer(0, 1);
    mod33noisep.H(0);

    mod33noisep.Spacer(4, 2);
    mod33noisep.CustomLabelOneQubitGate(4, LabelGate, "π");
    mod33noisep.H(4);
    mod33noisep.CustomLabelOneQubitGate(4, LabelGate, "M6");
    mod33noisep.Reset(4);
    mod33noisep.H(4);

    mod33noisep.CZ(0, 2);
    mod33noisep.CZ(1, 4);
    mod33noisep.CZ(0, 6);
    mod33noisep.CZ(2, 5);
    mod33noisep.Spacer(3, 1);
    mod33noisep.CZ(3, 4);

    mod33noisep.Spacer(0, 2);
    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "π");
    mod33noisep.H(0);
    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "M8");
    mod33noisep.Spacer(0, 7);
    mod33noisep.Reset(0);
    mod33noisep.H(0);

    mod33noisep.Spacer(2, 4);
    mod33noisep.CustomLabelOneQubitGate(2, LabelGate, "2α");
    mod33noisep.H(2);
    mod33noisep.CustomLabelOneQubitGate(2, LabelGate, "M9");

    mod33noisep.Spacer(3, 1);
    mod33noisep.H(3);
    mod33noisep.Spacer(3, 3);
    mod33noisep.CustomLabelOneQubitGate(3, LabelGate, "M13");

    mod33noisep.Spacer(4, 7);
    mod33noisep.CustomLabelOneQubitGate(4, LabelGate, "π");
    mod33noisep.H(4);
    mod33noisep.CustomLabelOneQubitGate(4, LabelGate, "M14");

    mod33noisep.Spacer(5, 7);
    mod33noisep.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33noisep.H(5);
    mod33noisep.Spacer(5, 1);
    mod33noisep.CustomLabelOneQubitGate(5, LabelGate, "M10");
    mod33noisep.Reset(5);
    mod33noisep.H(5);

    mod33noisep.H(6);
    mod33noisep.CustomLabelOneQubitGate(6, LabelGate, "M7");

    mod33noisep.CZ(0, 5);
    mod33noisep.CZ(1, 5);

    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33noisep.Spacer(0, 1);
    mod33noisep.H(0);
    mod33noisep.Spacer(0, 2);
    mod33noisep.CustomLabelOneQubitGate(0, LabelGate, "M17");

    mod33noisep.Spacer(1, 1);
    mod33noisep.H(1);
    mod33noisep.CustomLabelOneQubitGate(1, LabelGate, "M15");

    mod33noisep.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33noisep.H(5);
    mod33noisep.Spacer(5, 1);
    mod33noisep.CustomLabelOneQubitGate(5, LabelGate, "M16");

    circuit_graph::OutputCircuit(mod33noisep, "mod33noise-poster");


    qcircuit mod33shallowp(7);
    for(int i = 0; i < 7; i++) {
      mod33shallowp.H(i);
    }

    mod33shallowp.CZ(1, 2);
    mod33shallowp.CZ(3, 4);
    mod33shallowp.CZ(5, 6);
    mod33shallowp.CZ(0, 1);
    mod33shallowp.CZ(2, 3);
    mod33shallowp.CZ(4, 5);

    mod33shallowp.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33shallowp.H(0);
    mod33shallowp.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod33shallowp.Spacer(0, 5);
    mod33shallowp.Reset(0);
    mod33shallowp.H(0);

    mod33shallowp.CustomLabelOneQubitGate(1, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(1, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(1, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(1, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(1, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33shallowp.H(1);
    mod33shallowp.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod33shallowp.Reset(1);
    mod33shallowp.H(1);

    mod33shallowp.CustomLabelOneQubitGate(2, Blank, "");
    mod33shallowp.H(2);
    mod33shallowp.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod33shallowp.Spacer(2, 2);
    mod33shallowp.Reset(2);
    mod33shallowp.Spacer(2, 2);
    mod33shallowp.H(2);

    mod33shallowp.CustomLabelOneQubitGate(3, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(3, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(3, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(3, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33shallowp.H(3);
    mod33shallowp.CustomLabelOneQubitGate(3, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod33shallowp.Reset(3);
    mod33shallowp.Spacer(3, 3);
    mod33shallowp.H(3);

    mod33shallowp.CustomLabelOneQubitGate(4, Blank, "");
    mod33shallowp.H(4);
    mod33shallowp.CustomLabelOneQubitGate(4, LabelGate, "M5");
    mod33shallowp.Spacer(4, 2);
    mod33shallowp.Reset(4);
    mod33shallowp.Spacer(4, 2);
    mod33shallowp.H(4);

    mod33shallowp.CustomLabelOneQubitGate(5, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(5, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(5, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33shallowp.H(5);
    mod33shallowp.CustomLabelOneQubitGate(5, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(5, Blank, "");
    mod33shallowp.CustomLabelOneQubitGate(5, LabelGate, "M6");
    mod33shallowp.Reset(5);
    mod33shallowp.H(5);

    mod33shallowp.CZ(2, 4);
    mod33shallowp.CZ(0, 5);
    mod33shallowp.Spacer(5, 19);
    mod33shallowp.CZ(2, 6);

    mod33shallowp.Spacer(2, 2);

    mod33shallowp.CustomLabelOneQubitGate(2, LabelGate, "π");
    mod33shallowp.H(2);
    mod33shallowp.CustomLabelOneQubitGate(2, LabelGate, "M8");
    mod33shallowp.Reset(2);
    mod33shallowp.H(2);

    mod33shallowp.H(6);
    mod33shallowp.CustomLabelOneQubitGate(6, LabelGate, "M7");
    mod33shallowp.Reset(6);
    mod33shallowp.H(6);

    mod33shallowp.CZ(3, 6);
    mod33shallowp.CZ(0, 6);

    mod33shallowp.H(0);
    mod33shallowp.CustomLabelOneQubitGate(0, LabelGate, "M13");
    mod33shallowp.Spacer(0, 1);
    mod33shallowp.Reset(0);
    mod33shallowp.Spacer(0, 1);
    mod33shallowp.H(0);

    mod33shallowp.Spacer(6, 2);
    mod33shallowp.CustomLabelOneQubitGate(6, LabelGate, "π");
    mod33shallowp.H(6);
    mod33shallowp.CustomLabelOneQubitGate(6, LabelGate, "M14");
    mod33shallowp.Reset(6);
    mod33shallowp.H(6);

    mod33shallowp.CZ(0, 2);
    mod33shallowp.CZ(1, 6);
    mod33shallowp.Spacer(1, 2);
    mod33shallowp.CZ(0, 4);
    mod33shallowp.CZ(2, 5);
    mod33shallowp.CZ(1, 3);

    mod33shallowp.Spacer(0, 5);
    mod33shallowp.CustomLabelOneQubitGate(0, LabelGate, "π");
    mod33shallowp.H(0);
    mod33shallowp.CustomLabelOneQubitGate(0, LabelGate, "M10");

    mod33shallowp.Spacer(1, 2);
    mod33shallowp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33shallowp.H(1);
    mod33shallowp.CustomLabelOneQubitGate(1, LabelGate, "M16");

    mod33shallowp.H(2);
    mod33shallowp.CustomLabelOneQubitGate(2, LabelGate, "M11");

    mod33shallowp.H(3);
    mod33shallowp.CustomLabelOneQubitGate(3, LabelGate, "M15");
    
    mod33shallowp.CustomLabelOneQubitGate(4, LabelGate, "2α");
    mod33shallowp.Spacer(4, 2);
    mod33shallowp.H(4);
    mod33shallowp.CustomLabelOneQubitGate(4, LabelGate, "M9");

    mod33shallowp.Spacer(5, 3);
    mod33shallowp.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33shallowp.H(5);
    mod33shallowp.CustomLabelOneQubitGate(5, LabelGate, "M12");

    mod33shallowp.Spacer(6, 9);
    mod33shallowp.CustomLabelOneQubitGate(6, LabelGate, "α");
    mod33shallowp.H(6);
    mod33shallowp.CustomLabelOneQubitGate(6, LabelGate, "M17");

    circuit_graph::OutputCircuit(mod33shallowp, "mod33best-poster");



    qcircuit mod33bestp(7);
    for(int i = 0; i < 7; i++) {
      mod33bestp.H(i);
    }

    mod33bestp.CZ(1, 2);
    mod33bestp.CZ(3, 4);
    mod33bestp.CZ(5, 6);
    mod33bestp.CZ(0, 1);
    mod33bestp.CZ(2, 3);
    mod33bestp.CZ(4, 5);

    mod33bestp.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33bestp.H(0);
    mod33bestp.CustomLabelOneQubitGate(0, LabelGate, "M1");
    mod33bestp.Spacer(0, 5);
    mod33bestp.Reset(0);
    mod33bestp.H(0);

    mod33bestp.CustomLabelOneQubitGate(1, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(1, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(1, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(1, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(1, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33bestp.H(1);
    mod33bestp.CustomLabelOneQubitGate(1, LabelGate, "M2");
    mod33bestp.Reset(1);
    mod33bestp.H(1);

    mod33bestp.CustomLabelOneQubitGate(2, Blank, "");
    mod33bestp.H(2);
    mod33bestp.CustomLabelOneQubitGate(2, LabelGate, "M3");
    mod33bestp.Spacer(2, 2);
    mod33bestp.Reset(2);
    mod33bestp.Spacer(2, 2);
    mod33bestp.H(2);

    mod33bestp.CustomLabelOneQubitGate(3, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(3, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(3, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(3, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33bestp.H(3);
    mod33bestp.CustomLabelOneQubitGate(3, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(3, LabelGate, "M4");
    mod33bestp.Reset(3);
    mod33bestp.Spacer(3, 2);
    mod33bestp.H(3);

    mod33bestp.CustomLabelOneQubitGate(4, Blank, "");
    mod33bestp.H(4);
    mod33bestp.CustomLabelOneQubitGate(4, LabelGate, "M5");
    mod33bestp.Spacer(4, 2);
    mod33bestp.Reset(4);
    mod33bestp.Spacer(4, 2);
    mod33bestp.H(4);

    mod33bestp.CustomLabelOneQubitGate(5, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(5, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(5, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(5, LabelGate, "π");
    mod33bestp.H(5);
    mod33bestp.CustomLabelOneQubitGate(5, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(5, Blank, "");
    mod33bestp.CustomLabelOneQubitGate(5, LabelGate, "M6");
    mod33bestp.Reset(5);
    mod33bestp.H(5);

    mod33bestp.CZ(2, 4);
    mod33bestp.CZ(2, 6);

    mod33bestp.Spacer(2, 2);
    mod33bestp.CustomLabelOneQubitGate(2, LabelGate, "π");
    mod33bestp.H(2);
    mod33bestp.CustomLabelOneQubitGate(2, LabelGate, "M8");
    mod33bestp.Reset(2);
    mod33bestp.H(2);

    mod33bestp.H(6);
    mod33bestp.CustomLabelOneQubitGate(6, LabelGate, "M7");
    mod33bestp.Reset(6);
    mod33bestp.H(6);

    mod33bestp.CZ(0, 6);
    mod33bestp.CZ(2, 5);
    mod33bestp.CZ(0, 2);
    mod33bestp.CZ(4, 6);

    mod33bestp.Spacer(0, 1);
    mod33bestp.H(0);
    mod33bestp.CustomLabelOneQubitGate(0, LabelGate, "M11");
    mod33bestp.Spacer(0, 2);
    mod33bestp.Reset(0);
    mod33bestp.Spacer(0, 1);
    mod33bestp.H(0);

    mod33bestp.Spacer(2, 4);
    mod33bestp.CustomLabelOneQubitGate(2, LabelGate, "π");
    mod33bestp.H(2);
    mod33bestp.CustomLabelOneQubitGate(2, LabelGate, "M12");

    mod33bestp.CustomLabelOneQubitGate(4, LabelGate, "2α");
    mod33bestp.H(4);
    mod33bestp.CustomLabelOneQubitGate(4, LabelGate, "M9");

    mod33bestp.Spacer(6, 3);
    mod33bestp.CustomLabelOneQubitGate(6, LabelGate, "π");
    mod33bestp.H(6);
    mod33bestp.Spacer(6, 1);
    mod33bestp.CustomLabelOneQubitGate(6, LabelGate, "M10");
    mod33bestp.Reset(6);
    mod33bestp.H(6);

    mod33bestp.CZ(0, 3);
    mod33bestp.CZ(1, 6);
    mod33bestp.CZ(1, 5);
    mod33bestp.CZ(3, 6);

    mod33bestp.Spacer(0, 9);
    mod33bestp.CustomLabelOneQubitGate(0, LabelGate, "α");
    mod33bestp.H(0);
    mod33bestp.CustomLabelOneQubitGate(0, LabelGate, "M17");

    mod33bestp.Spacer(1, 4);
    mod33bestp.CustomLabelOneQubitGate(1, LabelGate, "π");
    mod33bestp.H(1);
    mod33bestp.CustomLabelOneQubitGate(1, LabelGate, "M14");

    mod33bestp.Spacer(3, 2);
    mod33bestp.CustomLabelOneQubitGate(3, LabelGate, "π");
    mod33bestp.Spacer(3, 1);
    mod33bestp.H(3);
    mod33bestp.CustomLabelOneQubitGate(3, LabelGate, "M16");

    mod33bestp.H(5);
    mod33bestp.CustomLabelOneQubitGate(5, LabelGate, "M13");

    mod33bestp.H(6);
    mod33bestp.CustomLabelOneQubitGate(6, LabelGate, "M15");

    circuit_graph::OutputCircuit(mod33bestp, "mod33shallow-poster");





































































    // FILE * fileWriteout;
    // fileWriteout = fopen ("mod33.txt","w");
    // int n = 3;
    // qcircuit mod3n = qcircuit::mod3n(n);
    // std::vector<std::vector<bool>> circcc = mod3n.CircuitCausalCone();

    // std::vector<std::vector<std::pair<int, int>>> minim = circuit_graph::CausalConeHeuristicReduction(circcc, circuit_graph::mod3nRestrictions(n), 9999, 10);
    // printf("mhmmm\n");
    // fprintf(fileWriteout, "All Reductions\n\n");
    // for(std::vector<std::pair<int, int>> v : minim) {
    //   mod3n = qcircuit::mod3n(n);
    //   for(std::pair<int, int> p : v) {
    //     fprintf(fileWriteout, "(%i %i) ", p.first, p.second);
    //     mod3n.SafeReuse(p.first, p.second);
    //   }
    //   fprintf(fileWriteout, "depth: %i, noise strength: %i, eveness: %i\n", mod3n.CircuitDepth(), circuit_graph::noiseStrength(v, 4*n+5), circuit_graph::ReductionEveness(v, 4*n+5));
    // }

    // fprintf(fileWriteout, "\n\nBest Circuit Depth\n\n");
    // for(std::vector<std::pair<int, int>> v : circuit_graph::smallestDepthMod3nReduction(minim, n)) {
    //   mod3n = qcircuit::mod3n(n);
    //   for(std::pair<int, int> p : v) {
    //     fprintf(fileWriteout, "(%i %i) ", p.first, p.second);
    //     mod3n.SafeReuse(p.first, p.second);
    //   }
    //   fprintf(fileWriteout, "depth: %i, noise strength: %i, eveness: %i\n", mod3n.CircuitDepth(), circuit_graph::noiseStrength(v, 4*n+5), circuit_graph::ReductionEveness(v, 4*n+5));
    // }

    // fprintf(fileWriteout, "\n\nBest Noise Strength\n\n");
    // for(std::vector<std::pair<int, int>> v : circuit_graph::bestNoiseStrengthMod3nReduction(minim, n)) {
    //   mod3n = qcircuit::mod3n(n);
    //   for(std::pair<int, int> p : v) {
    //     fprintf(fileWriteout, "(%i %i) ", p.first, p.second);
    //     mod3n.SafeReuse(p.first, p.second);
    //   }
    //   fprintf(fileWriteout, "depth: %i, noise strength: %i, eveness: %i\n", mod3n.CircuitDepth(), circuit_graph::noiseStrength(v, 4*n+5), circuit_graph::ReductionEveness(v, 4*n+5));
    // }

    // fprintf(fileWriteout, "\n\nBest Even Reuse Gates\n\n");
    // for(std::vector<std::pair<int, int>> v : circuit_graph::bestEvenReductionsMod3nReduction(minim, n)) {
    //   mod3n = qcircuit::mod3n(n);
    //   for(std::pair<int, int> p : v) {
    //     fprintf(fileWriteout, "(%i %i) ", p.first, p.second);
    //     mod3n.SafeReuse(p.first, p.second);
    //   }
    //   fprintf(fileWriteout, "depth: %i, noise strength: %i, eveness: %i\n", mod3n.CircuitDepth(), circuit_graph::noiseStrength(v, 4*n+5), circuit_graph::ReductionEveness(v, 4*n+5));
    // }

    // fclose(fileWriteout);

    // printf("size is aaaaa %lu. reduced to %lu\n", minim.size(), 4*n+5 - minim[0].size());


//(7 11) (1 10) (4 10) (3 9) (2 6) (0 4) (6 4)
    qcircuit mod35 = qcircuit::mod3n(2);
    circuit_graph::OutputCircuit(mod35, "cirq-first");
    mod35.Reuse(7, 11);
    mod35.Reuse(1, 10);
    mod35.Reuse(4, 10);
    mod35.Reuse(3, 9);
    mod35.Reuse(2, 6);
    mod35.Reuse(0, 4);
    mod35.Reuse(6, 4);
    circuit_graph::OutputCircuit(mod35, "cirq");
    
    // printf("depth: %i\n", mod35.CircuitDepth());
    // mod35.Reuse(5, 8);
    // printf("depth: %i\n", mod35.CircuitDepth());
    // mod35.Reuse(4, 7);
    // printf("depth: %i\n", mod35.CircuitDepth());
    // mod35.Reuse(1, 7);
    // printf("depth: %i\n", mod35.CircuitDepth());
    // mod35.Reuse(2, 6);
    // printf("depth: %i\n", mod35.CircuitDepth());
    // mod35.Reuse(0, 4);

    circuit_graph::OutputCircuit(mod35, "correct-hmmm");

    qcircuit pp = qcircuit::mod3n(2);
    qcircuit ppp = qcircuit::mod3n(2);

    qcircuit four = qcircuit::clusterState(3);
    //four.Reuse(2, 0);
    circuit_graph::OutputCircuit(four, "one");




    printf("%s\n", chp_simulation::equivalent(mod35,ppp) ? "true" : "false");


  // (6 10) (8 11) (3 10) (4 7) (1 8) (2 5) (0 4) depth: 17
  // (1 9) (0 7) (7 10) (6 9) (3 8) (4 0) (1 4) depth: 17
  qcircuit first = qcircuit::mod3n(2);
  first.Reuse(6, 10);
  first.Reuse(8, 11);
  first.Reuse(3, 10);
  first.Reuse(4, 7);
  first.Reuse(1, 8);
  first.Reuse(2, 5);
  first.Reuse(0, 4);
  circuit_graph::OutputCircuit(first, "first_choice");

  qcircuit second = qcircuit::mod3n(2);
  second.Reuse(1, 9);
  second.Reuse(0, 7);
  second.Reuse(7, 10);
  second.Reuse(6, 9);
  second.Reuse(3, 8);
  second.Reuse(4, 0);
  second.Reuse(1, 4);
  circuit_graph::OutputCircuit(second, "second_choice");


    //qcircuit mod32 = qcircuit::mod3n(5);
    //circuit_graph::(mod32, "god");

    // QState* q;
    // q = (QState*)malloc(sizeof(struct QState));
    // initstae_(q,4,NULL);
    // printtableu(q);
    // printf("\n\n");
    // hadamard(q, 2);
    // cnot(q, 2, 0);
    // hadamard(q, 1);
    // hadamard(q, 0);
    // cnot(q, 3, 2);
    // printtableu(q);
    // printf("\n\n");
    // reset(q, 2);
    // printtableu(q);

    return 0;
}