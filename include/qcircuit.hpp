#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include-shared/gate.hpp"

class qcircuit {
public:
    qcircuit(int rootnum);
    static qcircuit mod3n(int n);
    static qcircuit clusterState(int qbits);
    static qcircuit labeledClusterState(int n);
    bool Reuse(int from, int to);
    void SafeReuse(int from, int to);
    bool ReuseAndAdd(int from, int to, int spacers);
    void Spacer(int qbit, int count);
    void H(int qbit);
    void CNOT(int qbit1, int qbit2);
    void CZ(int qbit1, int bqit2);
    void Reset(int qbit);
    void LabeledMeasure(int qbit);
    void LabeledGate(int qbit, std::string label);
    void Measure(int qbit);
    bool hasCycle();
    Gate* EndOfExecution(int qbit);
    void OneQubitGate(int qbit, GateType type);
    void CustomLabelOneQubitGate(int qbit, GateType type, std::string label);
    void TwoQubitGate(int qbit1, int qbit2, GateType type);
    int Qbits();
    int LogicalQbits();
    std::vector<Gate*>* getroots();
    void setroots(std::vector<Gate*>* r);
    std::set<int> CausalCone(int qbit);
    void ReIndexQubit(int original);
    std::vector<std::vector<bool>> CircuitCausalCone();
    int CircuitDepth();
private:
    std::vector<Gate*> roots;
    int qbits;
    int logicalQubits;
};