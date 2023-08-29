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
    this->logicalQubits = rootnum;
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

int qcircuit::LogicalQbits() {
    return this->logicalQubits;
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

void qcircuit::LabeledMeasure(int qbit) {
    CustomLabelOneQubitGate(qbit, MeasureGate, "M" + std::to_string(qbit + 1));
}

void qcircuit::LabeledGate(int qbit, std::string label) {
    CustomLabelOneQubitGate(qbit, LabelGate, label);
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
    //set the measure logical qbit if it is a measure gate
    g->measureQbit = ((type == MeasureGate) ? (std::optional<int>{qbit}) : (std::nullopt));
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

void qcircuit::CustomLabelOneQubitGate(int qbit, GateType type, std::string label) {

    //find the last gate on qbit
    Gate* end = this->EndOfExecution(qbit);
    //create new gate
    Gate* g = new Gate;
    //set gate type
    g->type = type;
    //set the measure logical qbit if it is a measure gate
    g->measureQbit = ((type == MeasureGate) ? (std::optional<int>{qbit}) : (std::nullopt));
    //make pointer for qbit int
    int* gate_int = new int;
    *gate_int = qbit;
    //make edge for gate
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> added = std::make_tuple(std::optional<Gate*>{end}, gate_int, std::nullopt);
    //add edge to gate
    g->edges = {added};
    //make the new gate what the previously last gate now points to
    std::get<2>(*(end->findEdge(qbit))).emplace(g);
    //set the label
    g->customLabel = std::optional<std::string>{label};
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

void qcircuit::SafeReuse(int from, int to) {
    Gate* end = this->EndOfExecution(from);
    //we have the last measure gate point to the new qbit
    std::get<2>(*(end->findEdge(from))).emplace(this->roots[to]);
    //we have the first gate of the to qbit point from the from qbit
    std::get<0>(*(this->roots[to]->findEdge(to))).emplace(end);
    std::vector<Gate*>* curr = this->getroots();
    //find the root edge of the to qbit
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* edgeToUpdate = (*this->getroots())[to]->findEdge(to);
    //chame its qbit to the from qbit
    *std::get<1>(*edgeToUpdate) = from;
    while(std::get<2>(*edgeToUpdate).has_value()) {
        edgeToUpdate = std::get<2>(*edgeToUpdate).value()->findEdge(to);
        *std::get<1>(*edgeToUpdate) = from;
    }
    //and then we need to shift all higher number qbits down by one. the "to" index is now not used, so we start by filling in that gap and everything else goes down by one
    for(int i = to + 1; i < this->qbits; i++) {
        this->ReIndexQubit(i);
    }
    //the physical qbit that is now made available by reuse can be removed from the list of roots
    curr->erase(curr->begin() + to);
    this->setroots(curr);
    //and the number of physical qubits goes down by one accordingly
    this->qbits = this->qbits - 1;
}

bool qcircuit::Reuse(int from, int to) {
    if(to == from) {
        //printf("to and from are both %i\n", from);
        return false;
    }
    if(from > (this->qbits - 1) || from < 0) {
        //printf("qbit %i is outside the range of the circuit\n", from);
        return false;
    }
    if(to > (this->qbits - 1) || to < 0) {
        //printf("qbit %i is outside the range of the circuit\n", to);
        return false;
    }
    Gate* end = this->EndOfExecution(from);
    if(end->type != MeasureGate) {
        //printf("qbit %i doesn't currently end execution with a measure gate. it ends with %i\n", from, end->type);
        return false;
    }
    std::set<int> dependencies = end->UpstreamQbits();
    if(dependencies.find(to) != dependencies.end()) {
        //printf("qbit %i is dependent on qbit %i, cannot reuse\n", from, to);
        return false;
    } 
    // Gate* endTo = this->EndOfExecution(to);
    // std::set<int> dependenciesTo = endTo->UpstreamQbits();
    // if(dependenciesTo.find(from) != dependenciesTo.end()) {
    //     //printf("qbit %i is dependent on qbit %i, cannot reuse\n", to, from);
    //     return false;
    // } 
    else {
        //we have the last measure gate point to the new qbit
        std::get<2>(*(end->findEdge(from))).emplace(this->roots[to]);
        //we have the first gate of the to qbit point from the from qbit
        std::get<0>(*(this->roots[to]->findEdge(to))).emplace(end);
        std::vector<Gate*>* curr = this->getroots();
        //find the root edge of the to qbit
        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* edgeToUpdate = (*this->getroots())[to]->findEdge(to);
        //chame its qbit to the from qbit
        *std::get<1>(*edgeToUpdate) = from;
        while(std::get<2>(*edgeToUpdate).has_value()) {
            edgeToUpdate = std::get<2>(*edgeToUpdate).value()->findEdge(to);
            *std::get<1>(*edgeToUpdate) = from;
        }
        //and then we need to shift all higher number qbits down by one. the "to" index is now not used, so we start by filling in that gap and everything else goes down by one
        for(int i = to + 1; i < this->qbits; i++) {
            this->ReIndexQubit(i);
        }
        //the physical qbit that is now made available by reuse can be removed from the list of roots
        curr->erase(curr->begin() + to);
        this->setroots(curr);
        //and the number of physical qubits goes down by one accordingly
        this->qbits = this->qbits - 1;
        return true;
    }
}

bool qcircuit::ReuseAndAdd(int from, int to, int spacers) {
    if(to == from) {
        //printf("to and from are both %i\n", from);
        return false;
    }
    if(from > (this->qbits - 1) || from < 0) {
        //printf("qbit %i is outside the range of the circuit\n", from);
        return false;
    }
    if(to > (this->qbits - 1) || to < 0) {
        //printf("qbit %i is outside the range of the circuit\n", to);
        return false;
    }
    Gate* end = this->EndOfExecution(from);
    if(end->type != MeasureGate) {
        //printf("qbit %i doesn't currently end execution with a measure gate. it ends with %i\n", from, end->type);
        return false;
    }
    std::set<int> dependencies = end->UpstreamQbits();
    if(dependencies.find(to) != dependencies.end()) {
        //printf("qbit %i is dependent on qbit %i, cannot reuse\n", from, to);
        return false;
    // } 
    // Gate* endTo = this->EndOfExecution(to);
    // std::set<int> dependenciesTo = endTo->UpstreamQbits();
    // if(dependenciesTo.find(from) != dependenciesTo.end()) {
    //     //printf("qbit %i is dependent on qbit %i, cannot reuse\n", to, from);
    //     return false;
    } else {
        this->Spacer(from, 10);
        end = this->EndOfExecution(from);
        //we have the last measure gate point to the new qbit
        std::get<2>(*(end->findEdge(from))).emplace(this->roots[to]);
        //we have the first gate of the to qbit point from the from qbit
        std::get<0>(*(this->roots[to]->findEdge(to))).emplace(end);
        std::vector<Gate*>* curr = this->getroots();
        //find the root edge of the to qbit
        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* edgeToUpdate = (*this->getroots())[to]->findEdge(to);
        //chame its qbit to the from qbit
        *std::get<1>(*edgeToUpdate) = from;
        while(std::get<2>(*edgeToUpdate).has_value()) {
            edgeToUpdate = std::get<2>(*edgeToUpdate).value()->findEdge(to);
            *std::get<1>(*edgeToUpdate) = from;
        }
        //and then we need to shift all higher number qbits down by one. the "to" index is now not used, so we start by filling in that gap and everything else goes down by one
        for(int i = to + 1; i < this->qbits; i++) {
            this->ReIndexQubit(i);
        }
        //the physical qbit that is now made available by reuse can be removed from the list of roots
        curr->erase(curr->begin() + to);
        this->setroots(curr);
        //and the number of physical qubits goes down by one accordingly
        this->qbits = this->qbits - 1;
        return true;
    }
}

void qcircuit::ReIndexQubit(int original) {
    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* edgeToUpdate = (*this->getroots())[original]->findEdge(original);
    *std::get<1>(*edgeToUpdate) = original - 1;
    while(std::get<2>(*edgeToUpdate).has_value()) {
        edgeToUpdate = std::get<2>(*edgeToUpdate).value()->findEdge(original);
        *std::get<1>(*edgeToUpdate) = original - 1;
    }
}

std::set<int> qcircuit::CausalCone(int qbit) {
    std::set<int> ret = EndOfExecution(qbit)->UpstreamQbits();
    ret.insert(qbit);
    return ret;
}

qcircuit qcircuit::mod3n(int n) {
    qcircuit ret(4*n+5);
    for(int i = 0; i < 4*n+5; i++) {
        ret.H(i);
    }

    for(int i = 1; i < 4*n+5-1; i+=2) {
        ret.CZ(i, i+1);
    }

    for(int i = 0; i < 4*n+5-1; i+=2) {
        ret.CZ(i, i+1);
    }

    // ret.Spacer(1, 4);
    // ret.Spacer(2, 1);
    // ret.Spacer(3, 5);
    // ret.Spacer(4, 1);
    // ret.Spacer(5, 6);
    // ret.Spacer(6, 10);
    // ret.Spacer(7, 14);
    // ret.Spacer(8, 11);
    // ret.Spacer(9, 15);
    // ret.Spacer(10, 11);
    // ret.Spacer(11, 16);
    // ret.Spacer(12, 20);

    ret.LabeledGate(0, "α");

    for(int i = 1; i <= 2*n+1; i+=2) {
        ret.H(i - 1);
        ret.LabeledMeasure(i - 1);
    }

    for(int i = 2; i <= 2*n; i+=2) {
        ret.LabeledGate(i - 1, "π");
        ret.H(i - 1);
        ret.LabeledMeasure(i - 1);
    }

    ret.LabeledGate(2*n+2 - 1, "π");
    ret.H(2*n+2 - 1);
    ret.LabeledMeasure(2*n+2 - 1);

    ret.LabeledGate(2*n+3 - 1, "2α");

    for(int i = 2*n+3; i <= 4*n + 3; i+=2) {
        ret.H(i - 1);
        ret.LabeledMeasure(i - 1);
    }

    for(int i = 2*n+4; i <= 4*n + 2; i+=2) {
        ret.LabeledGate(i - 1, "π");
        ret.H(i - 1);
        ret.LabeledMeasure(i - 1);
    }

    ret.LabeledGate(4*n+4 - 1, "π");
    ret.H(4*n+4 - 1);
    ret.LabeledMeasure(4*n+4 - 1);

    ret.LabeledGate(4*n+5 -1 , "α");
    ret.H(4*n+5 - 1);
    ret.LabeledMeasure(4*n+5 - 1);

    return ret;
}

qcircuit qcircuit::clusterState(int n) {
    qcircuit circuit(n);
    for(int i = 0; i < n; i++) {
        circuit.H(i);
    }

    for(int i = 1; i < n-1; i+=2) {
        circuit.CZ(i, i+1);
    }

    for(int i = 0; i < n-1; i+=2) {
        circuit.CZ(i, i+1);
    }

    for(int i = 0; i < n; i++) {
        circuit.Measure(i);
    }
    return circuit;
}

qcircuit qcircuit::labeledClusterState(int n) {
    qcircuit circuit(n);
    for(int i = 0; i < n; i++) {
        circuit.H(i);
    }

    for(int i = 1; i < n-1; i+=2) {
        circuit.CZ(i, i+1);
    }

    for(int i = 0; i < n-1; i+=2) {
        circuit.CZ(i, i+1);
    }

    for(int i = 0; i < n; i++) {
        circuit.LabeledMeasure(i);
    }
    return circuit;
}

std::vector<std::vector<bool>> qcircuit::CircuitCausalCone() {
    std::vector<std::vector<bool>> ret(this->qbits);
    for(int i = 0; i < this->qbits; i++) {
        ret[i].resize(this->qbits);
        std::set<int> s = this->CausalCone(i);
        for(int j = 0; j < this->qbits; j++) {
            ret[i][j] = (s.count(j) == 1);
        }
    }
    return ret;
}

//returns the depth of the circuit
//it does assume that two two-qubit gates acting on qubits 1 and 3, and 2 and 4, can be implemented simultaneously
int qcircuit::CircuitDepth() {

    std::vector<Gate*> roots = *(this->getroots());

    //at an index i, currentLayer holds
    //1) A pointer to the next Gate to be printed for qbit i
    //2) A boolean stating if the Gate is ready to be updated. True means 1) is a printing of 2), false means 1) is a printing of a gate that points to 2)
    int layer = 0;
    std::vector<std::optional<std::tuple<Gate*, bool>>> currentLayer(this->getroots()->size());
    for(int i = 0; i < currentLayer.size(); i++) {
        currentLayer[i] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(roots[i], true)};
    }
    bool finished = true;
    for(auto o : currentLayer) {
        if(o.has_value()) {finished = false;}
    }
    while(!finished) {
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
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    break;
                }
                case ResetGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    //reset(q, qbit);
                    break;
                }
                case MeasureGate:
                {
                        std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                        int qbit = *std::get<1>(e);
                        currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                        break;
                }
                case LabelGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    break;
                }
                case HadamardGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e = g->edges[0];
                    int qbit = *std::get<1>(e);
                    currentLayer[qbit] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit].value()), true)};
                    break;
                }
                case CNOTGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    currentLayer[qbit1] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit1].value()), true)};
                    currentLayer[qbit2] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit2].value()), true)};
                    break;
                }
                case CZGate:
                {
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e1 = g->edges[0];
                    std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>> e2 = g->edges[1];
                    int qbit1 = *std::get<1>(e1);
                    int qbit2 = *std::get<1>(e2);
                    currentLayer[qbit1] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit1].value()), true)};
                    currentLayer[qbit2] = std::optional<std::tuple<Gate*, bool>>{std::make_tuple(std::get<0>(currentLayer[qbit2].value()), true)};
                    break;
                }
                default: printf("Unknown Gate Type\n");
            }
        }
        //update how many layers have been printed so far
        layer++;
        finished = true;
        for(auto o : currentLayer) {
            if(o.has_value()) {finished = false;}
        }
    }
    return layer;
}


