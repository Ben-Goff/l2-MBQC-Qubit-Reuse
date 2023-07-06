#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <set>
#include <optional>

#include "../include-shared/constants.hpp"
#include "../include/qcircuit.hpp"
#include "../include-shared/gate.hpp"
extern "C" {
  #include "../include/chp.h"
  #include <time.h>
}

struct chp_simulation {
    public:
        
        static std::vector<int> simulate(qcircuit c);
        
        static void cz(QState* q, int qbit1, int qbit2);

        static bool equivalent(qcircuit c1, qcircuit c2);
};