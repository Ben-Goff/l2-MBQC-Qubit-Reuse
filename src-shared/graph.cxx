#include "../include-shared/graph.hpp"
#include "../include-shared/constants.hpp"

#include <iostream>

using namespace ogdf;

bool finishedDrawing(std::vector<std::optional<std::tuple<node, Gate*, bool>>> currentLayer) {
    for(auto o : currentLayer) {
        if(o.has_value()) {return false;}
    }
    return true;
}

void OutputCircuit(qcircuit circuit, std::string file) {
    //at an index i, currentLayer holds
    //1) The most recently printed node for qbit i
    //2) A pointer to the next Gate to be printed for qbit i
    //3) A boolean stating if the Gate is ready to be updated. True means 1) is a printing of 2), false means 1) is a printing of a gate that points to 2)
    int layer = 0;
    std::vector<std::optional<std::tuple<node, Gate*, bool>>> currentLayer(circuit.getroots()->size());
    std::vector<Gate*> roots = *(circuit.getroots());
    Graph G;
    GraphAttributes GA(G, GraphAttributes::all);
    for(int i = 0; i < currentLayer.size(); i++) {
        node r = G.newNode();
        GA.x(r) = layer*NODE_HORZ_SEP;
        GA.y(r) = i*NODE_VERT_SEP;
        GA.width(r) = NODE_WIDTH + 3;
        GA.height(r) = NODE_HEIGHT;
        GA.strokeType(r) = ogdf::StrokeType::None;
        GA.label(r) = "|0〉";
        GA.xLabel(r) = layer*NODE_HORZ_SEP + 3;
        currentLayer[i] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(r, roots[i], true)};
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
                node currNode = std::get<0>(currentLayer[i].value());
                currentLayer[i] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(currNode, newGate, false)};
                GateType gt = newGate->type;
                int gtt = gt;
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
                printf("the qbite is %i\n", *qbit);
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
                printedOneQubitGateQubits.insert(*std::get<1>(g->edges[0]));
                switch(g->type) {
                    //first all of the one-qubit gates. This will prevent two-qubit gates from visually running over one-qubit gate
                    case ResetGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.label(n) = "R";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    case MeasureGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.label(n) = "M";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        break;
                    }
                    case HadamardGate:
                    {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int* qbit = std::get<1>(e);
                        node n = G.newNode();
                        GA.x(n) = layer*NODE_HORZ_SEP;
                        GA.y(n) = (*qbit)*NODE_VERT_SEP;
                        GA.width(n) = NODE_WIDTH;
                        GA.height(n) = NODE_HEIGHT;
                        GA.fillPattern(n) = ogdf::FillPattern::None;
                        GA.label(n) = "H";
                        GA.yLabel(n) = NODE_LABEL_Y;
                        edge ej = G.newEdge(std::get<0>(currentLayer[*qbit].value()),n);
                        GA.arrowType(ej) = ogdf::EdgeArrow::None;
                        currentLayer[*qbit] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(n, std::get<1>(currentLayer[*qbit].value()), true)};
                        printf("DID AN H GATE");
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
                    node n1 = G.newNode();
                    node n2 = G.newNode();
                    GA.x(n1) = layer*NODE_HORZ_SEP;
                    GA.y(n1) = (qbit1)*NODE_VERT_SEP;
                    GA.x(n2) = layer*NODE_HORZ_SEP;
                    GA.y(n2) = (qbit2)*NODE_VERT_SEP;
                    GA.width(n1) = NODE_WIDTH/2;
                    GA.height(n1) = NODE_HEIGHT/2;
                    GA.shape(n1) = ogdf::Shape::Ellipse;
                    GA.fillColor(n1) = ogdf::Color::Name::Black;
                    edge ej1 = G.newEdge(std::get<0>(currentLayer[qbit1].value()),n1);
                    GA.arrowType(ej1) = ogdf::EdgeArrow::None;
                    currentLayer[qbit1] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(n1, std::get<1>(currentLayer[qbit1].value()), true)};
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
                    edge ej2 = G.newEdge(std::get<0>(currentLayer[qbit2].value()),n2);
                    GA.arrowType(ej2) = ogdf::EdgeArrow::None;
                    currentLayer[qbit2] = std::optional<std::tuple<node, Gate*, bool>>{std::make_tuple(n2, std::get<1>(currentLayer[qbit2].value()), true)};
                    edge ej3 = G.newEdge(n1, n2);
                    GA.arrowType(ej3) = ogdf::EdgeArrow::None;
                }
            }
        }
        //update how many layers have been printed so far
        layer++;
    }
    printf("there are %i ndes\n", G.numberOfNodes());
    printf("there are %i edges\n", G.numberOfEdges());

    //GraphIO::write(GA, file + ".gml", GraphIO::writeGML);
    GraphIO::write(GA, file + ".svg", GraphIO::drawSVG);

}