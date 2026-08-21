/**
 * @file OrbitalMechanics.cpp
 * @brief Implements analytical orbital mechanics calculations.
 *
 * Provides calculations of specific orbital energy, specific angular
 * momentum, orbital eccentricity, acceleration due to gravity, 
 * circular orbital velocity, and escape velocity using the idealized 
 * two-body gravitational model declared in OrbitalMechanics.h.
 */

#include "OrbitalMechanics.h"

#include <cmath>
#include <stdexcept>

double calculate_specific_orbital_energy(
    State state,
    double mu
)
{
    // Specific orbital mechanical energy:
    //
    //     epsilon = v^2 / 2 - mu / r
    //
    // For an ideal two-body orbit with no thrust or other
    // non-gravitational forces, specific orbital energy is conserved.

    double radius =
        calculate_magnitude(state.position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate orbital energy: "
            "spacecraft is at the origin."
        );
    }

    double speed =
        calculate_magnitude(state.velocity);

    return
        (speed * speed / 2.0) -
        (mu / radius);
}

double calculate_specific_angular_momentum(
    State state
)
{
    // Specific angular momentum:
    //
    //     h = r x v
    //
    // In the two-dimensional model, only the component
    // perpendicular to the simulation plane remains:
    //
    //     h = x * v_y - y * v_x

    return
        state.position.x * state.velocity.y -
        state.position.y * state.velocity.x;
}

double calculate_eccentricity(
    State state,
    double mu
)
{
    // Eccentricity can be calculated from specific orbital
    // energy and specific angular momentum:
    //
    //     e = sqrt(1 + (2 * epsilon * h^2) / mu^2)
    //
    // For an ideal two-body orbit:
    //
    //     e = 0       circular
    //     0 < e < 1   elliptical
    //     e = 1       parabolic
    //     e > 1       hyperbolic

    if (mu == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate eccentricity: "
            "gravitational parameter mu is zero."
        );
    }

    double orbital_energy =
        calculate_specific_orbital_energy(
            state,
            mu
        );

    double angular_momentum =
        calculate_specific_angular_momentum(
            state
        );

    return
        std::sqrt(
            1.0 +
            (
                2.0 *
                orbital_energy *
                angular_momentum *
                angular_momentum
            ) /
            (mu * mu)
        );
}

Vector2D calculate_gravity_acceleration(
    Vector2D position,
    double mu
)
{
    double radius =
        calculate_magnitude(position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate gravitational acceleration "
            "at zero radius."
        );
    }

    double acceleration_magnitude =
        -mu / (radius * radius);

    return scale_vector(
        position,
        acceleration_magnitude / radius
    );
}

double calculate_circular_orbital_velocity(
    double radius,
    double mu
)
{
    if (radius <= 0.0)
    {
        throw std::runtime_error(
            "Orbital radius must be greater than zero."
        );
    }

    return std::sqrt(
        mu / radius
    );
}

double calculate_escape_velocity(
    double radius,
    double mu
)
{
    if (radius <= 0.0)
    {
        throw std::runtime_error(
            "Orbital radius must be greater than zero."
        );
    }

    return std::sqrt(
        2.0 * mu / radius
    );
}