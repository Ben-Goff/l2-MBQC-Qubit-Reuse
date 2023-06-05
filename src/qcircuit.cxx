#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"

qcircuit::qcircuit(int roots) {
    std::vector<Gate> r(roots);
    for(int i = 0; i < roots; i++) {
        Gate curr;
        curr.type = ResetGate;
        Edge e;
        e.from = std::nullopt;
        e.qbit = i;
        e.to = std::nullopt;
        r.push_back(curr);
    }
    this->roots = r;
}

void qcircuit::Reset(int qbit) {
    Gate end = this->EndOfExecution(qbit);
    Gate g;
    g.type = ResetGate;
    Edge added;
    added.from = end;
    added.to = g;
    added.qbit = qbit;
    Edge added;
    added.from = 

}


