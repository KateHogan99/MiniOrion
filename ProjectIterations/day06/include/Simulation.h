#ifndef SIMULATION_H
#define SIMULATION_H

#include "Integrator.h"
#include "State.h"
#include "Vector2D.h"

#include <vector>

struct SimulationConfig
{
    double mu;

    Vector2D initial_position;
    Vector2D initial_velocity;
    Vector2D thrust_acceleration;

    double time_step;
    int number_of_steps;

    Integrator integrator;
};

struct SimulationStep
{
    double time;

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

double calculate_displacement(Vector2D initial_position, Vector2D final_position);

SimulationStep calculate_simulation_step(State state, State previous_state, double time, double total_distance, SimulationConfig config);

State propagate_state(State spacecraft, SimulationConfig config);

std::vector<SimulationStep> run_simulation(SimulationConfig config);

#endif