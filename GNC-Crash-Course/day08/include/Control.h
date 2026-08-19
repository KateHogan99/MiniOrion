#ifndef CONTROL_H
#define CONTROL_H

#include "Guidance.h"
#include "State.h"
#include "Vector2D.h"

struct ControlConfig
{
    double acceleration_constraint; // Unit: m/s^2
};

struct ControlCommand
{
    Vector2D thrust_acceleration;
    double thrust_duration; // Unit: seconds
};

ControlCommand calculate_control_command(
    State current_state,
    GuidanceCommand guidance_command,
    ControlConfig control_config,
    double command_duration
);

#endif