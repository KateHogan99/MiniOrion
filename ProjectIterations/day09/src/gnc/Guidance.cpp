/**
 * @file Guidance.cpp
 * @brief Implements trajectory generation for the Guidance module.
 *
 * The current implementation generates a cubic polynomial reference
 * trajectory independently for the X and Y axes.
 *
 * The trajectory is constrained to satisfy:
 *
 *     r(0) = initial position
 *     v(0) = initial velocity
 *     r(T) = desired position
 *     v(T) = desired velocity
 *
 * These four boundary conditions uniquely determine the quadratic and
 * cubic coefficients for each spatial axis.
 */

#include "Guidance.h"

#include <stdexcept>

TrajectoryCoefficients calculate_trajectory_coefficients(
    State initial_state,
    State desired_state,
    double maneuver_duration
)
{
    if (maneuver_duration <= 0.0)
    {
        throw std::runtime_error(
            "Maneuver duration must be greater than zero."
        );
    }

    Vector2D position_delta =
        subtract_vectors(
            desired_state.position,
            initial_state.position
        );

    double T = maneuver_duration;

    TrajectoryCoefficients coefficients;

    /*
     * Cubic trajectory:
     *
     *     r(t) = r0 + v0*t + c2*t^2 + c3*t^3
     *
     * The coefficients are chosen to satisfy both position and
     * velocity boundary conditions at t = 0 and t = T.
     */

    coefficients.c2.x =
        (3.0 * position_delta.x / (T * T))
        -
        (
            (2.0 * initial_state.velocity.x
            + desired_state.velocity.x)
            / T
        );

    coefficients.c2.y =
        (3.0 * position_delta.y / (T * T))
        -
        (
            (2.0 * initial_state.velocity.y
            + desired_state.velocity.y)
            / T
        );

    coefficients.c3.x =
        (-2.0 * position_delta.x / (T * T * T))
        +
        (
            (initial_state.velocity.x
            + desired_state.velocity.x)
            / (T * T)
        );

    coefficients.c3.y =
        (-2.0 * position_delta.y / (T * T * T))
        +
        (
            (initial_state.velocity.y
            + desired_state.velocity.y)
            / (T * T)
        );

    return coefficients;
}

GuidanceCommand calculate_guidance_command(
    ManeuverRequest maneuver_request,
    double elapsed_time
)
{
    if (maneuver_request.maneuver_duration <= 0.0)
    {
        throw std::runtime_error(
            "Maneuver duration must be greater than zero."
        );
    }

    double T =
        maneuver_request.maneuver_duration;

    double t = elapsed_time;

    if (t < 0.0)
    {
        t = 0.0;
    }

    if (t > T)
    {
        t = T;
    }

    TrajectoryCoefficients coefficients =
        calculate_trajectory_coefficients(
            maneuver_request.initial_state,
            maneuver_request.desired_state,
            T
        );

    GuidanceCommand command;

    command.time_remaining =
        T - t;

    /*
     * Reference position:
     *
     *     r(t) = r0 + v0*t + c2*t^2 + c3*t^3
     */

    command.desired_position.x =
        maneuver_request.initial_state.position.x
        +
        maneuver_request.initial_state.velocity.x * t
        +
        coefficients.c2.x * t * t
        +
        coefficients.c3.x * t * t * t;

    command.desired_position.y =
        maneuver_request.initial_state.position.y
        +
        maneuver_request.initial_state.velocity.y * t
        +
        coefficients.c2.y * t * t
        +
        coefficients.c3.y * t * t * t;

    /*
     * Reference velocity:
     *
     *     v(t) = v0 + 2*c2*t + 3*c3*t^2
     */

    command.desired_velocity.x =
        maneuver_request.initial_state.velocity.x
        +
        2.0 * coefficients.c2.x * t
        +
        3.0 * coefficients.c3.x * t * t;

    command.desired_velocity.y =
        maneuver_request.initial_state.velocity.y
        +
        2.0 * coefficients.c2.y * t
        +
        3.0 * coefficients.c3.y * t * t;

    /*
     * Reference acceleration:
     *
     *     a(t) = 2*c2 + 6*c3*t
     */

    command.desired_acceleration.x =
        2.0 * coefficients.c2.x
        +
        6.0 * coefficients.c3.x * t;

    command.desired_acceleration.y =
        2.0 * coefficients.c2.y
        +
        6.0 * coefficients.c3.y * t;

    return command;
}