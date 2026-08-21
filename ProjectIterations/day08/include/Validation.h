#ifndef VALIDATION_H
#define VALIDATION_H

#include "Simulation.h"

#include <string>
#include <vector>

struct ValidationConfig
{
    double constant_acceleration_tolerance_position_percent;
    double constant_acceleration_tolerance_velocity_percent;

    double circular_orbit_tolerance_percent;
    double circular_orbit_speed_tolerance_percent;

    double orbital_energy_tolerance_percent;
    double angular_momentum_tolerance_percent;

    double elliptical_orbit_tolerance;
};

double calculate_relative_error_percent(
    double value,
    double reference
);

State calculate_analytical_state(
    State initial_state,
    Vector2D acceleration,
    double time
);

std::string validate_constant_acceleration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

std::string validate_circular_orbit(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

std::string validate_orbital_conservation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

std::string validate_angular_conservation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

std::string validate_elliptical_orbit(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

void validate_simulation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
);

#endif