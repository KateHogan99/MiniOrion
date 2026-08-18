#include "Control.h"
#include "Guidance.h"
#include "Simulation.h"
#include "State.h"
#include "Vector2D.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

int main()
{
    /*
     * ========================================================
     * MANEUVER DEFINITION
     * ========================================================
     *
     * Move from:
     *
     *   Position: (0, 0) m
     *   Velocity: (0, 0) m/s
     *
     * to:
     *
     *   Position: (100, 0) m
     *   Velocity: (0, 0) m/s
     *
     * over exactly 10 seconds.
     */

    ManeuverRequest maneuver;

    maneuver.initial_state.position = {
        0.0,
        0.0
    };

    maneuver.initial_state.velocity = {
        0.0,
        0.0
    };

    maneuver.desired_state.position = {
        100.0,
        0.0
    };

    maneuver.desired_state.velocity = {
        0.0,
        0.0
    };

    maneuver.maneuver_duration =
        10.0;


    /*
     * ========================================================
     * CONTROL CONFIGURATION
     * ========================================================
     */

    ControlConfig control_config;

    control_config.maximum_acceleration =
        100.0;


    /*
     * ========================================================
     * SIMULATION CONFIGURATION
     * ========================================================
     */

    SimulationConfig simulation_config;

    simulation_config.mu =
        0.0;

    simulation_config.initial_position =
        maneuver.initial_state.position;

    simulation_config.initial_velocity =
        maneuver.initial_state.velocity;

    simulation_config.thrust_acceleration = {
        0.0,
        0.0
    };

    simulation_config.time_step =
        0.1;

    simulation_config.number_of_steps =
        static_cast<int>(
            maneuver.maneuver_duration /
            simulation_config.time_step
        );

    simulation_config.integrator =
        Integrator::RK4;


    /*
     * ========================================================
     * RUN GUIDED MANEUVER
     * ========================================================
     */

    try
    {
        std::vector<SimulationStep> results =
            run_guided_maneuver(
                maneuver,
                control_config,
                simulation_config
            );


        /*
         * ====================================================
         * OUTPUT
         * ====================================================
         */

        std::cout
            << std::fixed
            << std::setprecision(4);

        std::cout
            << "============================================================\n"
            << "GUIDED MANEUVER SIMULATION\n"
            << "============================================================\n";

        std::cout
            << "Maneuver duration: "
            << maneuver.maneuver_duration
            << " s\n";

        std::cout
            << "Maximum acceleration: "
            << control_config.maximum_acceleration
            << " m/s^2\n";

        std::cout
            << "Simulation timestep: "
            << simulation_config.time_step
            << " s\n\n";


        std::cout
            << std::left
            << std::setw(10) << "Time"
            << std::setw(12) << "X"
            << std::setw(12) << "Y"
            << std::setw(12) << "VX"
            << std::setw(12) << "VY"
            << std::setw(14) << "Thrust X"
            << std::setw(14) << "Thrust Y"
            << "\n";

        std::cout
            << std::string(86, '-')
            << "\n";


        for (const SimulationStep& result : results)
        {
            std::cout
                << std::left
                << std::setw(10)
                << result.time

                << std::setw(12)
                << result.state.position.x

                << std::setw(12)
                << result.state.position.y

                << std::setw(12)
                << result.state.velocity.x

                << std::setw(12)
                << result.state.velocity.y

                << std::setw(14)
                << result.thrust_acceleration.x

                << std::setw(14)
                << result.thrust_acceleration.y

                << "\n";
        }


        /*
         * ====================================================
         * FINAL STATE VALIDATION
         * ====================================================
         */

        const SimulationStep& final_result =
            results.back();

        Vector2D final_position_error =
            subtract_vectors(
                maneuver.desired_state.position,
                final_result.state.position
            );

        Vector2D final_velocity_error =
            subtract_vectors(
                maneuver.desired_state.velocity,
                final_result.state.velocity
            );

        double position_error =
            calculate_magnitude(
                final_position_error
            );

        double velocity_error =
            calculate_magnitude(
                final_velocity_error
            );


        std::cout
            << "\n"
            << "============================================================\n"
            << "FINAL MANEUVER VALIDATION\n"
            << "============================================================\n";

        std::cout
            << "Final time: "
            << final_result.time
            << " s\n";

        std::cout
            << "\nDesired position: ("
            << maneuver.desired_state.position.x
            << ", "
            << maneuver.desired_state.position.y
            << ")\n";

        std::cout
            << "Final position:   ("
            << final_result.state.position.x
            << ", "
            << final_result.state.position.y
            << ")\n";

        std::cout
            << "Position error: "
            << position_error
            << " m\n";

        std::cout
            << "\nDesired velocity: ("
            << maneuver.desired_state.velocity.x
            << ", "
            << maneuver.desired_state.velocity.y
            << ")\n";

        std::cout
            << "Final velocity:   ("
            << final_result.state.velocity.x
            << ", "
            << final_result.state.velocity.y
            << ")\n";

        std::cout
            << "Velocity error: "
            << velocity_error
            << " m/s\n";

        std::cout
            << "\nTotal distance traveled: "
            << final_result.total_distance
            << " m\n";
    }
    catch (const std::runtime_error& error)
    {
        std::cerr
            << "Simulation error: "
            << error.what()
            << "\n";

        return 1;
    }

    return 0;
}