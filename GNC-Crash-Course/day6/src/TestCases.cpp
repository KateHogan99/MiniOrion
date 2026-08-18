#include "TestCases.h"

#include <cmath>

SimulationConfig create_no_gravity_test()
{
    SimulationConfig config;

    config.mu = 0.0;

    config.initial_position = {0.0, 0.0};
    config.initial_velocity = {10.0, 5.0};

    config.thrust_acceleration = {2.0, 1.0};

    double simulation_duration = 10.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration /
            config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}

SimulationConfig create_gravity_only_test()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};
    config.initial_velocity = {0.0, 0.0};

    config.thrust_acceleration = {0.0, 0.0};

    double simulation_duration = 10.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration /
            config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}

SimulationConfig create_circular_orbit_test()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};

    double radius = calculate_magnitude(config.initial_position);

    double circular_velocity = std::sqrt(config.mu / radius);

    config.initial_velocity = {0.0, circular_velocity};

    config.thrust_acceleration = {0.0, 0.0};

    double simulation_duration = 100.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration / config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}