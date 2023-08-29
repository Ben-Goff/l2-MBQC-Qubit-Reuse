#include "../include-shared/circuit_graph.hpp"
#include "../include-shared/constants.hpp"
#include "../include/qcircuit.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <set>
#include <iterator>

//using namespace ogdf;

bool circuit_graph::finishedDrawing(std::vector<std::optional<std::tuple<ogdf::node, Gate*, bool>>> currentLayer) {
    for(auto o : currentLayer) {
        if(o.has_value()) {return false;}
    }
    return true;
}

//Prints the input "circuit" out to "file".svg
void circuit_graph::OutputCircuit(qcircuit circuit, std::string file) {
    //at an index i, currentLayer holds
    //1) The most recently printed node for qbit i
    //2) A pointer to the next Gate to be printed for qbit i
    //3) A boolean stating if the Gate is ready to be updated. True means 1) is a printing of 2), false means 1) is a printing of a gate that points to 2)
    int layer = 0;
    std::vector<std::optional<std::tuple<ogdf::node, Gate*, bool>>> currentLayer(circuit.getroots()->size());
    std::vector<Gate*> roots = *(circuit.getroots());
    ogdf::Graph G;
    ogdf::GraphAttributes GA(G, ogdf::GraphAttributes::all);
    for(int i = 0; i < currentLayer.size(); i++) {
        ogdf::node r = G.newNode();
        GA.x(r) = layer*NODE_HORZ_SEP;
        GA.y(r) = i*NODE_VERT_SEP;
        GA.width(r) = NODE_WIDTH + 3;
        GA.height(r) = NODE_HEIGHT;
        GA.strokeType(r) = ogdf::StrokeType::None;
        GA.fillColor(r) = ogdf::Color("#CDEAFA");
        GA.label(r) = "|0〉";
        GA.xLabel(r) = layer*NODE_HORZ_SEP + 3;
        currentLayer[i] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(r, roots[i], true)};
    }
    layer++;
    while(!finishedDrawing(currentLayer)) {
        //update level of gates to be drawn
        for(int i = 0; i < currentLayer.size(); i++) {
            //if the gate's node has been printed
            if(currentLayer[i].has_value() && std::get<2>(currentLayer[i].value())) {
                //if it was the last gate for this qubit, remove entry from currentLayer
                if(std::get<1>(currentLayer[i].value())->isEnd(i)) {
                    currentLayer[i] = std::nullopt;
                //else update the gate by traversing the gate to the next gate for that qubit
                } else {
                Gate* newGate = std::get<2>(*std::get<1>(currentLayer[i].value())->findEdge(i)).value();
                ogdf::node currNode = std::get<0>(currentLayer[i].value());
                currentLayer[i] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(currNode, newGate, false)};
                }
            }
        }
        //draw gates that have every qubit ready to be printed
        std::set<Gate*> currentGates;
        for(auto i : currentLayer) {
            if(i.has_value()) {
                currentGates.insert(std::get<1>(i.value()));
            }
        }
        std::set<Gate*> printableGates;
        for(auto g : currentGates) {
            bool printable = true;
            //for each edge in every gate which can be printed....
            for(std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e : g->edges) {
                int* qbit = std::get<1>(e);
                //....we check if the qbit at that edge has printed every gate up to this gate and is ready to print
                if(std::get<2>(currentLayer[*qbit].value()) || std::get<1>(currentLayer[*qbit].value()) != g) {
                    printable = false;
                }
            }
            if(printable) {
                printableGates.insert(g);
            }
        }
        std::vector<int> cantPrint;
        for(auto g : printableGates) {
            switch(g->type) {
                //first all of the one-qubit gates. This will prevent two-qubit gates from visually running over one-qubit gate
                case Blank:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    ogdf::node n = G.newNode();
                    GA.x(n) = layer*NODE_HORZ_SEP;
                    GA.y(n) = qbit*NODE_VERT_SEP;
                    GA.width(n) = 0;
                    GA.height(n) = 0;
                    ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[qbit].value()),n);
                    GA.arrowType(ej) = ogdf::EdgeArrow::None;
                    currentLayer[qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[qbit].value()), true)};
                    break;
                }
                case ResetGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    if(std::find(cantPrint.begin(), cantPrint.end(), qbit) != cantPrint.end()) {
                        break;
                    }
                    cantPrint.push_back(qbit);
                    ogdf::node n = G.newNode();
                    GA.x(n) = layer*NODE_HORZ_SEP;
                    GA.y(n) = qbit*NODE_VERT_SEP;
                    GA.width(n) = NODE_WIDTH;
                    GA.height(n) = NODE_HEIGHT;
                    GA.label(n) = g->customLabel.value_or("R");
                    GA.yLabel(n) = NODE_LABEL_Y;
                    ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[qbit].value()),n);
                    GA.arrowType(ej) = ogdf::EdgeArrow::None;
                    currentLayer[qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[qbit].value()), true)};
                    break;
                }
                case MeasureGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    if(std::find(cantPrint.begin(), cantPrint.end(), qbit) != cantPrint.end()) {
                        break;
                    }
                    cantPrint.push_back(qbit);
                    ogdf::node n = G.newNode();
                    GA.x(n) = layer*NODE_HORZ_SEP;
                    GA.y(n) = qbit*NODE_VERT_SEP;
                    GA.width(n) = NODE_WIDTH + 10;
                    GA.height(n) = NODE_HEIGHT;
                    GA.label(n) = g->customLabel.value_or("M");
                    GA.yLabel(n) = NODE_LABEL_Y;
                    ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[qbit].value()),n);
                    GA.arrowType(ej) = ogdf::EdgeArrow::None;
                    currentLayer[qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[qbit].value()), true)};
                    break;
                }
                case LabelGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    if(std::find(cantPrint.begin(), cantPrint.end(), qbit) != cantPrint.end()) {
                        break;
                    }
                    cantPrint.push_back(qbit);
                    ogdf::node n = G.newNode();
                    GA.x(n) = layer*NODE_HORZ_SEP;
                    GA.y(n) = qbit*NODE_VERT_SEP;
                    GA.width(n) = NODE_WIDTH + 3*(int)(g->customLabel.value().length() - 2 + 2*std::count(g->customLabel.value().begin(), g->customLabel.value().end(), 'M'));
                    GA.height(n) = NODE_HEIGHT;
                    GA.label(n) = g->customLabel.value();
                    GA.yLabel(n) = NODE_LABEL_Y;
                    ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[qbit].value()),n);
                    GA.arrowType(ej) = ogdf::EdgeArrow::None;
                    currentLayer[qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[qbit].value()), true)};
                    break;
                }
                case HadamardGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    if(std::find(cantPrint.begin(), cantPrint.end(), qbit) != cantPrint.end()) {
                        break;
                    }
                    cantPrint.push_back(qbit);
                    ogdf::node n = G.newNode();
                    GA.x(n) = layer*NODE_HORZ_SEP;
                    GA.y(n) = qbit*NODE_VERT_SEP;
                    GA.width(n) = NODE_WIDTH;
                    GA.height(n) = NODE_HEIGHT;
                    GA.fillPattern(n) = ogdf::FillPattern::None;
                    GA.label(n) = g->customLabel.value_or("H");
                    GA.yLabel(n) = NODE_LABEL_Y;
                    ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[qbit].value()),n);
                    GA.arrowType(ej) = ogdf::EdgeArrow::None;
                    currentLayer[qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[qbit].value()), true)};
                    break;
                }
                case CNOTGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    bool printable = true;
                    if(qbit1 < qbit2) {
                        for(int q = qbit1; q <= qbit2; q++) {
                            printable = printable && (std::find(cantPrint.begin(), cantPrint.end(), q) == cantPrint.end());
                        } 
                    } else {
                        for(int q = qbit2; q <= qbit1; q++) {
                            printable = printable && (std::find(cantPrint.begin(), cantPrint.end(), q) == cantPrint.end());
                        }
                    }
                    if(!printable) {
                        break;
                    }
                    if(qbit1 < qbit2) {
                        for(int q = qbit1; q <= qbit2; q++) {
                            cantPrint.push_back(q);
                        }
                    } else {
                        for(int q = qbit2; q <= qbit1; q++) {
                            cantPrint.push_back(q);
                        }
                    }
                    ogdf::node n1 = G.newNode();
                    ogdf::node n2 = G.newNode();
                    GA.x(n1) = layer*NODE_HORZ_SEP;
                    GA.y(n1) = (qbit1)*NODE_VERT_SEP;
                    GA.x(n2) = layer*NODE_HORZ_SEP;
                    GA.y(n2) = (qbit2)*NODE_VERT_SEP;
                    GA.width(n1) = NODE_WIDTH/2;
                    GA.height(n1) = NODE_HEIGHT/2;
                    GA.shape(n1) = ogdf::Shape::Ellipse;
                    GA.fillColor(n1) = ogdf::Color::Name::Black;
                    ogdf::edge ej1 = G.newEdge(std::get<0>(currentLayer[qbit1].value()),n1);
                    GA.arrowType(ej1) = ogdf::EdgeArrow::None;
                    currentLayer[qbit1] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n1, std::get<1>(currentLayer[qbit1].value()), true)};
                    GA.width(n2) = NODE_WIDTH;
                    GA.height(n2) = NODE_HEIGHT;
                    GA.label(n2) = "+";
                    GA.yLabel(n2) = NODE_LABEL_Y;
                    GA.shape(n2) = ogdf::Shape::Ellipse;
                    ogdf::edge ej2 = G.newEdge(std::get<0>(currentLayer[qbit2].value()),n2);
                    GA.arrowType(ej2) = ogdf::EdgeArrow::None;
                    currentLayer[qbit2] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n2, std::get<1>(currentLayer[qbit2].value()), true)};
                    ogdf::edge ej3 = G.newEdge(n1, n2);
                    GA.arrowType(ej3) = ogdf::EdgeArrow::None;
                    break;
                }
                case CZGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    bool printable = true;
                    if(qbit1 < qbit2) {
                        for(int q = qbit1; q <= qbit2; q++) {
                            printable = printable && (std::find(cantPrint.begin(), cantPrint.end(), q) == cantPrint.end());
                        } 
                    } else {
                        for(int q = qbit2; q <= qbit1; q++) {
                            printable = printable && (std::find(cantPrint.begin(), cantPrint.end(), q) == cantPrint.end());
                        }
                    }
                    if(!printable) {
                        break;
                    }
                    if(qbit1 < qbit2) {
                        for(int q = qbit1; q <= qbit2; q++) {
                            cantPrint.push_back(q);
                        }
                    } else {
                        for(int q = qbit2; q <= qbit1; q++) {
                            cantPrint.push_back(q);
                        }
                    }
                    ogdf::node n1 = G.newNode();
                    ogdf::node n2 = G.newNode();
                    GA.x(n1) = layer*NODE_HORZ_SEP;
                    GA.y(n1) = (qbit1)*NODE_VERT_SEP;
                    GA.x(n2) = layer*NODE_HORZ_SEP;
                    GA.y(n2) = (qbit2)*NODE_VERT_SEP;
                    GA.width(n1) = NODE_WIDTH/2;
                    GA.height(n1) = NODE_HEIGHT/2;
                    GA.shape(n1) = ogdf::Shape::Ellipse;
                    GA.fillColor(n1) = ogdf::Color::Name::Black;
                    ogdf::edge ej1 = G.newEdge(std::get<0>(currentLayer[qbit1].value()),n1);
                    GA.arrowType(ej1) = ogdf::EdgeArrow::None;
                    currentLayer[qbit1] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n1, std::get<1>(currentLayer[qbit1].value()), true)};
                    GA.width(n2) = NODE_WIDTH/2;
                    GA.height(n2) = NODE_HEIGHT/2;
                    GA.shape(n2) = ogdf::Shape::Ellipse;
                    GA.fillColor(n2) = ogdf::Color::Name::Black;
                    ogdf::edge ej2 = G.newEdge(std::get<0>(currentLayer[qbit2].value()),n2);
                    GA.arrowType(ej2) = ogdf::EdgeArrow::None;
                    currentLayer[qbit2] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n2, std::get<1>(currentLayer[qbit2].value()), true)};
                    ogdf::edge ej3 = G.newEdge(n1, n2);
                    GA.arrowType(ej3) = ogdf::EdgeArrow::None;
                    break;
                }
                default: printf("Unrecognized Gate Type\n");
            }
        }
        //update how many layers have been printed so far
        layer++;
    }

    //GraphIO::write(GA, file + ".gml", GraphIO::writeGML);
    ogdf::GraphIO::write(GA, file + ".svg", ogdf::GraphIO::drawSVG);

}

std::vector<std::vector<std::pair<int, int>>> MinimizeHelper(int qbits, std::vector<std::vector<std::pair<int, int>>> reductions) {
    qcircuit circuit = qcircuit::clusterState(qbits);
    std::vector<std::vector<std::pair<int, int>>> newReductions;
    int currentReduction = qbits - reductions[0].size();
    bool reachedBound;
    for(auto vec : reductions) {
        reachedBound = false;
        for(int i = 0; i < currentReduction; i++) {
            for(int j = 0; j < currentReduction; j++) {
                circuit = qcircuit::clusterState(qbits);
                for(std::pair<int, int> elem : vec) {
                    circuit.SafeReuse(elem.first, elem.second);
                }
                while(!circuit.Reuse(i, j)) {
                    if(j == currentReduction - 1) {
                        if(i == currentReduction - 1) {
                            reachedBound = true;
                            break;
                        } else {
                            i++;
                            j = 0;
                        }
                    } else {
                        j++;
                    }
                }
                if(!reachedBound) {
                    std::vector<std::pair<int, int>> newReduction;
                    newReduction.reserve(vec.size() + 1);
                    newReduction.insert(newReduction.end(), vec.begin(), vec.end());
                    newReduction.push_back(std::make_pair(i, j));
                    newReductions.push_back(newReduction);
                }
            }
        }
    }
    if(newReductions.size() > 0) {
        return MinimizeHelper(qbits, newReductions);
    } else {
        return reductions;
    }
}

std::vector<std::vector<std::pair<int, int>>> circuit_graph::MinimizeClusterState(int qbits) {
    qcircuit circuit = qcircuit::clusterState(qbits);
    std::vector<std::vector<std::pair<int, int>>> reductions;
    for(int i = 0; i < circuit.Qbits(); i++) {
      for(int j = 1; j < circuit.Qbits(); j++) {
        if(circuit.Reuse(i, j)) {
            reductions.push_back(std::vector<std::pair<int, int>>{std::make_pair(i, j)});
        }
        circuit = qcircuit::clusterState(qbits);
      }
    }
    return MinimizeHelper(qbits, reductions);
}

std::vector<std::vector<std::pair<int, int>>> ThreadedMinimizeHelper(int qbits, std::vector<std::vector<std::pair<int, int>>> reductions, int threads) {
    size_t reductionsCount = reductions.size();
    auto lambd = [reductions, qbits](int start, int end) {
        std::vector<std::pair<int, int>> curr;
        qcircuit circuit = qcircuit::clusterState(qbits);
        std::vector<std::vector<std::pair<int, int>>> threadReductions;
        int currentReduction = qbits - reductions[0].size();
        bool reachedBound;
        for(int i = start; i < end; i++) {
            curr = reductions[i];
            reachedBound = false;
            for(int i = 0; i < currentReduction; i++) {
                for(int j = 0; j < currentReduction; j++) {
                    circuit = qcircuit::clusterState(qbits);
                    for(std::pair<int, int> elem : curr) {
                        circuit.SafeReuse(elem.first, elem.second);
                    }
                    if(i == 0 && j == 0) {
                    }
                    while(!circuit.Reuse(i, j)) {
                        if(j == currentReduction - 1) {
                            if(i == currentReduction - 1) {
                                reachedBound = true;
                                break;
                            } else {
                                i++;
                                j = 0;
                            }
                        } else {
                            j++;
                        }
                    }
                    if(!reachedBound) {
                        std::vector<std::pair<int, int>> newReduction;
                        newReduction.reserve(curr.size() + 1);
                        newReduction.insert(newReduction.end(), curr.begin(), curr.end());
                        newReduction.push_back(std::make_pair(i, j));
                        threadReductions.push_back(newReduction);
                    }
                }
            }
        }
        return threadReductions;
    };

    std::vector<std::future<std::vector<std::vector<std::pair<int, int>>>>> futures(threads);
    size_t threadSize = reductionsCount / threads;
    size_t overflow = reductionsCount % threadSize;
    for(int i = 0; i < threads - 1; i++) {
        futures[i] = std::async(std::launch::async, lambd, i*threadSize, (i+1)*threadSize);
    }
    futures[threads - 1] = std::async(std::launch::async, lambd, (threads - 1)*threadSize, reductionsCount);
    std::future_status status;
    //this determine how often we check if threads are completed
    std::chrono::milliseconds span(600);
    std::vector<std::vector<std::pair<int, int>>> newReductions;
    std::vector<std::vector<std::pair<int, int>>> currFuture;
    for(int i = 0; i < threads; i++) {
        do {
            switch(status = futures[i].wait_for(span); status) {
                case std::future_status::deferred: /*std::cout << "deferred\n";*/ break;
                case std::future_status::timeout: /*std::cout << "timeout\n";*/ break;
                case std::future_status::ready: /*std::cout << "ready!\n";*/ break;
            }
        } while (status != std::future_status::ready);
        currFuture = futures[i].get();
        newReductions.reserve(newReductions.size() + currFuture.size());
        newReductions.insert(newReductions.end(), currFuture.begin(), currFuture.end());
        }
    return newReductions;
}

std::vector<std::vector<std::pair<int, int>>> circuit_graph::ThreadedMinimizeClusterState(int qbits, int threads) {
    qcircuit circuit = qcircuit::clusterState(qbits);
    std::vector<std::vector<std::pair<int, int>>> reductions;
    for(int i = 0; i < circuit.Qbits(); i++) {
      for(int j = 0; j < circuit.Qbits(); j++) {
        if(circuit.Reuse(i, j)) {
            reductions.push_back(std::vector<std::pair<int, int>>{std::make_pair(i, j)});
        }
        circuit = qcircuit::clusterState(qbits);
      }
    }
    std::vector<std::vector<std::pair<int, int>>> possibleBestReductions = reductions;
    std::vector<std::vector<std::pair<int, int>>> bestReductions;
    int levelsReduced = 0;
    do {
        levelsReduced++;
        bestReductions = possibleBestReductions;
        possibleBestReductions = ThreadedMinimizeHelper(qbits, possibleBestReductions, threads);
    } while(possibleBestReductions.size() > 0);
    return bestReductions;
}

std::vector<std::vector<std::pair<int, int>>> CausalConeHeuristicReductionHelper(std::tuple<std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<std::vector<bool>>>, std::vector<bool**>> input, int s) {
    printf("heuristic helper called. %lu\n", std::get<0>(input).size());
    std::vector<std::vector<std::pair<int, int>>> reductions = std::get<0>(input);
    std::vector<std::vector<std::vector<bool>>> circuits = std::get<1>(input);
    std::vector<bool**> restrictionsVec = std::get<2>(input);
    //slack controls how much "worse" we let each individual step be. At upper limit, this becomes a brute force search
    //but low slack number (like 0) may still become brute force later on in the search. this is because low slack places a bottleneck in the first few reduction steps, so at later steps, essentially every reduction will have the same CC
    //lower number also has less optimal reductions. For example, size 30 reduces to 12 with slack 0, but 10 with slack 4
    //slack does seem to be more important at initial steps. The last 1/3 of steps seem to all have same CC size
    int slack = s;
    int qbits = circuits[0].size();
    //seting least additions to causal cones to max possible value (qbits^2 implies every qbit relies on ever other qbit)
    int leastAdditions = qbits*qbits;

    //std::vector<std::pair<int, std::pair<int, std::pair<int, int>>>> nextReductionStep;

    //nextReductionStep[i] is a list of reductions that have cc leastAdditions + i, 0 <= i <= slack
    //nextReductionStep[i][elem].second is the next step add on to reductions[nextReductionStep[i][elem].first]
    //the elements are pointers so that they can be easily shifted up as leastAdditions is reduced
    std::vector<std::vector<std::pair<int, std::pair<int, int>>>> nextReductionStep(slack + 1);

    int conesIncrease;
    int circuitConeSize;
    //calculates the total size increase in circuit causal cones if we reuse(i, j)
    std::set<int> difference;
    for(int k = 0; k < reductions.size(); k++) {
        printf("%lu\n", reductions.size() - k);
        std::vector<std::vector<bool>> circuit = circuits[k];
        bool** restrictions = restrictionsVec[k];
        circuitConeSize = 0;
        for(std::vector<bool> s : circuit) {
            for(bool b : s) {
                conesIncrease+=(int)b;
            }
        }
        bool unrestricted;
        for(int i = 0; i < qbits; i++) {
            for(int j = 0; j < qbits; j++) {
                //if reuse i to j, must make sure that for any element c that has j in its causal cone, that restrictions[i][c]
                //and also make sure that resrictions[i][j]
                unrestricted = restrictions[i][j];
                for(int q = 0; q < qbits; q++) {
                    if(circuit[q][j]) {
                        unrestricted = unrestricted && restrictions[i][q];
                    }
                }
                if(!circuit[i][j] && !circuit[j][i] && unrestricted) {
                    //THIS CHANGES THE CODE FROM CALCULATING BASED ON CONE DIFFERENCE TO TOTAL CONE SIZE. TO REVERT, MAKE IT EQUAL TO ZERO
                    conesIncrease = circuitConeSize;

                    //reusing from i to j means concatenating causal cone of i to causal cone of all qbits that have j in their causal cone
                    std::vector<bool> cone;
                    for(int q = 0; q < qbits; q++) {
                        if(q != j) {
                            cone = circuit[q];
                            if(cone[j]) {
                                for(int l = 0; l < qbits; l++) {
                                    conesIncrease+= (circuit[i][l] && !cone[l]);
                                }
                                //minus one because we remove j
                                conesIncrease--;
                            }
                        }
                    }
                    //reusing from i to j also means concatenating causal cone of j to causal cone of i
                    for(int l = 0; l < qbits; l++) {
                        conesIncrease+= (circuit[j][l] && !circuit[i][l]);
                    }
                    //minus 1 because we remove j
                    conesIncrease--;

                    //and then getting rid of cc of j
                    for(int l = 0; l < qbits; l++) {
                        conesIncrease-= circuit[j][l];
                    }

                    //no copying, just maintain slack + 1 lists, one for each count of additions
                    if(conesIncrease <= leastAdditions + slack) {
                        std::pair<int, int> pair = std::make_pair(i, j);
                        if(conesIncrease < leastAdditions) {
                            int improvement = leastAdditions - conesIncrease;

                            for(int l = slack; (l - improvement) >= 0; l--) {
                                std::pair<int, std::pair<int, int>>* d = nextReductionStep[l - improvement].data();
                                nextReductionStep[l].assign(d, d + nextReductionStep[l - improvement].size());
                            }
                            for(int l = 0; l < improvement && l <= slack; l++) {
                                nextReductionStep[l].clear();
                            }
                            leastAdditions = conesIncrease;
                            nextReductionStep[0].push_back(std::make_pair(k, pair));
                        } else {
                            nextReductionStep[conesIncrease - leastAdditions].push_back(std::make_pair(k, pair));
                        }
                    }
                }
            }
        }
    }


    std::vector<std::vector<std::pair<int, int>>> newReductions;
    std::vector<std::vector<std::vector<bool>>> reducedCircuits;
    std::vector<bool**> newRestrictions;
    for(std::vector<std::pair<int, std::pair<int, int>>> vec : nextReductionStep) {
        for(std::pair<int, std::pair<int, int>> r : vec) {
            //add the reduction steps to the list
            std::vector<std::pair<int, int>> newReduction = reductions[r.first];
            //THIS COPYING TAKES A LOOOOOOONG TIME, NEED TO REMOVE
            newReduction.push_back(r.second);
            newReductions.push_back(newReduction);

            //this is where we will store the new causal cones
            std::vector<std::vector<bool>> reducedCircuit(qbits - 1);
            for(int p = 0; p < qbits - 1; p++) {
                reducedCircuit[p].resize(qbits - 1);
            }
            std::vector<std::vector<bool>> currOrigCircuit = circuits[r.first];

            //then we copy over the previous circuit with the jth row an column removed
            for(int l = 0; l < r.second.second; l++) {
                for(int m = 0; m < r.second.second; m++) {
                    reducedCircuit[l][m] = currOrigCircuit[l][m];
                }
            }
            for(int l = r.second.second + 1; l < qbits; l++) {
                for(int m = 0; m < r.second.second; m++) {
                    reducedCircuit[l - 1][m] = currOrigCircuit[l][m];
                }
            }
            for(int l = 0; l < r.second.second; l++) {
                for(int m = r.second.second + 1; m < qbits; m++) {
                    reducedCircuit[l][m - 1] = currOrigCircuit[l][m];
                }
            }
            for(int l = r.second.second + 1; l < qbits; l++) {
                for(int m = r.second.second + 1; m < qbits; m++) {
                    reducedCircuit[l - 1][m - 1] = currOrigCircuit[l][m];
                }
            }

            //then for all p which depends on j, we concatenate the cc of i to cc of p
            for(int p = 0; p < r.second.second; p++) {
                if(currOrigCircuit[p][r.second.second]) {
                    for(int q = 0; q < r.second.second; q++) {
                        reducedCircuit[p][q] = currOrigCircuit[p][q] || currOrigCircuit[r.second.first][q];
                    }
                    for(int q = r.second.second + 1; q < qbits; q++) {
                        reducedCircuit[p][q - 1] = currOrigCircuit[p][q] || currOrigCircuit[r.second.first][q];
                    }
                }
            }
            for(int p = r.second.second + 1; p < qbits; p++) {
                if(currOrigCircuit[p][r.second.second]) {
                    for(int q = 0; q < r.second.second; q++) {
                        reducedCircuit[p - 1][q] = currOrigCircuit[p][q] || currOrigCircuit[r.second.first][q];
                    }
                    for(int q = r.second.second + 1; q < qbits; q++) {
                        reducedCircuit[p - 1][q - 1] = currOrigCircuit[p][q] || currOrigCircuit[r.second.first][q];
                    }
                }
            }


            //we concatente cc of j to cc of i
            if(r.second.second > r.second.first) {
            for(int q = 0; q < r.second.second; q++) {
            reducedCircuit[r.second.first][q] = currOrigCircuit[r.second.first][q] || currOrigCircuit[r.second.second][q];
            }
            for(int q = r.second.second + 1; q < qbits; q++) {
                reducedCircuit[r.second.first][q - 1] = currOrigCircuit[r.second.first][q] || currOrigCircuit[r.second.second][q];
            }
            } else {
                //minus one if r.first is larger than r.second because then r.first will be shifted down one
                for(int q = 0; q < r.second.second; q++) {
                reducedCircuit[r.second.first - 1][q] = currOrigCircuit[r.second.first][q] || currOrigCircuit[r.second.second][q];
                }
                for(int q = r.second.second + 1; q < qbits; q++) {
                    reducedCircuit[r.second.first - 1][q - 1] = currOrigCircuit[r.second.first][q] || currOrigCircuit[r.second.second][q];
                }
            }

            //and add it to list of circuits
            reducedCircuits.push_back(reducedCircuit);


            bool** rest = new bool*[qbits - 1];
            for(int p = 0; p < qbits - 1; p++) {
                rest[p] = new bool[qbits - 1];
            }

            //top left quadrant
            for(int m = 0; m < r.second.second; m++) {
                for(int n = 0; n < r.second.second; n++) {
                    rest[m][n] = restrictionsVec[r.first][m][n];
                }
            }

            //top right quadrant
            for(int m = r.second.second; m < qbits - 1; m++) {
                for(int n = 0; n < r.second.second; n++) {
                    rest[m][n] = restrictionsVec[r.first][m + 1][n];
                }
            }

            //bottom left quadrant
            for(int m = r.second.second; m < qbits - 1; m++) {
                for(int n = 0; n < r.second.second; n++) {
                    rest[n][m] = restrictionsVec[r.first][n][m+1];
                }
            }

            //bottom right quadrant
            for(int m = r.second.second; m < qbits - 1; m++) {
                for(int n = r.second.second; n < qbits - 1; n++) {
                    rest[n][m] = restrictionsVec[r.first][n+1][m+1];
                }
            }

            //new restrictions
            for(int m = 0; m < qbits; m++) {
                if(!restrictionsVec[r.first][r.second.second][m]) {
                    rest[r.second.first - (r.second.first > r.second.second ? 1 : 0)][m - (m > r.second.second ? 1 : 0)] = false;
                }
            }

            newRestrictions.push_back(rest);
        }
    }

    //free old restrictions
    for(bool** r : restrictionsVec) {
        for(int i = 0; i < qbits; i++) {
            free(r[i]);
        }
        free(r);
    }


    if(newReductions.size() == 0) {
        printf("we are done!\n");
        return reductions;
    } else {
        return CausalConeHeuristicReductionHelper(std::make_tuple(newReductions, reducedCircuits, newRestrictions), s);
    }
}

std::vector<std::vector<std::pair<int, int>>> circuit_graph::CausalConeHeuristicReduction(std::vector<std::vector<bool>> circuit, int s1, int s2) {
    int circuit_size = circuit.size();
    bool** rest = new bool*[circuit_size];
    for(int i = 0; i < circuit_size; i++) {
        rest[i] = new bool[circuit_size];
        for(int j = 0; j < circuit_size; j++) {
            rest[i][j] = true;
        }
    }
    return CausalConeHeuristicReduction(circuit, rest, s1, s2);
}

std::vector<std::vector<std::pair<int, int>>> circuit_graph::CausalConeHeuristicReduction(std::vector<std::vector<bool>> circuit, bool** restrictions, int s1, int s2) {
    printf("heuristic called\n");
    //slack controls how much "worse" we let each individual step be. At upper limit, this becomes a brute force search
    //for some reason the actual slack seems to be one less than this variable.
    int slack = s1;
    int qbits = circuit.size();
    //seting least additions to causal cones to max possible value (qbits^2 implies every qbit relies on ever other qbit)
    int leastAdditions = (int)pow(qbits, 2);
    std::vector<std::pair<int, std::pair<int, int>>> nextReductionStep;

    int conesIncrease;
    std::set<int> difference;
    //calculates the total size increase in circuit causal cones if we reuse(i, j)
    for(int i = 0; i < qbits; i++) {
        for(int j = 0; j < qbits; j++) {
            if(i != j && !circuit[i][j] && !circuit[j][i] && restrictions[i][j]) {
                conesIncrease = 0;

                //THIS CHANGES THE CODE FROM CALCULATING BASED ON CONE DIFFERENCE TO TOTAL CONE SIZE
                // for(std::vector<bool> s : circuit) {
                //     for(bool b : s) {
                //         conesIncrease+=(int)b;
                //     }
                // }

                //reusing from i to j means concatenating causal cone of i to causal cone of all qbits that have j in their causal cone
                std::vector<bool> cone;
                for(int k = 0; k < qbits; k++) {
                    if(k != j) {
                        cone = circuit[k];
                        if(cone[j]) {
                            for(int l = 0; l < qbits; l++) {
                                conesIncrease+= (circuit[i][l] && !cone[l]);
                            }
                            //minus one because we remove j
                            conesIncrease--;
                        }
                    }
                }
                //reusing from i to j also means concatenating causal cone of j to causal cone of i
                for(int l = 0; l < qbits; l++) {
                    conesIncrease+= (circuit[j][l] && !circuit[i][l]);
                }
                //minus 1 because we remove j
                conesIncrease--;

                //and then getting rid of cc of j
                for(int l = 0; l < qbits; l++) {
                    conesIncrease-= circuit[j][l];
                }

                if(conesIncrease <= leastAdditions + slack) {
                    if(conesIncrease < leastAdditions) {
                        leastAdditions = conesIncrease;
                        std::vector<std::pair<int, std::pair<int, int>>> temp;
                        std::copy_if(nextReductionStep.begin(), nextReductionStep.end(), std::inserter(temp, temp.begin()), [leastAdditions, slack](std::pair<int, std::pair<int, int>> el){return el.first <= leastAdditions + slack;});
                        nextReductionStep.clear();
                        nextReductionStep = temp;
                    }
                    std::pair<int, int> best = std::make_pair(i, j);
                    nextReductionStep.push_back(std::make_pair(conesIncrease, best));
                }
            }
        }
    }

    std::vector<std::pair<int, int>> optimalReductionStep;
    std::transform(nextReductionStep.begin(), nextReductionStep.end(), std::inserter(optimalReductionStep, optimalReductionStep.begin()), [](std::pair<int, std::pair<int, int>> el){return el.second;});

    std::vector<std::vector<std::pair<int, int>>> reductions(optimalReductionStep.size());
    std::vector<std::vector<std::vector<bool>>> reducedCircuits(optimalReductionStep.size());
    std::vector<bool**> restrictionsVec(optimalReductionStep.size());
    std::tuple<std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<std::vector<bool>>>, std::vector<bool**>> reduced = std::make_tuple(reductions, reducedCircuits, restrictionsVec);
    
    for(int i = 0; i < optimalReductionStep.size(); i++) {
        std::pair<int, int> r = optimalReductionStep[i];
        //this is where we will store the new causal cones
        //first we allocate the correct size
        std::vector<std::vector<bool>> reducedCircuit(qbits - 1);
        for(int k = 0; k < qbits - 1; k++) {
            reducedCircuit[k].resize(qbits - 1);
        }
        //then we copy over the previous circuit with the jth row an column removed
        for(int l = 0; l < r.second; l++) {
            for(int m = 0; m < r.second; m++) {
                reducedCircuit[l][m] = circuit[l][m];
            }
        }
        for(int l = r.second + 1; l < qbits; l++) {
            for(int m = 0; m < r.second; m++) {
                reducedCircuit[l - 1][m] = circuit[l][m];
            }
        }
        for(int l = 0; l < r.second; l++) {
            for(int m = r.second + 1; m < qbits; m++) {
                reducedCircuit[l][m - 1] = circuit[l][m];
            }
        }
        for(int l = r.second + 1; l < qbits; l++) {
            for(int m = r.second + 1; m < qbits; m++) {
                reducedCircuit[l - 1][m - 1] = circuit[l][m];
            }
        }
        //then for all k which depends on j, we concatenate the cc of i to cc of k
        for(int k = 0; k < r.second; k++) {
            if(circuit[k][r.second]) {
                for(int q = 0; q < r.second; q++) {
                    reducedCircuit[k][q] = circuit[k][q] || circuit [r.first][q];
                }
                for(int q = r.second + 1; q < qbits; q++) {
                    reducedCircuit[k][q - 1] = circuit[k][q] || circuit [r.first][q];
                }
            }
        }
        for(int k = r.second + 1; k < qbits; k++) {
            if(circuit[k][r.second]) {
                for(int q = 0; q < r.second; q++) {
                    reducedCircuit[k - 1][q] = circuit[k][q] || circuit [r.first][q];
                }
                for(int q = r.second + 1; q < qbits; q++) {
                    reducedCircuit[k - 1][q - 1] = circuit[k][q] || circuit [r.first][q];
                }
            }
        }

        //we concatente cc of j to cc of i
        if(r.second > r.first) {
            for(int q = 0; q < r.second; q++) {
            reducedCircuit[r.first][q] = circuit[r.first][q] || circuit [r.second][q];
            }
            for(int q = r.second + 1; q < qbits; q++) {
                reducedCircuit[r.first][q - 1] = circuit[r.first][q] || circuit [r.second][q];
            }
        } else {
            //minus one if r.first is larger than r.second because then r.first will be shifted down one
            for(int q = 0; q < r.second; q++) {
            reducedCircuit[r.first - 1][q] = circuit[r.first][q] || circuit [r.second][q];
            }
            for(int q = r.second + 1; q < qbits; q++) {
                reducedCircuit[r.first - 1][q - 1] = circuit[r.first][q] || circuit [r.second][q];
            }
        }

        bool** rest = new bool*[qbits - 1];
        for(int p = 0; p < qbits - 1; p++) {
            rest[p] = new bool[qbits - 1];
        }

        //top left quadrant
        for(int m = 0; m < r.second; m++) {
            for(int n = 0; n < r.second; n++) {
                rest[m][n] = restrictions[m][n];
            }
        }

        //top right quadrant
        for(int m = r.second; m < qbits - 1; m++) {
            for(int n = 0; n < r.second; n++) {
                rest[m][n] = restrictions[m + 1][n];
            }
        }

        //bottom left quadrant
        for(int m = r.second; m < qbits - 1; m++) {
            for(int n = 0; n < r.second; n++) {
                rest[n][m] = restrictions[n][m+1];
            }
        }

        //bottom right quadrant
        for(int m = r.second; m < qbits - 1; m++) {
            for(int n = r.second; n < qbits - 1; n++) {
                rest[n][m] = restrictions[n+1][m+1];
            }
        }

        //new restrictions
        for(int m = 0; m < qbits; m++) {
            if(!restrictions[r.second][m]) {
                //printf("qubits: %i\nr.first: %i\nr.second: %i\n", qbits, r.first, r.second);
                rest[r.first - (r.first > r.second ? 1 : 0)][m - (m > r.second ? 1 : 0)] = false;
            }
        }

        std::get<0>(reduced)[i] = {r};
        std::get<1>(reduced)[i] = reducedCircuit;
        std::get<2>(reduced)[i] = rest;
    }

    //free old restrictions
    for(int i = 0; i < qbits; i++) {
        free(restrictions[i]);
    }
    free(restrictions);
    
    return CausalConeHeuristicReductionHelper(reduced, s2);
}

bool** circuit_graph::emptyRestrictions(int size) {
    bool** rest = new bool*[size];
    for(int i = 0; i < size; i++) {
        rest[i] = new bool[size];
        for(int j = 0; j < size; j++) {
            rest[i][j] = true;
        }
    }
    return rest;
}

bool** circuit_graph::mod3nRestrictions(int n) {
    bool** rest = new bool*[4*n+5];
    for(int i = 0; i < 4*n+5; i++) {
        rest[i] = new bool[4*n+5];
        for(int j = 0; j < 4*n+5; j++) {
            rest[i][j] = true;
        }
    }

    // - 1 everywhere to make up for zero indexing (paper is 1 index)

    //step 2
    for(int j = 2; j <= 2*n+2; j+=2) {
        for(int i = 1; i<j; i+=2) {
            rest[j - 1][i - 1] = false;
        }
    }

    //step 3
    for(int j = 2; j <= 2*n+3; j+=2) {
        rest[2*n+3 - 1][j - 1] = false;
    }

    //step 4
    for(int j = 2*n+4; j <= 4*n+2; j+=2) {
        for(int i = 1; i<j; i+=2) {
            rest[j - 1][i - 1] = false;
        }
    }

    //weird part of step 4 here!!!
    for(int i = 1; i<4*n+4; i+=2) {
        rest[4*n+4 - 1][i - 1] = false;
    }

    //step 5
    for(int i = 2; i<4*n+5; i+=2) {
        rest[4*n+5 - 1][i - 1] = false;
    }
    return rest;
}

//CONSIDER CHANGING DEPTH FUNCTION TO SHOW PRINTABLE DEPTH
//takes in a list of circuit reductions and the n value (circuit has 4n+5 qubits), and returns the reduction(s) that has the smallest circuit depth
std::vector<std::vector<std::pair<int, int>>> circuit_graph::smallestDepthMod3nReduction(std::vector<std::vector<std::pair<int, int>>> reductions, int n) {
    if(reductions.size() == 0) {
        throw std::invalid_argument("received empty reductions vector");
    }
    std::vector<std::vector<std::pair<int, int>>> ret;
    qcircuit mod3n = qcircuit::mod3n(n);
    int smallest = -1;
    int curr_depth;
    for(int i = 0; i < reductions.size(); i++) {
        mod3n = qcircuit::mod3n(n);
        for(std::pair<int, int> p : reductions[i]) {
            mod3n.SafeReuse(p.first, p.second);
        }
        curr_depth = mod3n.CircuitDepth();
        if(curr_depth == smallest || smallest == -1) {
            ret.push_back(reductions[i]);
            smallest = curr_depth;
        } else if(curr_depth < smallest) {
            ret.clear();
            ret.push_back(reductions[i]);
            smallest = curr_depth;
        }
    }
    return ret;
}

int circuit_graph::noiseStrength(std::vector<std::pair<int, int>> reduction, int n) {
    std::vector<std::vector<std::pair<int, int>>> ret;
    qcircuit mod3n = qcircuit::mod3n(n);
    int curr_max;
    int curr_qubit_max;
    std::vector<Gate*> curr_roots;
    Gate* curr_root_gate;
    mod3n = qcircuit::mod3n(n);
    for(std::pair<int, int> p : reduction) {
        mod3n.SafeReuse(p.first, p.second);
    }
    curr_max = 0;
    curr_roots = *mod3n.getroots();
    for(int j = 0; j < mod3n.Qbits(); j++) {
        curr_qubit_max = 0;
        curr_root_gate = curr_roots[j];
        while(std::get<2>(*(curr_root_gate->findEdge(j))).has_value()) {
            curr_root_gate = std::get<2>(*(curr_root_gate->findEdge(j))).value();
            if(curr_root_gate->edges.size() == 2) {
                curr_qubit_max++;
            }
        }
        curr_max = std::max(curr_max, curr_qubit_max);
    }
    return curr_max;
}

int circuit_graph::ReductionEveness(std::vector<std::pair<int, int>> reduction, int n) {
    std::vector<std::vector<std::pair<int, int>>> ret;
    qcircuit mod3n = qcircuit::mod3n(n);
    int curr_max;
    int curr_qubit_max;
    std::vector<Gate*> curr_roots;
    Gate* curr_root_gate;
    mod3n = qcircuit::mod3n(n);
    for(std::pair<int, int> p : reduction) {
        mod3n.SafeReuse(p.first, p.second);
    }
    curr_max = 0;
    curr_roots = *mod3n.getroots();
    for(int j = 0; j < mod3n.Qbits(); j++) {
        curr_qubit_max = 0;
        curr_root_gate = curr_roots[j];
        while(std::get<2>(*(curr_root_gate->findEdge(j))).has_value()) {
            curr_root_gate = std::get<2>(*(curr_root_gate->findEdge(j))).value();
            if(curr_root_gate->type == ResetGate) {
                curr_qubit_max++;
            }
        }
        curr_max = std::max(curr_max, curr_qubit_max);
    }
    return curr_max;
}

//takes in a list of circuit reductions and the n value (circuit has 4n+5 qubits), and returns the reduction(s) that has the highest noise strength (least number of 2 qubit gates on any given qubit \ minmax)
std::vector<std::vector<std::pair<int, int>>> circuit_graph::bestNoiseStrengthMod3nReduction(std::vector<std::vector<std::pair<int, int>>> reductions, int n) {
    if(reductions.size() == 0) {
        throw std::invalid_argument("received empty reductions vector");
    }
    std::vector<std::vector<std::pair<int, int>>> ret;
    qcircuit mod3n = qcircuit::mod3n(n);
    int smallest = -1;
    int curr_max;
    int curr_qubit_max;
    std::vector<Gate*> curr_roots;
    Gate* curr_root_gate;
    for(int i = 0; i < reductions.size(); i++) {
        mod3n = qcircuit::mod3n(n);
        for(std::pair<int, int> p : reductions[i]) {
            mod3n.SafeReuse(p.first, p.second);
        }
        curr_max = 0;
        curr_roots = *mod3n.getroots();
        for(int j = 0; j < mod3n.Qbits(); j++) {
            curr_qubit_max = 0;
            curr_root_gate = curr_roots[j];
            while(std::get<2>(*(curr_root_gate->findEdge(j))).has_value()) {
                curr_root_gate = std::get<2>(*(curr_root_gate->findEdge(j))).value();
                if(curr_root_gate->edges.size() == 2) {
                    curr_qubit_max++;
                }
            }
            curr_max = std::max(curr_max, curr_qubit_max);
        }
        if(smallest == -1 || curr_max == smallest) {
            smallest = curr_max;
            ret.push_back(reductions[i]);
        } else if(curr_max < smallest) {
            smallest = curr_max;
            ret.clear();
            ret.push_back(reductions[i]);

        }
    }
    return ret;
}

//takes in a list of circuit reductions and the n value (circuit has 4n+5 qubits), and returns the reduction(s) that has the least number of reset gates on the qubit with the most number of reset gates
std::vector<std::vector<std::pair<int, int>>> circuit_graph::bestEvenReductionsMod3nReduction(std::vector<std::vector<std::pair<int, int>>> reductions, int n) {
    if(reductions.size() == 0) {
        throw std::invalid_argument("received empty reductions vector");
    }
    std::vector<std::vector<std::pair<int, int>>> ret;
    qcircuit mod3n = qcircuit::mod3n(n);
    int smallest = -1;
    int curr_max;
    int curr_qubit_max;
    std::vector<Gate*> curr_roots;
    Gate* curr_root_gate;
    for(int i = 0; i < reductions.size(); i++) {
        mod3n = qcircuit::mod3n(n);
        for(std::pair<int, int> p : reductions[i]) {
            mod3n.SafeReuse(p.first, p.second);
        }
        curr_max = 0;
        curr_roots = *mod3n.getroots();
        for(int j = 0; j < mod3n.Qbits(); j++) {
            curr_qubit_max = 0;
            curr_root_gate = curr_roots[j];
            while(std::get<2>(*(curr_root_gate->findEdge(j))).has_value()) {
                curr_root_gate = std::get<2>(*(curr_root_gate->findEdge(j))).value();
                if(curr_root_gate->type == ResetGate) {
                    curr_qubit_max++;
                }
            }
            curr_max = std::max(curr_max, curr_qubit_max);
        }
        if(smallest == -1 || curr_max == smallest) {
            smallest = curr_max;
            ret.push_back(reductions[i]);
        } else if(curr_max < smallest) {
            smallest = curr_max;
            ret.clear();
            ret.push_back(reductions[i]);

        }
    }
    return ret;
}