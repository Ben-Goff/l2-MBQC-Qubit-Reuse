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
        GA.label(r) = "|0〉";
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
        std::set<int> printedOneQubitGateQubits;
        for(auto g : printableGates) {
            if(g->edges.size() == 1) {
                if(g->type != Blank) {
                    printedOneQubitGateQubits.insert(*std::get<1>(g->edges[0]));
                }
                switch(g->type) {
                    //first all of the one-qubit gates. This will prevent two-qubit gates from visually running over one-qubit gate
                    case Blank:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        ogdf::node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = 0;
                        GA.height(n) = 0;
                        ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    case ResetGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        ogdf::node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.label(n) = "R";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    case MeasureGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        ogdf::node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.label(n) = "M";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    case HadamardGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        ogdf::node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.fillPattern(n) = ogdf::FillPattern::None;
                        GA.label(n) = "H";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        ogdf::edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    default: printf("Multi-Qubit gate flagged as One-Qubit gate\n");
                }
            }
        }
        for(auto g : printableGates) {
            if(g->edges.size() == 2) {
                std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                int qbit1 = *std::get<1>(e1);
                int qbit2 = *std::get<1>(e2);
                bool wontOverlapOneQubitGate = true;
                for(int i : printedOneQubitGateQubits) {
                    if(qbit1 < qbit2) {
                        wontOverlapOneQubitGate = (i < qbit1 && i > qbit2);
                    } else {
                        wontOverlapOneQubitGate = (i > qbit1 && i < qbit2);
                    }
                }
                if(wontOverlapOneQubitGate) {
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
                    switch(g->type) {
                        case CNOTGate:
                        {
                            GA.width(n2) = NODE_WIDTH;
                            GA.height(n2) = NODE_HEIGHT;
                            GA.label(n2) = "+";
                            GA.yLabel(n2) = NODE_LABEL_Y;
                            GA.shape(n2) = ogdf::Shape::Ellipse;
                            break;
                        }
                        case CZGate:
                        {
                            GA.width(n2) = NODE_WIDTH/2;
                            GA.height(n2) = NODE_HEIGHT/2;
                            GA.shape(n2) = ogdf::Shape::Ellipse;
                            GA.fillColor(n2) = ogdf::Color::Name::Black;
                            break;
                        }
                        default: printf("One-Qubit gate flagged as Two-Qubit gate\n");
                    }
                    ogdf::edge ej2 = G.newEdge(std::get<0>(currentLayer[qbit2].value()),n2);
                    GA.arrowType(ej2) = ogdf::EdgeArrow::None;
                    currentLayer[qbit2] = std::optional<std::tuple<ogdf::node, Gate*, bool>>{std::make_tuple(n2, std::get<1>(currentLayer[qbit2].value()), true)};
                    ogdf::edge ej3 = G.newEdge(n1, n2);
                    GA.arrowType(ej3) = ogdf::EdgeArrow::None;
                }
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

std::pair<std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<std::set<int>>>> CausalConeHeuristicReductionHelper(std::pair<std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<std::set<int>>>> input) {
    printf("heuristic helper called. %lu\n", input.first.size());
    std::vector<std::vector<std::pair<int, int>>> reductions = input.first;
    std::vector<std::vector<std::set<int>>> circuits = input.second;
    //slack controls how much "worse" we let each individual step be. At upper limit, this becomes a brute force search
    //but low slack number (like 0) may still become brute force later on in the search. this is because low slack places a bottleneck in the first few reduction steps, so at later steps, essentially every reduction will have the same CC
    //lower number also has less optimal reductions. For example, size 30 reduces to 12 with slack 0, but 10 with slack 4
    //slack does seem to be more important at initial steps. The last 1/3 of steps seem to all have same CC size
    int slack = 5;
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
        std::vector<std::set<int>> circuit = circuits[k];
        circuitConeSize = 0;
        for(std::set<int> s : circuit) {
            circuitConeSize+=s.size();
        }
        for(int i = 0; i < qbits; i++) {
            for(int j = 0; j < qbits; j++) {
                if(circuit[i].count(j) == 0 && circuit[j].count(i) == 0) {
                    //THIS CHANGES THE CODE FROM CALCULATING BASED ON CONE DIFFERENCE TO TOTAL CONE SIZE. TO REVERT, MAKE IT EQUAL TO ZERO
                    conesIncrease = circuitConeSize;

                    //reusing from i to j means concatenating causal cone of i to causal cone of all qbits that have j in their causal cone
                    std::set<int> cone;
                    for(int k = 0; k < circuit.size(); k++) {
                        if(k != j) {
                            cone = circuit[k];
                            if(cone.count(j) == 1) {
                                difference = {};
                                std::set_difference(circuit[i].begin(), circuit[i].end(), cone.begin(), cone.end(), std::inserter(difference, difference.begin()));
                                conesIncrease+=(int)(difference.size());
                                //minus one because we remove j
                                conesIncrease--;
                            }
                        }
                    }
                    //reusing from i to j also means concatenating causal cone of j to causal cone of i
                    difference.clear();
                    std::set_difference(circuit[j].begin(), circuit[j].end(), circuit[i].begin(), circuit[i].end(), std::inserter(difference, difference.begin()));
                    conesIncrease+=(int)(difference.size());
                    //minus 1 because we remove j
                    conesIncrease--;

                    //and then getting rid of cc of j
                    conesIncrease-=circuit[j].size();

                    //no copying, just maintain slack + 1 lists, one for each count of additions
                    if(conesIncrease <= leastAdditions + slack) {
                        std::pair<int, int> pair = std::make_pair(i, j);
                        if(conesIncrease < leastAdditions) {
                            int improvement = leastAdditions - conesIncrease;

                            for(int l = slack; (l - improvement) >= 0; l--) {
                                nextReductionStep[l] = nextReductionStep[l - improvement];
                                printf("yep %p %p\n", &nextReductionStep[l], &nextReductionStep[l - improvement]);
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
    std::vector<std::vector<std::set<int>>> reducedCircuits;
    for(std::vector<std::pair<int, std::pair<int, int>>> vec : nextReductionStep) {
        for(std::pair<int, std::pair<int, int>> r : vec) {
            //add the reduction steps to the list
            std::vector<std::pair<int, int>> newReduction = reductions[r.first];
            newReduction.push_back(r.second);
            newReductions.push_back(newReduction);

            //this is where we will store the new causal cones
            std::vector<std::set<int>> reducedCircuit(qbits);
            std::vector<std::set<int>> currOrigCircuit = circuits[r.first];
            for(int k = 0; k < currOrigCircuit.size(); k++) {
                if(k != r.second.first && k != r.second.second) {
                    if(currOrigCircuit[k].count(r.second.second) == 1) {
                        //if k depends on j, we concatenate the cc of i to it
                        std::set_union(currOrigCircuit[k].begin(), currOrigCircuit[k].end(),
                        currOrigCircuit[r.second.first].begin(), currOrigCircuit[r.second.first].end(),
                        std::inserter(reducedCircuit[k], reducedCircuit[k].begin()));
                    } else {
                        //if it is not, we just copy
                        reducedCircuit[k] = currOrigCircuit[k];
                    }
                }
            }


            //we concatente cc of j to cc of i
            std::set_union(currOrigCircuit[r.second.first].begin(), currOrigCircuit[r.second.first].end(),
            currOrigCircuit[r.second.second].begin(), currOrigCircuit[r.second.second].end(),
            std::inserter(reducedCircuit[r.second.first], reducedCircuit[r.second.first].begin()));


            //now remove j from reduced circuit, and reducedCircuit is complete
            reducedCircuit.erase(reducedCircuit.begin() + r.second.second);

            //and we shift each qbit above the deleted qbit down by one
            for(int j = 0; j < reducedCircuit.size(); j++) {
                std::set<int> new_s;
                for(int k : reducedCircuit[j]) {
                    if(k != r.second.second) {
                        new_s.insert(k - (int)(k > r.second.second));
                    }
                }
                reducedCircuit[j] = new_s;
            }

            //and add it to list of circuits
            reducedCircuits.push_back(reducedCircuit);
        }
    }

    if(newReductions.size() == 0) {
        return input;
    } else {
        return CausalConeHeuristicReductionHelper(std::make_pair(newReductions, reducedCircuits));
    }
}

std::vector<std::vector<std::pair<int, int>>> circuit_graph::CausalConeHeuristicReduction(std::vector<std::set<int>> circuit) {
    printf("heuristic called\n");
    //slack controls how much "worse" we let each individual step be. At upper limit, this becomes a brute force search
    //for some reason the actual slack seems to be one less than this variable.
    int slack = 5;
    int qbits = circuit.size();
    //seting least additions to causal cones to max possible value (qbits^2 implies every qbit relies on ever other qbit)
    int leastAdditions = (int)pow(qbits, 2);
    std::vector<std::pair<int, std::pair<int, int>>> nextReductionStep;

    int conesIncrease;
    std::set<int> difference;
    //calculates the total size increase in circuit causal cones if we reuse(i, j)
    for(int i = 0; i < qbits; i++) {
        for(int j = 0; j < qbits; j++) {
            if(i != j && circuit[i].count(j) == 0 && circuit[j].count(i) == 0) {
                conesIncrease = 0;

                //THIS CHANGES THE CODE FROM CALCULATING BASED ON CONE DIFFERENCE TO TOTAL CONE SIZE
                for(std::set<int> s : circuit) {
                    conesIncrease+=s.size();
                }

                //reusing from i to j means concatenating causal cone of i to causal cone of all qbits that have j in their causal cone
                std::set<int> cone;
                for(int k = 0; k < circuit.size(); k++) {
                    if(k != j) {
                        cone = circuit[k];
                        if(cone.count(j) == 1) {
                            difference = {};
                            std::set_difference(circuit[i].begin(), circuit[i].end(), cone.begin(), cone.end(), std::inserter(difference, difference.begin()));
                            conesIncrease+=(int)(difference.size());
                            //minus one because we remove j
                            conesIncrease--;
                        }
                    }
                }
                //reusing from i to j also means concatenating causal cone of j to causal cone of i
                difference.clear();
                std::set_difference(circuit[j].begin(), circuit[j].end(), circuit[i].begin(), circuit[i].end(), std::inserter(difference, difference.begin()));
                conesIncrease+=(int)(difference.size());
                //minus 1 because we remove j
                conesIncrease--;

                //and then getting rid of cc of j
                conesIncrease-=circuit[j].size();

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

    // TO DEBUG: will print out size of calculates and actual causal cones to see if there is a difference
    //     for(std::pair<int, std::pair<int, int>> pp : nextReductionStep) {
    //     qcircuit eleven = qcircuit::clusterState(11);
    //     eleven.Reuse(pp.second.first, pp.second.second);
    //     int actualCC = 0;
    //     for(std::set<int> s : eleven.CircuitCausalCone()) {
    //         actualCC+=s.size();
    //     }
    //     int supposedCC = pp.first;
    //     if(supposedCC != actualCC) {
    //         printf("ERROR: %i, calculated as %i, but should be %i for reduction %i %i\n", supposedCC - actualCC, supposedCC, actualCC, pp.second.first, pp.second.second);
    //     }
    // }

    std::vector<std::pair<int, int>> optimalReductionStep;
    std::transform(nextReductionStep.begin(), nextReductionStep.end(), std::inserter(optimalReductionStep, optimalReductionStep.begin()), [](std::pair<int, std::pair<int, int>> el){return el.second;});

    std::vector<std::vector<std::pair<int, int>>> reductions(optimalReductionStep.size());
    std::vector<std::vector<std::set<int>>> reducedCircuits(optimalReductionStep.size());
    std::pair<std::vector<std::vector<std::pair<int, int>>>, std::vector<std::vector<std::set<int>>>> reduced = std::make_pair(reductions, reducedCircuits);
    
    for(int i = 0; i < optimalReductionStep.size(); i++) {
        std::pair<int, int> r = optimalReductionStep[i];
        //this is where we will store the new causal cones
        std::vector<std::set<int>> reducedCircuit(qbits);
        for(int k = 0; k < circuit.size(); k++) {
            if(k != r.second && k != r.first) {
                if(circuit[k].count(r.second) == 1) {
                    //if k depends on j, we concatenate the cc of i to it
                    std::set_union(circuit[k].begin(), circuit[k].end(),
                    circuit[r.first].begin(), circuit[r.first].end(),
                    std::inserter(reducedCircuit[k], reducedCircuit[k].begin()));
                } else {
                    //if it is not, we just copy
                    std::set<int> original(circuit[k]);
                    reducedCircuit[k] = original;
                }
            }
        }

        //we concatente cc of j to cc of i
        std::set_union(circuit[r.first].begin(), circuit[r.first].end(),
        circuit[r.second].begin(), circuit[r.second].end(),
        std::inserter(reducedCircuit[r.first], reducedCircuit[r.first].begin()));

        //now remove j from reduced circuit, and reducedCircuit is complete
        reducedCircuit.erase(reducedCircuit.begin() + r.second);

        //and we shift each qbit above the deleted qbit down by one
        for(int j = 0; j < reducedCircuit.size(); j++) {
            std::set<int> new_s;
            for(int k : reducedCircuit[j]) {
                if(k != r.second) {
                    new_s.insert(k - (int)(k > r.second));
                }
            }
            reducedCircuit[j] = new_s;
        }
        reduced.first[i] = {r};
        reduced.second[i] = reducedCircuit;
    }
    
    return CausalConeHeuristicReductionHelper(reduced).first;
}