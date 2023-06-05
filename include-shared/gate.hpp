#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// ================================================
// GATE TYPES
// ================================================

enum GateType {
  ResetGate = 1,
  MeasureGate = 2,
  HadamardGate = 3,
  CNOTGate = 4,
};

// ================================================
// GATE STRUCT
// ================================================

struct Edge {
  std::optional<Gate*> from;
  int qbit;
  std::optional<Gate*> to;
};

struct Gate {
  GateType type;
  std::vector<Edge> edges;
  Edge* findEdge(int qbit);
};