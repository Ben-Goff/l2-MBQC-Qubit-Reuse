#include <optional>

#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"

qcircuit::qcircuit(int roots) {
    std::vector<Gate*> r(roots);
    for(int i = 0; i < roots; i++) {
        Gate curr;
        curr.type = ResetGate;
        Edge e;
        e.from = std::nullopt;
        e.qbit = i;
        e.to = std::nullopt;
        r.push_back(&curr);
    }
    this->roots = r;
    this->qbits = roots;
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
    Gate* end = this->EndOfExecution(qbit);
    //find the edge for qbit on that gate
    Edge* toChange = end->findEdge(qbit);
    //create new gate
    Gate* g;
    //set gate type
    g->type = type;
    //make edge for gate
    Edge added;
    //set the egde to be coming from the previously found last gate on qbit
    added.from = std::optional<Gate*>{end};
    //the edge isn't going anywhere yet; we are creating the new last gate on qbit
    added.to = std::nullopt;
    //the edge is on the qbit's wire
    added.qbit = qbit;
    //add edge to gate
    g->edges = {added};
    //make the new gate what the previously last gate now points to
    toChange->to = g;
}

void qcircuit::TwoQubitGate(int qbit1, int qbit2, GateType type) {
    //find the last gate on the two qbits
    Gate* end1 = this->EndOfExecution(qbit1);
    Gate* end2 = this->EndOfExecution(qbit2);
    //find the edge for the qbit on those gates
    Edge* toChange1 = end1->findEdge(qbit1);
    Edge* toChange2 = end2->findEdge(qbit2);
    //create new gate
    Gate* g;
    //set gate type
    g->type = type;
    //make edge for gate for qbit1
    Edge added1;
    added1.from = std::optional<Gate*>{end1};
    added1.to = std::nullopt;
    added1.qbit = qbit1;
    //make edge for gate for qbit2
    Edge added2;
    added2.from = std::optional<Gate*>{end2};
    added2.to = std::nullopt;
    added2.qbit = qbit2;
    //add edge to gate
    g->edges = {added1, added2};
    //make the new gate what the previously last gate now points to
    toChange1->to = g;
    toChange2->to = g;
}

Gate* qcircuit::EndOfExecution(int qbit) {
    Gate* curr = this->roots[qbit];
    while(curr->findEdge(qbit)->to != std::nullopt) {
        curr = curr->findEdge(qbit)->to.value();
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
    Gate* end = this->EndOfExecution(from);
    if(end->type != MeasureGate) {
        printf("qbit %i doesn't currently end execution with a measure gate\n", from);
        return false;
    }
    std::set<int> dependencies = end->UpstreamQbits();
    if(dependencies.contains(to)) {
        printf("qbit %i is dependent on qbit %i, cannot reuse\n", from, to);
        return false;
    } else {
        end->edges[0].to = std::optional<Gate*>{this->roots[to]};
        this->roots[to]->findEdge(to)->from = std::optional<Gate*>{end};
        this->roots.erase(this->roots.begin() + to);
        return true;
    }
}


