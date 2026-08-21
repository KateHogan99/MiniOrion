/**
 * @file Integrator.h
 * @brief Defines numerical integration methods for spacecraft state propagation.
 *
 * The Integrator module advances a spacecraft state through time using
 * derivatives calculated by the Dynamics module.
 *
 * The current implementation provides:
 *
 *     - Forward Euler integration
 *     - Fourth-order Runge-Kutta (RK4) integration
 */

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "Dynamics.h"
#include "State.h"
#include "Vector2D.h"

/**
 * @brief Numerical integration methods supported by the simulation.
 */
enum class Integrator
{
    Euler,
    RK4
};

/**
 * @brief Applies a state derivative over a specified time interval.
 *
 * Performs:
 *
 *     state_next = state + derivative * dt
 *
 * This function is the common state-update operation used by
 * the numerical integration methods.
 *
 * @param state Current spacecraft state.
 * @param state_derivative State derivative to apply.
 * @param d_time Time interval over which to apply the derivative.
 *
 * @return Updated spacecraft state.
 */
State apply_state_derivative(
    State state,
    StateDerivative state_derivative,
    double d_time
);

/**
 * @brief Advances the spacecraft state using Forward Euler integration.
 *
 * @param current_state Current spacecraft state.
 * @param thrust_acceleration Commanded thrust acceleration.
 * @param mu Standard gravitational parameter of the central body.
 * @param d_time Integration timestep.
 *
 * @return State after one Euler integration step.
 */
State euler_integrate(
    State current_state,
    Vector2D thrust_acceleration,
    double mu,
    double d_time
);

/**
 * @brief Advances the spacecraft state using fourth-order Runge-Kutta integration.
 *
 * RK4 evaluates the equations of motion four times during each timestep
 * and combines the resulting derivatives using a weighted average.
 *
 * @param current_state Current spacecraft state.
 * @param thrust_acceleration Commanded thrust acceleration.
 * @param mu Standard gravitational parameter of the central body.
 * @param d_time Integration timestep.
 *
 * @return State after one RK4 integration step.
 */
State rk4_integrate(
    State current_state,
    Vector2D thrust_acceleration,
    double mu,
    double d_time
);

#endif