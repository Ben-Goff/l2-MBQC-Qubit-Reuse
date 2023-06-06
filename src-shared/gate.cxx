#include "../include-shared/gate.hpp"

std::tuple<std::optional<Gate*>, int, std::optional<Gate*>> Gate::findEdge(int qbit) {
    printf("hello2 looking for %d. there are %lu edges\n", qbit, this->edges.size());
    for(auto e : this->edges) {
        printf("i found %d\n", std::get<1>(e));
        if(std::get<1>(e) == qbit) {
            return e;
        }
    }
    throw "can't be found";
}

std::vector<int> Gate::Qbits() {
    std::vector<int> qbits;
    for(auto e : this->edges) {
        qbits.push_back(std::get<1>(e));
    }
    return qbits;
}

std::set<int> Gate::UpstreamQbits() {
    std::set<int> qbits;
    for(auto e : this->edges) {
        qbits.insert(std::get<1>(e));
        if(std::get<2>(e).has_value()) {
            qbits.merge(std::get<2>(e).value()->UpstreamQbits());
        }
    }
    return qbits;
}