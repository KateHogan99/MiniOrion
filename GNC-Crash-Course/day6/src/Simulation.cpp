#include "Simulation.h"

#include "Dynamics.h"
#include "OrbitalMechanics.h"

#include <stdexcept>

double calculate_displacement(Vector2D initial_position, Vector2D final_position)
{
    return calculate_magnitude(
        subtract_vectors(
            final_position,
            initial_position
        )
    );
}

SimulationStep calculate_simulation_step(State state, State previous_state, double time, double total_distance, SimulationConfig config)
{
    Vector2D gravity_acceleration =
        calculate_gravity_acceleration(
            state.position,
            config.mu
        );

    Vector2D total_acceleration =
        add_vectors(
            gravity_acceleration,
            config.thrust_acceleration
        );

    double radius =
        calculate_magnitude(state.position);

    double speed =
        calculate_magnitude(state.velocity);

    double orbital_energy = 0.0;

    if (config.mu != 0.0)
    {
        orbital_energy =
            calculate_specific_orbital_energy(
                state,
                config.mu
            );
    }

    double angular_momentum =
        calculate_specific_angular_momentum(state);

    double distance_this_step =
        calculate_displacement(
            previous_state.position,
            state.position
        );

    return {
        time,
        state,
        gravity_acceleration,
        config.thrust_acceleration,
        total_acceleration,
        radius,
        speed,
        orbital_energy,
        angular_momentum,
        distance_this_step,
        total_distance
    };
}

State propagate_state(State spacecraft, SimulationConfig config)
{
    switch (config.integrator)
    {
        case Integrator::Euler:
            return euler_integrate(
                spacecraft,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        case Integrator::RK4:
            return rk4_integrate(
                spacecraft,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        default:
            throw std::runtime_error(
                "Unknown integration method."
            );
    }
}

std::vector<SimulationStep> run_simulation(SimulationConfig config)
{
    State spacecraft;

    spacecraft.position =
        config.initial_position;

    spacecraft.velocity =
        config.initial_velocity;

    if (config.mu != 0.0 &&
        calculate_magnitude(spacecraft.position) == 0.0)
    {
        throw std::runtime_error(
            "Invalid initial condition: "
            "spacecraft cannot start at the origin "
            "when gravity is enabled."
        );
    }

    std::vector<SimulationStep> results;

    results.reserve(
        config.number_of_steps + 1
    );

    double total_distance = 0.0;

    // Store initial state at t = 0.

    SimulationStep initial_result =
        calculate_simulation_step(
            spacecraft,
            spacecraft,
            0.0,
            0.0,
            config
        );

    results.push_back(initial_result);

    // Propagate.

    for (int step = 1;
         step <= config.number_of_steps;
         ++step)
    {
        State previous_state =
            spacecraft;

        spacecraft =
            propagate_state(
                spacecraft,
                config
            );

        double distance_this_step =
            calculate_displacement(
                previous_state.position,
                spacecraft.position
            );

        total_distance +=
            distance_this_step;

        double elapsed_time =
            step * config.time_step;

        SimulationStep result =
            calculate_simulation_step(
                spacecraft,
                previous_state,
                elapsed_time,
                total_distance,
                config
            );

        results.push_back(result);
    }

    return results;
}