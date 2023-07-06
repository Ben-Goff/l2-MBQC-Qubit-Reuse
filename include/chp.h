#ifndef CHP_H
#define CHP_H

struct QProg

// Quantum circuit

{

	long n;         // # of qubits
	long T;         // # of gates
	char *a; // Instruction opcode
	long *b; // Qubit 1
	long *c; // Qubit 2 (target for CNOT)
	int DISPQSTATE; // whether to print the state (q for final state only, Q for every iteration)
	int DISPTIME; // whether to print the execution time
	int SILENT;         // whether NOT to print measurement results
	int	DISPPROG; // whether to print instructions being executed as they're executed
	int SUPPRESSM; // whether to suppress actual computation of determinate measurement results

};



struct QState

// Quantum state

{

	// To save memory and increase speed, the bits are packed 32 to an unsigned long
	long n;         // # of qubits
	unsigned long **x; // (2n+1)*n matrix for stabilizer/destabilizer x bits (there's one "scratch row" at
	unsigned long **z; // (2n+1)*n matrix for z bits                                                 the bottom)
	int *r;         // Phase bits: 0 for +1, 1 for i, 2 for -1, 3 for -i.  Normally either 0 or 2.
	unsigned long pw[32]; // pw[i] = 2^i
	long over32; // floor(n/8)+1

};

void error(int k);



void cnot(struct QState *q, long b, long c);



void hadamard(struct QState *q, long b);



void phase(struct QState *q, long b);



void rowcopy(struct QState *q, long i, long k);



void rowswap(struct QState *q, long i, long k);



void rowset(struct QState *q, long i, long b);



int clifford(struct QState *q, long i, long k);



void rowmult(struct QState *q, long i, long k);



void printstate(struct QState *q);



int measure(struct QState *q, long b, int sup);



long gaussian(struct QState *q);



long innerprod(struct QState *q1, struct QState *q2);



void printbasisstate(struct QState *q);



void seed(struct QState *q, long g);



void printket(struct QState *q);



void runprog(struct QProg *h, struct QState *q);



void preparestate(struct QState *q, char *s);



void initstae_(struct QState *q, long n, char *s);



void readprog(struct QProg *h, char *fn, char *params);

#endif 