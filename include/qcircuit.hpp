#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include-shared/gate.hpp"

class qcircuit {
public:
    qcircuit(int rootnum);
    bool Reuse(int from, int to);
    void H(int qbit);
    void CNOT(int qbit1, int qbit2);
    void CZ(int qbit1, int bqit2);
    void Reset(int qbit);
    void Measure(int qbit);
    bool hasCycle();
    Gate* EndOfExecution(int qbit);
    void OneQubitGate(int qbit, GateType type);
    void TwoQubitGate(int qbit1, int qbit2, GateType type);
    int Qbits();
    std::vector<Gate*>* getroots();
    void setroots(std::vector<Gate*>* r);
private:
    std::vector<Gate*> roots;
    int qbits;
};