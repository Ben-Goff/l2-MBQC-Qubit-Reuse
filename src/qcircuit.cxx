#include <optional>

#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"

qcircuit::qcircuit(int rootnum) {
    std::vector<Gate*>* r = new std::vector<Gate*>;
    for(int i = 0; i < rootnum; i++) {
        Gate* curr = new Gate;
        curr->type = ResetGate;
        int* gate_int = new int;
        *gate_int = i;
        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = std::make_tuple(std::nullopt, gate_int, std::nullopt);
        curr->edges = {e};
        r->push_back(curr);
    }
    this->roots = *r;
    this->qbits = rootnum;
}

std::vector<Gate*>* qcircuit::getroots() {
    return &(this->roots);
}

void qcircuit::setroots(std::vector<Gate*>* r) {
    this->roots = *r;
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

void qcircuit::Spacer(int qbit, int count) {
    for(int i = 0; i < count; i++) {
        OneQubitGate(qbit, Blank);
    }
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
    //create new gate
    Gate* g = new Gate;
    //set gate type
    g->type = type;
    //make pointer for qbit int
    int* gate_int = new int;
    *gate_int = qbit;
    //make edge for gate
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> added = std::make_tuple(std::optional<Gate*>{end}, gate_int, std::nullopt);
    //add edge to gate
    g->edges = {added};
    //make the new gate what the previously last gate now points to
    std::get<2>(*(end->findEdge(qbit))).emplace(g);
}

void qcircuit::TwoQubitGate(int qbit1, int qbit2, GateType type) {
    //find the last gate on the two qbits
    Gate* end1 = this->EndOfExecution(qbit1);
    Gate* end2 = this->EndOfExecution(qbit2);
    //create new gate
    Gate* g = new Gate;
    //set gate type
    g->type = type;
    //make pointers for qbit ints
    int* gate_int1 = new int;
    *gate_int1 = qbit1;
    int* gate_int2 = new int;
    *gate_int2 = qbit2;
    //make edge for gate for qbit1
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> added1 = std::make_tuple(std::optional<Gate*>{end1}, gate_int1, std::nullopt);
    //make edge for gate for qbit2
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> added2 = std::make_tuple(std::optional<Gate*>{end2}, gate_int2, std::nullopt);
    //add edge to gate
    g->edges = {added1, added2};
    //make the new gate what the previously last gate now points to
    std::get<2>(*(end1->findEdge(qbit1))).emplace(g);
    std::get<2>(*(end2->findEdge(qbit2))).emplace(g);
}

Gate* qcircuit::EndOfExecution(int qbit) {
    Gate* curr = this->roots[qbit];

    while(std::get<2>(*(curr->findEdge(qbit))).has_value()) {
        curr = std::get<2>(*(curr->findEdge(qbit))).value();
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
        printf("qbit %i doesn't currently end execution with a measure gate. it ends with %i\n", from, end->type);
        return false;
    }
    std::set<int> dependencies = end->UpstreamQbits();
    if(dependencies.find(to) != dependencies.end()) {
        printf("qbit %i is dependent on qbit %i, cannot reuse\n", from, to);
        return false;
    } else {
        std::get<2>(*(end->findEdge(from))).emplace(this->roots[to]);
        std::get<0>(*(this->roots[to]->findEdge(to))).emplace(end);
        std::vector<Gate*>* curr = this->getroots();
        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* edgeToUpdate = (*this->getroots())[to]->findEdge(to);
        *std::get<1>(*edgeToUpdate) = from;
        while(std::get<2>(*edgeToUpdate).has_value()) {
            edgeToUpdate = std::get<2>(*edgeToUpdate).value()->findEdge(to);
            *std::get<1>(*edgeToUpdate) = from;
        }
        curr->erase(curr->begin() + to);
        this->setroots(curr);
        this->qbits = this->qbits - 1;
        return true;
    }
}


