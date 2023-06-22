#include "../include-shared/circuit_graph.hpp"
#include "../include-shared/constants.hpp"
#include "../include/qcircuit.hpp"

#include <iostream>

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
    printf("there are %lu roots\n", circuit.getroots()->size());
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
            printf("printing gate of type %i, with qbits: ", g->type);
            for (int i : g->Qbits()) {
                std::cout << ' ' << i;
            }
            std::cout << '\n';
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

std::vector<std::vector<std::pair<int, int>>> circuit_graph::MinimizeHelper(int qbits, std::vector<std::vector<std::pair<int, int>>> reductions) {
    printf("can reduce to %lu qubits. number of ways is %lu\n", qbits - reductions[0].size(), reductions.size());
    qcircuit circuit = qcircuit::clusterState(qbits);
    std::vector<std::vector<std::pair<int, int>>> newReductions;
    int currentReduction = qbits - reductions[0].size();
    printf("hhh: %i\n", currentReduction);
    bool reachedBound;
    for(auto vec : reductions) {
        reachedBound = false;
        for(int i = 0; i < currentReduction; i++) {
            for(int j = 0; j < currentReduction; j++) {
                circuit = qcircuit::clusterState(qbits);
                for(std::pair<int, int> elem : vec) {
                    circuit.Reuse(elem.first, elem.second);
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