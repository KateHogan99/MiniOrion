#include "Control.h"
#include "Guidance.h"
#include "GNCValidation.h"
#include "Simulation.h"

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
     *   Position: (100, 50) m
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
        50.0
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

    control_config.acceleration_constraint =
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
        0.05;

    simulation_config.number_of_steps =
        static_cast<int>(
            maneuver.maneuver_duration /
            simulation_config.time_step
        );

    simulation_config.integrator =
        Integrator::RK4;

    simulation_config.maneuver =
        maneuver;

    simulation_config.control_config =
        control_config;


    /*
     * ========================================================
     * GNC VALIDATION CONFIGURATION
     * ========================================================
     */

    GNCValidationConfig validation_config;

    validation_config.boundary_position_tolerance =
        1e-9;

    validation_config.boundary_velocity_tolerance =
        1e-9;

    validation_config.intermediate_position_tolerance =
        0.1;

    validation_config.intermediate_velocity_tolerance =
        0.1;

    validation_config.acceleration_tolerance =
        1e-9;

    validation_config.command_duration_tolerance =
        1e-9;

    validation_config.final_position_tolerance =
        0.1;

    validation_config.final_velocity_tolerance =
        0.1;

    validation_config.maneuver_duration_tolerance =
        1e-9;

    validation_config.simulated_trajectory_tolerance =
        0.1;


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
         * SIMULATION OUTPUT
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
            << "Acceleration constraint: "
            << control_config.acceleration_constraint
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
         * GNC VALIDATION
         * ====================================================
         */

        gnc_validate_simulation(
            results,
            simulation_config,
            validation_config
        );
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