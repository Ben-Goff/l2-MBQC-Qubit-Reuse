#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include-shared/gate.hpp"

class qcircuit {
public:
    qcircuit(int rootnum);
    static qcircuit clusterState(int qbits);
    static qcircuit hadamardClusterState(int n);
    bool Reuse(int from, int to);
    void SafeReuse(int from, int to);
    void Spacer(int qbit, int count);
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
    int LogicalQbits();
    std::vector<Gate*>* getroots();
    void setroots(std::vector<Gate*>* r);
    std::set<int> CausalCone(int qbit);
    void ReIndexQubit(int original);
    std::vector<std::set<int>> CircuitCausalCone();
    int CircuitDepth();
private:
    std::vector<Gate*> roots;
    int qbits;
    int logicalQubits;
};