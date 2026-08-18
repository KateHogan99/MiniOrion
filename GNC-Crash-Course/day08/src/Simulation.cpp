#include "Simulation.h"

#include "Dynamics.h"
#include "Guidance.h"
#include "OrbitalMechanics.h"

#include <cmath>
#include <stdexcept>
#include <vector>

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
        calculate_magnitude(
            state.position
        );

    double speed =
        calculate_magnitude(
            state.velocity
        );

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
        calculate_specific_angular_momentum(
            state
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

State propagate_state(
    State state,
    Vector2D thrust_acceleration,
    SimulationConfig config
)
{
    switch (config.integrator)
    {
        case Integrator::Euler:
            return euler_integrate(
                state,
                thrust_acceleration,
                config.mu,
                config.time_step
            );

        case Integrator::RK4:
            return rk4_integrate(
                state,
                thrust_acceleration,
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

    state.position =
        config.initial_position;

    state.velocity =
        config.initial_velocity;

    if (config.mu != 0.0 &&
        calculate_magnitude(state.position) == 0.0)
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

    SimulationStep initial_result =
        calculate_simulation_step(
            state,
            0.0,
            0.0,
            0.0,
            config
        );

    results.push_back(
        initial_result
    );

    for (int step = 1;
         step <= config.number_of_steps;
         ++step)
    {
        State previous_state =
            state;

        state =
            propagate_state(
                state,
                config.thrust_acceleration,
                config
            );

        double distance_this_step =
            calculate_displacement(
                previous_state.position,
                state.position
            );

        total_distance +=
            distance_this_step;

        double elapsed_time =
            step * config.time_step;

        SimulationStep result =
            calculate_simulation_step(
                state,
                elapsed_time,
                distance_this_step,
                total_distance,
                config
            );

        results.push_back(
            result
        );
    }

    return results;
}

std::vector<SimulationStep> run_guided_maneuver(
    ManeuverRequest maneuver,
    ControlConfig control_config,
    SimulationConfig simulation_config
)
{
    if (maneuver.maneuver_duration <= 0.0)
    {
        throw std::runtime_error(
            "Maneuver duration must be greater than zero."
        );
    }

    if (simulation_config.time_step <= 0.0)
    {
        throw std::runtime_error(
            "Simulation timestep must be greater than zero."
        );
    }

    if (simulation_config.mu != 0.0 &&
        calculate_magnitude(
            maneuver.initial_state.position
        ) == 0.0)
    {
        throw std::runtime_error(
            "Invalid initial condition: "
            "spacecraft cannot start at the origin "
            "when gravity is enabled."
        );
    }

    State state =
        maneuver.initial_state;

    std::vector<SimulationStep> results;

    double total_distance = 0.0;
    double elapsed_time = 0.0;

    results.reserve(
        static_cast<int>(
            std::ceil(
                maneuver.maneuver_duration /
                simulation_config.time_step
            )
        ) + 1
    );

    /*
     * Store the initial state at t = 0.
     *
     * No control command has been applied yet.
     */
    SimulationConfig initial_config =
        simulation_config;

    initial_config.thrust_acceleration = {
        0.0,
        0.0
    };

    results.push_back(
        calculate_simulation_step(
            state,
            0.0,
            0.0,
            0.0,
            initial_config
        )
    );

    /*
     * Closed-loop guidance and control loop.
     *
     * At every timestep:
     *
     *   1. Guidance determines the desired acceleration.
     *   2. Control applies acceleration constraints.
     *   3. Dynamics/integration propagates the spacecraft.
     *   4. The resulting state becomes the next input.
     */
    while (
        elapsed_time <
        maneuver.maneuver_duration
    )
    {
        double remaining_time =
            maneuver.maneuver_duration -
            elapsed_time;

        /*
         * Normally use the configured timestep.
         *
         * If the remaining maneuver duration is
         * smaller than the timestep, use the remaining
         * duration so that the maneuver ends exactly
         * at the requested time.
         */
        double current_time_step =
            simulation_config.time_step;

        if (remaining_time <
            current_time_step)
        {
            current_time_step =
                remaining_time;
        }

        GuidanceCommand guidance_command =
            calculate_guidance_command(
                maneuver,
                state,
                elapsed_time
            );

        ControlCommand control_command =
            calculate_control_command(
                state,
                guidance_command,
                control_config,
                current_time_step
            );

        SimulationConfig current_config =
            simulation_config;

        current_config.time_step =
            current_time_step;

        current_config.thrust_acceleration =
            control_command.thrust_acceleration;

        State previous_state =
            state;

        state =
            propagate_state(
                state,
                current_config.thrust_acceleration,
                current_config
            );

        double distance_this_step =
            calculate_displacement(
                previous_state.position,
                state.position
            );

        total_distance +=
            distance_this_step;

        elapsed_time +=
            current_time_step;

        /*
         * Prevent floating-point accumulation from
         * pushing the recorded time beyond the requested
         * maneuver duration.
         */
        if (elapsed_time >
            maneuver.maneuver_duration)
        {
            elapsed_time =
                maneuver.maneuver_duration;
        }

        SimulationStep result =
            calculate_simulation_step(
                state,
                elapsed_time,
                distance_this_step,
                total_distance,
                current_config
            );

        results.push_back(
            result
        );
    }

    return results;
}