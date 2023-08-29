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
  LabelGate = 6,
};

// ================================================
// GATE STRUCT
// ================================================


struct Gate {
  GateType type;
  std::optional<std::string> customLabel = std::nullopt;
  //contains the logical qbit being measured if this is a measure gate
  std::optional<int> measureQbit;
  std::vector<std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>> edges;
  std::tuple<std::optional<Gate*>, int*, std::optional<Gate*>>* findEdge(int qbit);
  std::set<int> UpstreamQbits();
  bool Dependency(int n);
  std::vector<int> Qbits();
  bool isEnd(int qbit);
};