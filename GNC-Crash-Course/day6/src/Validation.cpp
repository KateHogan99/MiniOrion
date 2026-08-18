#include "Validation.h"

#include <iostream>

// Constant-acceleration analytical solution:
// r(t) = r0 + v0*t + 1/2*a*t^2
// This is NOT used for propagation.
// It exists to validate the numerical integrators.

State calculate_analytical_state(State initial_state, Vector2D acceleration, double time)
{
    Vector2D position =
        add_vectors(
            add_vectors(
                initial_state.position,
                scale_vector(
                    initial_state.velocity,
                    time
                )
            ),
            scale_vector(
                acceleration,
                0.5 * time * time
            )
        );

    Vector2D velocity =
        add_vectors(
            initial_state.velocity,
            scale_vector(
                acceleration,
                time
            )
        );

    return {
        position,
        velocity
    };
}

void validate_constant_acceleration(const std::vector<SimulationStep>& results, SimulationConfig config)
{
    if (config.mu != 0.0)
    {
        return;
    }

    State initial_state = results.front().state;

    State analytical_state =
        calculate_analytical_state(
            initial_state,
            config.thrust_acceleration,
            results.back().time
        );

    State numerical_state =
        results.back().state;

    double position_error =
        calculate_displacement(
            analytical_state.position,
            numerical_state.position
        );

    double velocity_error =
        calculate_displacement(
            analytical_state.velocity,
            numerical_state.velocity
        );

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "ANALYTICAL VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Analytical final position: ("
        << analytical_state.position.x
        << ", "
        << analytical_state.position.y
        << ")\n";

    std::cout
        << "Numerical final position:  ("
        << numerical_state.position.x
        << ", "
        << numerical_state.position.y
        << ")\n";

    std::cout
        << "Position error: "
        << position_error
        << "\n";

    std::cout
        << "Velocity error: "
        << velocity_error
        << "\n";
}

void validate_simulation(const std::vector<SimulationStep>& results, SimulationConfig config)
{
    validate_constant_acceleration(results, config);
}