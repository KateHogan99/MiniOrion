#include "OrbitalMechanics.h"

#include <cmath>
#include <stdexcept>

double calculate_specific_orbital_energy(
    State state,
    double mu
)
{
    // Specific orbital mechanical energy:
    // epsilon = v^2 / 2 - mu / r
    // Units: energy per unit mass.
    // For an ideal two-body orbit with no thrust,
    // specific orbital energy should remain constant.

    double radius = calculate_magnitude(state.position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate orbital energy: "
            "spacecraft is at the origin."
        );
    }

    double speed = calculate_magnitude(state.velocity);

    return
        (speed * speed / 2.0) - 
        (mu / radius);
}

double calculate_specific_angular_momentum(
    State state
)
{
    // Specific angular momentum:
    // h = r x v
    // In 2D, only the z-component exists:
    // h_z = x * v_y - y * v_x
    // This is a scalar.

    return
        state.position.x * state.velocity.y - 
        state.position.y * state.velocity.x;
}

double calculate_eccentricity(
    State state,
    double mu
)
{
    // Eccentricity:
    // e = √(1 + (2ϵh^2 / μ^2))
    // For a circle: e = 0
    // For an ellipse: 0 <= e < 1
    // For a parabola: e = 1
    // For a hyperbola: e > 1

    double radius = calculate_magnitude(state.position);

    if (mu == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate eccentricity: "
            "gravitational parameter mu is zero."
        );
    }

    double orbital_energy = calculate_specific_orbital_energy(state, mu);
    double angular_momentum = calculate_specific_angular_momentum(state);

    return
        std::sqrt(
            1 + (
                (2 * orbital_energy * angular_momentum * angular_momentum) / 
                (mu * mu)
            )
        );
}