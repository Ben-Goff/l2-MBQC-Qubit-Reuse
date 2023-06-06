#include <optional>

#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"

qcircuit::qcircuit(int rootnum) {
    printf("hello5");
    std::vector<Gate> r(rootnum);
    for(int i = 0; i < rootnum; i++) {
        Gate curr;
        curr.type = ResetGate;
        std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> e = std::make_tuple(std::nullopt, i, std::nullopt);
        curr.edges = {e};
        r[i] = curr;
    }
    this->roots = r;
    this->qbits = rootnum;
}

int qcircuit::Qbits() {
    return this->qbits;
}

void qcircuit::Reset(int qbit) {
    OneQubitGate(qbit, ResetGate);
}

void qcircuit::H(int qbit) {
    OneQubitGate(qbit, HadamardGate);
}

void qcircuit::Measure(int qbit) {
    OneQubitGate(qbit, MeasureGate);
}

void qcircuit::CZ(int qbit1, int qbit2) {
    TwoQubitGate(qbit1, qbit2, CZGate);
}

void qcircuit::CNOT(int qbit1, int qbit2) {
    TwoQubitGate(qbit1, qbit2, CNOTGate);
}

void qcircuit::OneQubitGate(int qbit, GateType type) {
    //find the last gate on qbit
    Gate end = this->EndOfExecution(qbit);
    printf("h is initially this type: %d\n", end.type);
    //find the edge for qbit on that gate
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> toChange = end.findEdge(qbit);
    //create new gate
    Gate* g;
    //set gate type
    g->type = type;
    //make edge for gate
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> added = std::make_tuple(std::optional<Gate*>{&end}, qbit, std::nullopt);
    //add edge to gate
    g->edges = {added};
    //make the new gate what the previously last gate now points to
    std::get<2>(toChange) = std::optional<Gate*>{g};
}

void qcircuit::TwoQubitGate(int qbit1, int qbit2, GateType type) {
    //find the last gate on the two qbits
    Gate end1 = this->EndOfExecution(qbit1);
    Gate end2 = this->EndOfExecution(qbit2);
    //find the edge for the qbit on those gates
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> toChange1 = end1.findEdge(qbit1);
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> toChange2 = end2.findEdge(qbit2);
    //create new gate
    Gate* g;
    //set gate type
    g->type = type;
    //make edge for gate for qbit1
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> added1 = std::make_tuple(std::optional<Gate*>{&end1}, qbit1, std::nullopt);
    //make edge for gate for qbit2
    std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> added2 = std::make_tuple(std::optional<Gate*>{&end2}, qbit1, std::nullopt);
    //add edge to gate
    g->edges = {added1, added2};
    //make the new gate what the previously last gate now points to
    std::get<2>(toChange1) = std::optional<Gate*>{g};
    std::get<2>(toChange2) = std::optional<Gate*>{g};
}

Gate qcircuit::EndOfExecution(int qbit) {
    Gate curr = this->roots[qbit];
    while(std::get<2>(curr.findEdge(qbit)) != std::nullopt) {
        printf("type is: %d\n", curr.type);
        curr = *(std::get<2>(curr.findEdge(qbit)).value());
    }
    return curr;
}

bool qcircuit::Reuse(int from, int to) {
    if(from > (this->qbits - 1) || from < 0) {
        printf("qbit %i is outside the range of the circuit\n", from);
        return false;
    }
    if(to > (this->qbits - 1) || to < 0) {
        printf("qbit %i is outside the range of the circuit\n", to);
        return false;
    }
    Gate end = this->EndOfExecution(from);
    if(end.type != MeasureGate) {
        printf("qbit %i doesn't currently end execution with a measure gate\n", from);
        return false;
    }
    std::set<int> dependencies = end.UpstreamQbits();
    if(dependencies.find(to) == dependencies.end()) {
        printf("qbit %i is dependent on qbit %i, cannot reuse\n", from, to);
        return false;
    } else {
        std::get<2>(end.edges[0]) = std::optional<Gate*>{&(this->roots[to])};
        std::get<0>(this->roots[to].findEdge(to)) = std::optional<Gate*>{&end};
        this->roots.erase(this->roots.begin() + to);
        return true;
    }
}


