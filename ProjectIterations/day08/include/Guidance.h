#ifndef GUIDANCE_H
#define GUIDANCE_H

#include "State.h"
#include "Vector2D.h"

struct ManeuverRequest
{
    State initial_state;
    State desired_state;

    double maneuver_duration; // Unit: seconds
};

struct GuidanceCommand
{
    Vector2D position_error;
    Vector2D velocity_error;

    Vector2D desired_position;
    Vector2D desired_velocity;
    Vector2D desired_acceleration;

    double time_remaining; // Unit: seconds
};

Vector2D calculate_position_error(
    Vector2D current_position,
    Vector2D desired_position
);

Vector2D calculate_velocity_error(
    Vector2D current_velocity,
    Vector2D desired_velocity
);

GuidanceCommand calculate_guidance_command(
    ManeuverRequest maneuver,
    State current_state,
    double elapsed_time
);

#endif