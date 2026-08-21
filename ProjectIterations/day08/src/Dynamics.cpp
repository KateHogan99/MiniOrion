#include "Dynamics.h"

#include <stdexcept>

Vector2D calculate_gravity_acceleration(
    Vector2D position, 
    double mu
)
{
    if (mu == 0.0)
    {
        return {0.0, 0.0};
    }

    double radius = calculate_magnitude(position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate acceleration due to "
            "gravity: spacecraft is at the origin."
        );
    }

    return scale_vector(
        position,
        - mu / (radius * radius * radius)
    );
}

StateDerivative calculate_state_derivative(
    State state, 
    Vector2D thrust_acceleration, 
    double mu
)
{
    Vector2D gravity_acceleration = calculate_gravity_acceleration(
        state.position,
        mu
    );

    Vector2D total_acceleration = add_vectors(
        gravity_acceleration,
        thrust_acceleration
    );

    return {
        state.velocity,
        total_acceleration
    };
}