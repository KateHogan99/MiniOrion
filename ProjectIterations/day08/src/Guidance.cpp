#include "Guidance.h"

Vector2D calculate_position_error(
    Vector2D current_position,
    Vector2D desired_position
)
{
    return subtract_vectors(
        desired_position,
        current_position
    );
}

Vector2D calculate_velocity_error(
    Vector2D current_velocity,
    Vector2D desired_velocity
)
{
    return subtract_vectors(
        desired_velocity,
        current_velocity
    );
}

GuidanceCommand calculate_guidance_command(
    ManeuverRequest maneuver,
    State current_state,
    double elapsed_time
)
{
    GuidanceCommand command;

    command.position_error =
        calculate_position_error(
            current_state.position,
            maneuver.desired_state.position
        );

    command.velocity_error =
        calculate_velocity_error(
            current_state.velocity,
            maneuver.desired_state.velocity
        );

    double T = maneuver.maneuver_duration;

    double t = elapsed_time;

    double time_remaining = T - t;

    if (time_remaining < 0.0)
    {
        time_remaining = 0.0;
    }

    command.time_remaining = time_remaining;

    /*
     * Cubic trajectory coefficients.
     *
     * r(t) = r0 + v0*t + c2*t^2 + c3*t^3
     *
     * These coefficients are calculated independently
     * for X and Y.
     */

    Vector2D position_delta =
        subtract_vectors(
            maneuver.desired_state.position,
            maneuver.initial_state.position
        );

    Vector2D c2;

    c2.x =
        (3.0 * position_delta.x / (T * T))
        -
        ((2.0 * maneuver.initial_state.velocity.x
          + maneuver.desired_state.velocity.x) / T);

    c2.y =
        (3.0 * position_delta.y / (T * T))
        -
        ((2.0 * maneuver.initial_state.velocity.y
          + maneuver.desired_state.velocity.y) / T);

    Vector2D c3;

    c3.x =
        (-2.0 * position_delta.x / (T * T * T))
        +
        ((maneuver.initial_state.velocity.x
          + maneuver.desired_state.velocity.x)
         / (T * T));

    c3.y =
        (-2.0 * position_delta.y / (T * T * T))
        +
        ((maneuver.initial_state.velocity.y
          + maneuver.desired_state.velocity.y)
         / (T * T));

    /*
     * Desired position:
     *
     * r(t) = r0 + v0*t + c2*t^2 + c3*t^3
     */

    command.desired_position.x =
        maneuver.initial_state.position.x
        +
        maneuver.initial_state.velocity.x * t
        +
        c2.x * t * t
        +
        c3.x * t * t * t;

    command.desired_position.y =
        maneuver.initial_state.position.y
        +
        maneuver.initial_state.velocity.y * t
        +
        c2.y * t * t
        +
        c3.y * t * t * t;

    /*
     * Desired velocity:
     *
     * v(t) = v0 + 2*c2*t + 3*c3*t^2
     */

    command.desired_velocity.x =
        maneuver.initial_state.velocity.x
        +
        2.0 * c2.x * t
        +
        3.0 * c3.x * t * t;

    command.desired_velocity.y =
        maneuver.initial_state.velocity.y
        +
        2.0 * c2.y * t
        +
        3.0 * c3.y * t * t;

    /*
     * Desired acceleration:
     *
     * a(t) = 2*c2 + 6*c3*t
     */

    command.desired_acceleration.x =
        2.0 * c2.x
        +
        6.0 * c3.x * t;

    command.desired_acceleration.y =
        2.0 * c2.y
        +
        6.0 * c3.y * t;

    return command;
}