#include "../include-shared/gate.hpp"

Edge* Gate::findEdge(int qbit) {
    for(Edge e : this->edges) {
        if(e.qbit == qbit) {
            return &e;
        }
    }
}

std::vector<int> Gate::Qbits() {
    std::vector<int> qbits;
    for(Edge e : this->edges) {
        qbits.push_back(e.qbit);
    }
}

std::set<int> Gate::UpstreamQbits() {
    std::set<int> qbits;
    for(Edge e : this->edges) {
        qbits.insert(e.qbit);
        if(e.from.has_value()) {
            qbits.merge(e.from.value()->UpstreamQbits());
        }
    }
    return qbits;
}