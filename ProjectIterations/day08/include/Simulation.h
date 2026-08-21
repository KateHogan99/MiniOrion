#ifndef SIMULATION_H
#define SIMULATION_H

#include "Control.h"
#include "Guidance.h"
#include "Integrator.h"
#include "State.h"
#include "Vector2D.h"

#include <vector>

struct SimulationConfig
{
    double mu; // Gravitational parameter, unit: m^3/s^2

    Vector2D initial_position;
    Vector2D initial_velocity;
    Vector2D thrust_acceleration;

    double time_step; // Unit: seconds
    int number_of_steps;

    Integrator integrator;

    ManeuverRequest maneuver;
    ControlConfig control_config;
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

    double command_duration;
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
    double command_duration,
    SimulationConfig config
);

State propagate_state(
    State state,
    Vector2D thrust_acceleration,
    SimulationConfig config
);

std::vector<SimulationStep> run_simulation(
    SimulationConfig config
);

std::vector<SimulationStep> run_guided_maneuver(
    ManeuverRequest maneuver,
    ControlConfig control_config,
    SimulationConfig simulation_config
);

#endif