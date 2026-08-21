#include "Simulation.h"

#include "Dynamics.h"
#include "OrbitalMechanics.h"

#include <stdexcept>

double calculate_displacement(
    Vector2D initial_position,
    Vector2D final_position
)
{
    return calculate_magnitude(
        subtract_vectors(
            final_position,
            initial_position
        )
    );
}

SimulationStep calculate_simulation_step(
    State state,
    double time,
    double distance_this_step,
    double total_distance,
    SimulationConfig config
)
{
    SimulationStep simulation_step;
    simulation_step.time = time;
    simulation_step.state = state;

    // Acceleration
    simulation_step.gravity_acceleration = 
        calculate_gravity_acceleration(
            state.position,
            config.mu
        );
    simulation_step.thrust_acceleration = config.thrust_acceleration;
    simulation_step.total_acceleration = 
        add_vectors(
            simulation_step.gravity_acceleration,
            simulation_step.thrust_acceleration
        );
    
    // Properties
    simulation_step.radius = calculate_magnitude(state.position);
    simulation_step.speed = calculate_magnitude(state.velocity);
    if (config.mu == 0.0)
    {
        simulation_step.orbital_energy = 0.0;
    } 
    else 
    {
        simulation_step.orbital_energy = 
            calculate_specific_orbital_energy(
                state,
                config.mu
            );
    }
    simulation_step.angular_momentum = calculate_specific_angular_momentum(state);

    // Distance
    simulation_step.distance_this_step = distance_this_step;
    simulation_step.total_distance = total_distance;

    return simulation_step;
}

State propagate_state(
    State state,
    SimulationConfig config
)
{
    switch (config.integrator)
    {
        case Integrator::Euler:
            return euler_integrate(
                state,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        case Integrator::RK4:
            return rk4_integrate(
                state,
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

std::vector<SimulationStep> run_simulation(
    SimulationConfig config
)
{
    State state;
    state.position = config.initial_position;
    state.velocity = config.initial_velocity;

    if (config.mu != 0.0 &&
        calculate_magnitude(state.position) == 0.0)
    {
        throw std::runtime_error(
            "Invalid initial condition: spacecraft cannot "
            "start at the origin when gravity is enabled."
        );
    }

    std::vector<SimulationStep> results;

    results.reserve(config.number_of_steps + 1);

    double total_distance = 0.0;

    // Store initial state at t = 0.

    SimulationStep initial_result = 
        calculate_simulation_step(
            state,
            0.0,
            0.0,
            total_distance,
            config
        );

    results.push_back(initial_result);

    // Propagate
    for (int step = 1; step <= config.number_of_steps; ++step)
    {
        State previous_state = state;

        state = propagate_state(state, config);

        double distance_this_step = calculate_displacement(
            previous_state.position, 
            state.position
        );
        total_distance += distance_this_step;

        double elapsed_time = step * config.time_step;

        SimulationStep result = calculate_simulation_step(
            state,
            elapsed_time,
            distance_this_step,
            total_distance,
            config
        );

        results.push_back(result);
    }
    
    return results;
}