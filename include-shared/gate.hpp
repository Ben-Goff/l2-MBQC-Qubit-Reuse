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
  Blank = 0,
  ResetGate = 1,
  MeasureGate = 2,
  HadamardGate = 3,
  CNOTGate = 4,
  CZGate = 5,
};

// ================================================
// GATE STRUCT
// ================================================


struct Gate {
  GateType type;
  std::vector<std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>> edges;
  std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* findEdge(int qbit);
  std::set<int> UpstreamQbits();
  std::vector<int> Qbits();
  bool isEnd(int qbit);
};