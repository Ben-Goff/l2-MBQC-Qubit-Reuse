#include "qcircuit.cxx"
#include "../src-shared/gate.cxx"

int main(int argc, char* argv[]) {
    static qcircuit simplecircuit(3);
    simplecircuit.H(0);
    simplecircuit.CNOT(0, 1);
    simplecircuit.Measure(0);
    simplecircuit.H(1);
    simplecircuit.CNOT(1, 2);
    simplecircuit.Measure(1);
    simplecircuit.Measure(2);
    bool works = simplecircuit.Reuse(0, 2);
    printf("%s, and the number of roots is %d\n", works ? "true" : "false", simplecircuit.Qbits());
    return 0;
}