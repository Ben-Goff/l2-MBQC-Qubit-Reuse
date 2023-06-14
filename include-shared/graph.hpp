#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <optional>

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

#include "../include-shared/constants.hpp"
#include "../include/qcircuit.hpp"

using namespace ogdf;

void OutputCircuit(qcircuit circuit, std::string file);

void addH(Graph g, int qbit);

void addM(Graph g, int qbit);

void addCNOT(Graph g, int qbit1, int qbit2);