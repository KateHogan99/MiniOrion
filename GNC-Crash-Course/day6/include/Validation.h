#ifndef VALIDATION_H
#define VALIDATION_H

#include "Simulation.h"

#include <vector>

State calculate_analytical_state(State initial_state, Vector2D acceleration, double time);

void validate_constant_acceleration(const std::vector<SimulationStep>& results, SimulationConfig config);

void validate_simulation(const std::vector<SimulationStep>& results, SimulationConfig config);

#endif