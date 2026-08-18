#ifndef SIMULATION_H
#define SIMULATION_H

#include "Integrator.h"
#include "State.h"
#include "Vector2D.h"

#include <vector>

struct SimulationConfig
{
    double mu; // Gravitational constant

    Vector2D initial_position;
    Vector2D initial_velocity;
    Vector2D thrust_acceleration;

    double time_step; // Unit: seconds
    int number_of_steps;

    Integrator integrator;
};

struct SimulationStep
{
    double time; // Unit: seconds

    State state;

    Vector2D gravity_acceleration;
    Vector2D thrust_acceleration;
    Vector2D total_acceleration;

    double radius;
    double speed;
    double orbital_energy;
    double angular_momentum;

    double distance_this_step;
    double total_distance;
};

double calculate_displacement(
    Vector2D initial_position,
    Vector2D final_position
);

SimulationStep calculate_simulation_step(
    State state,
    double time,
    double distance_this_step,
    double total_distance,
    SimulationConfig config
);

State propagate_state(
    State state,
    SimulationConfig config
);

std::vector<SimulationStep> run_simulation(
    SimulationConfig config
);

#endif