#include "Simulation.h"
#include "TestCases.h"
#include "Validation.h"

#include <iostream>
#include <stdexcept>

int main()
{
    SimulationConfig sim_config =
        create_escape_trajectory_test();

    ValidationConfig val_config;

    val_config.constant_acceleration_tolerance_position_percent = 0.01;
    val_config.constant_acceleration_tolerance_velocity_percent = 0.01;

    val_config.circular_orbit_tolerance_percent = 0.01;
    val_config.circular_orbit_speed_tolerance_percent = 0.01;

    val_config.orbital_energy_tolerance_percent = 0.01;
    val_config.angular_momentum_tolerance_percent = 0.01;

    val_config.elliptical_orbit_tolerance = 0.001;

    try
    {
        std::vector<SimulationStep> results =
            run_simulation(sim_config);

        validate_simulation(
            results,
            sim_config,
            val_config
        );
    }
    catch (const std::runtime_error& error)
    {
        std::cerr
            << "Simulation error: "
            << error.what()
            << "\n";

        return 1;
    }

    return 0;
}