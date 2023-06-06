#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <optional>

// ================================================
// GATE TYPES
// ================================================

enum GateType {
  ResetGate = 1,
  MeasureGate = 2,
  HadamardGate = 3,
  CNOTGate = 4,
  CZGate = 5,

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
  std::set<int> UpstreamQbits();
  std::vector<int> Qbits();
};