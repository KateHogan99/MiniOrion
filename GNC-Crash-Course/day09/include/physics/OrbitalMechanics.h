/**
 * @file OrbitalMechanics.h
 * @brief Defines analytical orbital mechanics calculations.
 *
 * Provides calculations used to characterize the spacecraft's
 * orbit under the idealized two-body gravitational model.
 *
 * These calculations are independent of numerical integration and
 * can therefore be used to analyze and validate simulation results.
 */

#ifndef ORBITALMECHANICS_H
#define ORBITALMECHANICS_H

#include "State.h"

/**
 * @brief Calculates the spacecraft's specific orbital energy.
 *
 * Specific orbital mechanical energy is given by:
 *
 *     epsilon = v^2 / 2 - mu / r
 *
 * where:
 *
 *     epsilon = specific orbital energy
 *     v       = spacecraft speed
 *     mu      = gravitational parameter
 *     r       = spacecraft distance from the central body
 *
 * For an ideal two-body system with no non-gravitational forces,
 * specific orbital energy remains constant throughout the orbit.
 *
 * The sign of the specific orbital energy indicates the type of
 * trajectory:
 *
 *     epsilon < 0  : bound orbit
 *     epsilon = 0  : parabolic escape trajectory
 *     epsilon > 0  : hyperbolic trajectory
 *
 * @param state Spacecraft state containing position and velocity.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Specific orbital energy.
 *
 * Units: J/kg.
 *
 * @throws std::runtime_error If the spacecraft is at the origin,
 *         where the point-mass gravitational model is undefined.
 */
double calculate_specific_orbital_energy(
    State state,
    double mu
);

/**
 * @brief Calculates the spacecraft's specific angular momentum.
 *
 * Specific angular momentum is defined by:
 *
 *     h = r x v
 *
 * In the two-dimensional simulation, only the component perpendicular
 * to the simulation plane is non-zero. Its magnitude and sign are
 * calculated as:
 *
 *     h = x * v_y - y * v_x
 *
 * The sign indicates the direction of orbital motion relative to
 * the chosen coordinate system.
 *
 * @param state Spacecraft state containing position and velocity.
 *
 * @return The out-of-plane component of specific angular momentum.
 *
 * Units: m^2/s.
 */
double calculate_specific_angular_momentum(
    State state
);

/**
 * @brief Calculates orbital eccentricity from the spacecraft state.
 *
 * Eccentricity is calculated from specific orbital energy and
 * specific angular momentum:
 *
 *     e = sqrt(1 + (2 * epsilon * h^2) / mu^2)
 *
 * The resulting eccentricity characterizes the orbital trajectory:
 *
 *     e = 0       : circular
 *     0 < e < 1   : elliptical
 *     e = 1       : parabolic
 *     e > 1       : hyperbolic
 *
 * @param state Spacecraft state containing position and velocity.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Orbital eccentricity.
 *
 * @throws std::runtime_error If the gravitational parameter is zero.
 */
double calculate_eccentricity(
    State state,
    double mu
);

/**
 * @brief Calculates gravitational acceleration at a given position.
 *
 * Uses the point-mass gravitational acceleration equation:
 *
 *     a = -mu / ||r||^3 * r
 *
 * where:
 *
 *     a      = gravitational acceleration vector
 *     mu     = gravitational parameter of the central body
 *     r      = position vector relative to the central body
 *     ||r||  = magnitude of the position vector
 *
 * The resulting acceleration points toward the central body.
 *
 * @param position Position relative to the central body.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Gravitational acceleration vector.
 *
 * @throws std::runtime_error If the position magnitude is zero.
 */
Vector2D calculate_gravity_acceleration(
    Vector2D position,
    double mu
);

/**
 * @brief Calculates the circular orbital velocity at a given radius.
 *
 * Uses the circular-orbit relationship:
 *
 *     v = sqrt(mu / r)
 *
 * where mu is the gravitational parameter and r is the
 * orbital radius.
 *
 * @param radius Distance from the central body's center.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Circular orbital velocity magnitude.
 *
 * @throws std::runtime_error If the radius is not greater than zero.
 */
double calculate_circular_orbital_velocity(
    double radius,
    double mu
);

/**
 * @brief Calculates the escape velocity at a given radius.
 *
 * Uses the escape velocity equation:
 *
 *     v_escape = sqrt(2 * mu / r)
 *
 * where mu is the gravitational parameter and r is the
 * distance from the central body.
 *
 * @param radius Distance from the central body's center.
 * @param mu Standard gravitational parameter of the central body.
 *
 * @return Escape velocity magnitude.
 *
 * @throws std::runtime_error If the radius is not greater than zero.
 */
double calculate_escape_velocity(
    double radius,
    double mu
);

#endif