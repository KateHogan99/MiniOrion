#ifndef GNCVALIDATION_H
#define GNCVALIDATION_H

#include "Control.h"
#include "Guidance.h"
#include "Simulation.h"

#include <string>
#include <vector>

struct TrajectoryCoefficients
{
    Vector2D c2;
    Vector2D c3;
};

struct GNCValidationConfig
{
    double boundary_position_tolerance;
    double boundary_velocity_tolerance;

    double intermediate_position_tolerance;
    double intermediate_velocity_tolerance;

    double acceleration_tolerance;
    double command_duration_tolerance;
    double maneuver_duration_tolerance;

    double final_position_tolerance;
    double final_velocity_tolerance;

    double simulated_trajectory_tolerance;
};

TrajectoryCoefficients calculate_trajectory_coefficients(
    State initial_state,
    State desired_state,
    double maneuver_duration
);

Vector2D calculate_reference_position(
    State initial_state,
    TrajectoryCoefficients coefficients,
    double reference_time
);

Vector2D calculate_reference_velocity(
    State initial_state,
    TrajectoryCoefficients coefficients,
    double reference_time
);

std::string validate_boundary_conditions(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

double calculate_position_trajectory_error(
    State initial_state,
    SimulationStep simulation_step,
    TrajectoryCoefficients coefficients
);

double calculate_velocity_trajectory_error(
    State initial_state,
    SimulationStep simulation_step,
    TrajectoryCoefficients coefficients
);

std::string validate_intermediate_trajectory(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_acceleration_constraint(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_command_duration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_final_position(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_final_velocity(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_maneuver_duration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

std::string validate_simulated_trajectory(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

void gnc_validate_simulation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
);

#endif