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
#include "../include-shared/gate.hpp"

//using namespace ogdf;

struct circuit_graph {
    public:
    static void OutputCircuit(qcircuit circuit, std::string file);

    static bool finishedDrawing(std::vector<std::optional<std::tuple<ogdf::node, Gate*, bool>>> currentLayer);

    static std::vector<std::vector<std::pair<int, int>>> MinimizeClusterState(int qbits);

    static std::vector<std::vector<std::pair<int, int>>> ThreadedMinimizeClusterState(int qbits, int threads);

    static std::vector<std::vector<std::pair<int, int>>> CausalConeHeuristicReduction(std::vector<std::set<int>> circuit);
};