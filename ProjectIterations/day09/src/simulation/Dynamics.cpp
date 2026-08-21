/**
 * @file Dynamics.cpp
 * @brief Implements the spacecraft equations of motion.
 *
 * Combines gravitational and commanded thrust acceleration and
 * calculates the resulting spacecraft state derivative.
 */

#include "Dynamics.h"

#include "OrbitalMechanics.h"

Vector2D calculate_total_acceleration(
    Vector2D position,
    Vector2D thrust_acceleration,
    double mu
)
{
    Vector2D gravity_acceleration =
        calculate_gravity_acceleration(
            position,
            mu
        );

    return add_vectors(
        gravity_acceleration,
        thrust_acceleration
    );
}

StateDerivative calculate_state_derivative(
    State state,
    Vector2D thrust_acceleration,
    double mu
)
{
    Vector2D total_acceleration =
        calculate_total_acceleration(
            state.position,
            thrust_acceleration,
            mu
        );

    return {
        state.velocity,
        total_acceleration
    };
}