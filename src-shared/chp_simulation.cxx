#include "../include-shared/chp_simulation.hpp"
#include "../include-shared/constants.hpp"
#include "../include/qcircuit.hpp"
extern "C" {
  #include "../include/chp.h"
  #include <time.h>
}

bool finishedDrawing(std::vector<std::optional<std::tuple<Gate*, bool>>> currentLayer) {
    for(auto o : currentLayer) {
        if(o.has_value()) {return false;}
    }
    return true;
}

std::vector<int> chp_simulation::simulate(qcircuit c) {
    //set the number of qubits
    QState* q;
    q = (QState*)malloc(sizeof(struct QState));
    initstae_(q,c.Qbits(),NULL);
    printf("%i qubits\n", c.Qbits());

    std::vector<Gate*> roots = *(c.getroots());

    //holds the measured values
    std::vector<int> measurements(c.LogicalQbits());

    //at an index i, currentLayer holds
    //1) A pointer to the next Gate to be printed for qbit i
    //2) A boolean stating if the Gate is ready to be updated. True means 1) is a printing of 2), false means 1) is a printing of a gate that points to 2)
    int layer = 0;
    std::vector<std::optional<std::tuple<Gate*, bool>>> currentLayer(c.getroots()->size());
    for(int i = 0; i < currentLayer.size(); i++) {
        currentLayer[i] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(roots[i], true)};
    }
    layer++;
    while(!finishedDrawing(currentLayer)) {
        //update level of gates to be drawn
        for(int i = 0; i < currentLayer.size(); i++) {
            //if the gate's node has been printed
            if(currentLayer[i].has_value() && std::get<1>(currentLayer[i].value())) {
                //if it was the last gate for this qubit, remove entry from currentLayer
                if(std::get<0>(currentLayer[i].value())->isEnd(i)) {
                    currentLayer[i] = std::nullopt;
                //else update the gate by traversing the gate to the next gate for that qubit
                } else {
                Gate* newGate = std::get<2>(*std::get<0>(currentLayer[i].value())->findEdge(i)).value();
                currentLayer[i] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(newGate, false)};
                }
            }
        }
        //draw gates that have every qubit ready to be printed
        std::set<Gate*> currentGates;
        for(auto i : currentLayer) {
            if(i.has_value()) {
                currentGates.insert(std::get<0>(i.value()));
            }
        }
        std::set<Gate*> printableGates;
        for(auto g : currentGates) {
            bool printable = true;
            //for each edge in every gate which can be printed....
            for(std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e : g->edges) {
                int* qbit = std::get<1>(e);
                //....we check if the qbit at that edge has printed every gate up to this gate and is ready to print
                if(std::get<1>(currentLayer[*qbit].value()) || std::get<0>(currentLayer[*qbit].value()) != g) {
                    printable = false;
                }
            }
            if(printable) {
                printableGates.insert(g);
            }
        }
        for(auto g : printableGates) {
            switch(g->type) {
                //first all of the one-qubit gates. This will prevent two-qubit gates from visually running over one-qubit gate
                case Blank:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    printf("blank gate on qbit %i\n", qbit);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    break;
                }
                case ResetGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    printf("reset gate on qbit %i\n", qbit);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    break;
                }
                case MeasureGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    printf("measure gate on qbit %i\n", g->measureQbit.value());
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    measurements[g->measureQbit.value()] = measure(q, qbit, 0);
                    break;
                }
                case HadamardGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    printf("h gate on qbit %i\n", qbit);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    hadamard(q, qbit);
                    break;
                }
                case CNOTGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    printf("cnot gate on qbits %i and %i\n", qbit1, qbit2);
                    currentLayer[qbit1] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit1].value()), true)};
                    currentLayer[qbit2] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit2].value()), true)};
                    cnot(q, qbit1, qbit2);
                    break;
                }
                case CZGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    printf("cz gate on qbits %i and %i\n", qbit1, qbit2);
                    currentLayer[qbit1] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit1].value()), true)};
                    currentLayer[qbit2] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit2].value()), true)};
                    cz(q, qbit1, qbit2);
                    break;
                }
                default: printf("Unknown Gate Type\n");
            }
        }
        //update how many layers have been printed so far
        layer++;
    }
    free(q);
    return measurements;
}


void chp_simulation::cz(QState* q, int qbit1, int qbit2) {
    hadamard(q, qbit2);
    cnot(q, qbit1, qbit2);
    hadamard(q, qbit2);
}

bool chp_simulation::equivalent(qcircuit c1, qcircuit c2) {
    if(c1.LogicalQbits() != c2.LogicalQbits()) {
        printf("The two circuits had different numbers of logical qubits\n");
        return false;
    }
    std::vector<int> m1 = simulate(c1);
    std::vector<int> m2 = simulate(c2);
    for(int i = 0; i < m1.size(); i++) {
        if((m1[i] < 2) && (m1[i] != m2[i])) {
            printf("Circuit 1 (deterministically) measured logical qubit %i as %i, but circuit 2 (deterministically) measured it as %i\n", i, m1[i], m2[i]);
            return false;
        }
        if((m1[i] >= 2) && (m2[i] < 2)) {
            printf("Circuit 1 measured logical qubit %i randomly, but circuit 2 measured it deterministically\n", i);
            return false;
        }
        if((m2[i] >= 2) && (m1[i] < 2)) {
            printf("Circuit 2 measured logical qubit %i randomly, but circuit 1 measured it deterministically\n", i);
            return false;
        }
    }
    return true;
}