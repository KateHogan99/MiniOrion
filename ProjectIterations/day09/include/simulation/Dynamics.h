/**
 * @file Dynamics.h
 * @brief Defines the equations of motion for the spacecraft.
 *
 * The Dynamics module determines how the spacecraft state changes
 * under the influence of modeled accelerations.
 *
 * The current implementation models:
 *
 *     - point-mass gravitational acceleration
 *     - commanded thrust acceleration
 *
 * Dynamics provides the state derivatives required by numerical
 * integration methods.
 */

#ifndef DYNAMICS_H
#define DYNAMICS_H

#include "State.h"
#include "Vector2D.h"

/**
 * @brief Represents the time derivative of a spacecraft state.
 *
 * The state derivative describes how the spacecraft's position and
 * velocity change with respect to time.
 *
 * For the current model:
 *
 *     dr/dt = v
 *
 *     dv/dt = a_total
 *
 * Mass is not included because the current model assumes constant
 * spacecraft mass. A future propulsion model may add a mass derivative.
 */
struct StateDerivative
{
    /**
     * @brief Time derivative of spacecraft position.
     *
     * Equal to the spacecraft velocity.
     *
     * Units: meters per second (m/s).
     */
    Vector2D position_derivative;

    /**
     * @brief Time derivative of spacecraft velocity.
     *
     * Equal to the total acceleration acting on the spacecraft.
     *
     * Units: meters per second squared (m/s^2).
     */
    Vector2D velocity_derivative;
};

/**
 * @brief Calculates the total acceleration acting on the spacecraft.
 *
 * The current model combines gravitational acceleration and commanded
 * thrust acceleration:
 *
 *     a_total = a_gravity + a_thrust
 *
 * @param position Spacecraft position relative to the central body.
 * @param thrust_acceleration Commanded non-gravitational acceleration.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Total spacecraft acceleration.
 *
 * Units: meters per second squared (m/s^2).
 */
Vector2D calculate_total_acceleration(
    Vector2D position,
    Vector2D thrust_acceleration,
    double mu
);

/**
 * @brief Calculates the derivative of the spacecraft state.
 *
 * The equations of motion are:
 *
 *     dr/dt = v
 *
 *     dv/dt = a_gravity + a_thrust
 *
 * @param state Current spacecraft state.
 * @param thrust_acceleration Commanded non-gravitational acceleration.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return State derivative at the current state.
 */
StateDerivative calculate_state_derivative(
    State state,
    Vector2D thrust_acceleration,
    double mu
);

#endif