#ifndef OUTPUT_H
#define OUTPUT_H

#include "Simulation.h"

#include <string>
#include <vector>

std::string integrator_name(Integrator integrator);

void print_results_table(const std::vector<SimulationStep>& results);

void print_summary(const std::vector<SimulationStep>& results, SimulationConfig config);

#endif