#include "Control.h"

#include <stdexcept>

ControlCommand calculate_control_command(
    State current_state,
    GuidanceCommand guidance_command,
    ControlConfig control_config,
    double command_duration
)
{
    (void)current_state;

    if (control_config.acceleration_constraint <= 0.0)
    {
        throw std::runtime_error(
            "Maximum acceleration must be greater than zero."
        );
    }

    Vector2D desired_acceleration =
        guidance_command.desired_acceleration;

    double desired_magnitude =
        calculate_magnitude(desired_acceleration);

    ControlCommand control_command;

    if (desired_magnitude == 0.0)
    {
        control_command.thrust_acceleration = {
            0.0,
            0.0
        };

        control_command.thrust_duration = 0.0;

        return control_command;
    }

    if (desired_magnitude <=
        control_config.acceleration_constraint)
    {
        control_command.thrust_acceleration =
            desired_acceleration;
    }
    else
    {
        control_command.thrust_acceleration =
            scale_vector(
                desired_acceleration,
                control_config.acceleration_constraint /
                desired_magnitude
            );
    }

    control_command.thrust_duration = command_duration;

    return control_command;
}