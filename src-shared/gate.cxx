#include "../include-shared/gate.hpp"

Edge Gate::findEdge(int qbit) {
    for(Edge e : this->edges) {
        if(e.qbit == qbit) {
            return e;
        }
    }
}