#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include-shared/gate.hpp"

class qcircuit {
public:
    qcircuit(int roots);
    bool Reuse(int from, int to);
    void H(int qbit);
    void CNOT(int qbit1, int qbit2);
    void CZ(int qbit1, int bqit2);
    void Reset(int qbit);
    void Measure(int qbit);
    bool hasCycle();
    Gate* EndOfExecution(int qbit);
    void OneQubitGate(int qubit, GateType type);

private:
    std::vector<Gate*> roots;
};