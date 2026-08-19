#include "GNCValidation.h"

#include "OrbitalMechanics.h"
#include "Validation.h"

#include <cmath>
#include <iostream>

// --------------------------------------------------------
// CUBIC TRAJECTORY
// --------------------------------------------------------
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

    coefficients.c2.x =
        (3.0 * position_delta.x / (T * T))
        -
        ((2.0 * initial_state.velocity.x
          + desired_state.velocity.x) / T);

    coefficients.c2.y =
        (3.0 * position_delta.y / (T * T))
        -
        ((2.0 * initial_state.velocity.y
          + desired_state.velocity.y) / T);

    coefficients.c3.x =
        (-2.0 * position_delta.x / (T * T * T))
        +
        ((initial_state.velocity.x
          + desired_state.velocity.x)
         / (T * T));

    coefficients.c3.y =
        (-2.0 * position_delta.y / (T * T * T))
        +
        ((initial_state.velocity.y
          + desired_state.velocity.y)
         / (T * T));

    return coefficients;
}

Vector2D calculate_reference_position(
    State initial_state,
    TrajectoryCoefficients coefficients,
    double reference_time
)
{
    Vector2D c2 = coefficients.c2;
    Vector2D c3 = coefficients.c3;

    double t = reference_time;

    Vector2D reference_position;

    reference_position.x =
        initial_state.position.x
        + initial_state.velocity.x * t
        + c2.x * t * t
        + c3.x * t * t * t;

    reference_position.y =
        initial_state.position.y
        + initial_state.velocity.y * t
        + c2.y * t * t
        + c3.y * t * t * t;

    return reference_position;
}

Vector2D calculate_reference_velocity(
    State initial_state,
    TrajectoryCoefficients coefficients,
    double reference_time
)
{
    Vector2D c2 = coefficients.c2;
    Vector2D c3 = coefficients.c3;

    double t = reference_time;

    Vector2D reference_velocity;

    reference_velocity.x =
        initial_state.velocity.x
        + 2.0 * c2.x * t
        + 3.0 * c3.x * t * t;

    reference_velocity.y =
        initial_state.velocity.x
        + 2.0 * c2.y * t
        + 3.0 * c3.y * t * t;

    return reference_velocity;
}

// --------------------------------------------------------
// BOUNDARY CONDITIONS
// --------------------------------------------------------
std::string validate_boundary_conditions(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    double T =
        sim_config.maneuver.maneuver_duration;

    if (T <= 0.0)
    {
        return "N/A";
    }

    TrajectoryCoefficients coefficients =
        calculate_trajectory_coefficients(
            sim_config.maneuver.initial_state,
            sim_config.maneuver.desired_state,
            T
        );

    Vector2D c2 = coefficients.c2;
    Vector2D c3 = coefficients.c3;

    State initial_state =
        sim_config.maneuver.initial_state;

    State desired_state =
        sim_config.maneuver.desired_state;

    // ----------------------------------------------------
    // Evaluate trajectory at t = 0
    // ----------------------------------------------------

    double t0 = 0.0;

    Vector2D reference_position_0 = 
        calculate_reference_position(
            initial_state,
            coefficients,
            t0
        );

    Vector2D reference_velocity_0 = 
        calculate_reference_velocity(
            initial_state,
            coefficients,
            t0
        );

    double position_error_0 =
        calculate_magnitude(
            subtract_vectors(
                reference_position_0,
                initial_state.position
            )
        );

    double velocity_error_0 =
        calculate_magnitude(
            subtract_vectors(
                reference_velocity_0,
                initial_state.velocity
            )
        );

    // ----------------------------------------------------
    // Evaluate trajectory at t = T
    // ----------------------------------------------------

    double tT = T;

    Vector2D reference_position_T = 
        calculate_reference_position(
            initial_state,
            coefficients,
            tT
        );

    Vector2D reference_velocity_T = 
        calculate_reference_velocity(
            initial_state,
            coefficients,
            tT
        );

    double position_error_T =
        calculate_magnitude(
            subtract_vectors(
                reference_position_T,
                desired_state.position
            )
        );

    double velocity_error_T =
        calculate_magnitude(
            subtract_vectors(
                reference_velocity_T,
                desired_state.velocity
            )
        );

    bool position_passed =
        position_error_0 <=
            val_config.boundary_position_tolerance
        &&
        position_error_T <=
            val_config.boundary_position_tolerance;

    bool velocity_passed =
        velocity_error_0 <=
            val_config.boundary_velocity_tolerance
        &&
        velocity_error_T <=
            val_config.boundary_velocity_tolerance;

    // ----------------------------------------------------
    // Output
    // ----------------------------------------------------

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "BOUNDARY CONDITIONS VALIDATION\n"
        << "============================================================\n";

    std::cout << "At t = 0:\n";

    std::cout
        << "     Position error: "
        << position_error_0
        << " m\n";

    std::cout
        << "     Position tolerance: "
        << val_config.boundary_position_tolerance
        << " m\n";

    std::cout
        << "     Velocity error: "
        << velocity_error_0
        << " m/s\n";

    std::cout
        << "     Velocity tolerance: "
        << val_config.boundary_velocity_tolerance
        << " m/s\n";

    std::cout << "\nAt t = T:\n";

    std::cout
        << "     Position error: "
        << position_error_T
        << " m\n";

    std::cout
        << "     Position tolerance: "
        << val_config.boundary_position_tolerance
        << " m\n";

    std::cout
        << "     Velocity error: "
        << velocity_error_T
        << " m/s\n";

    std::cout
        << "     Velocity tolerance: "
        << val_config.boundary_velocity_tolerance
        << " m/s\n";

    std::cout
        << "\nPosition boundary result: "
        << (position_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Velocity boundary result: "
        << (velocity_passed ? "PASS" : "FAIL")
        << "\n";

    if (position_passed && velocity_passed)
    {
        return "PASS";
    }

    return "FAIL";
}

// --------------------------------------------------------
// INTERMEDIATE TRAJECTORY
// --------------------------------------------------------
double calculate_position_trajectory_error(
    State initial_state,
    SimulationStep simulation_step,
    TrajectoryCoefficients coefficients
)
{
    double t =
        simulation_step.time;

    Vector2D expected_position;

    expected_position.x =
        initial_state.position.x
        + initial_state.velocity.x * t
        + coefficients.c2.x * t * t
        + coefficients.c3.x * t * t * t;

    expected_position.y =
        initial_state.position.y
        + initial_state.velocity.y * t
        + coefficients.c2.y * t * t
        + coefficients.c3.y * t * t * t;

    return calculate_magnitude(
        subtract_vectors(
            simulation_step.state.position,
            expected_position
        )
    );
}

double calculate_velocity_trajectory_error(
    State initial_state,
    SimulationStep simulation_step,
    TrajectoryCoefficients coefficients
)
{
    double t =
        simulation_step.time;

    Vector2D expected_velocity;

    expected_velocity.x =
        initial_state.velocity.x
        + 2.0 * coefficients.c2.x * t
        + 3.0 * coefficients.c3.x * t * t;

    expected_velocity.y =
        initial_state.velocity.y
        + 2.0 * coefficients.c2.y * t
        + 3.0 * coefficients.c3.y * t * t;

    return calculate_magnitude(
        subtract_vectors(
            simulation_step.state.velocity,
            expected_velocity
        )
    );
}

std::string validate_intermediate_trajectory(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    double T =
        sim_config.maneuver.maneuver_duration;

    if (T <= 0.0)
    {
        return "N/A";
    }

    TrajectoryCoefficients coefficients =
        calculate_trajectory_coefficients(
            sim_config.maneuver.initial_state,
            sim_config.maneuver.desired_state,
            T
        );

    double max_position_error = 0.0;
    double max_position_error_time = 0.0;

    double max_velocity_error = 0.0;
    double max_velocity_error_time = 0.0;

    for (const SimulationStep& step : results)
    {
        double position_error =
            calculate_position_trajectory_error(
                sim_config.maneuver.initial_state,
                step,
                coefficients
            );

        if (position_error > max_position_error)
        {
            max_position_error = position_error;
            max_position_error_time = step.time;
        }

        double velocity_error =
            calculate_velocity_trajectory_error(
                sim_config.maneuver.initial_state,
                step,
                coefficients
            );

        if (velocity_error > max_velocity_error)
        {
            max_velocity_error = velocity_error;
            max_velocity_error_time = step.time;
        }
    }

    bool position_passed =
        max_position_error <=
        val_config.intermediate_position_tolerance;

    bool velocity_passed =
        max_velocity_error <=
        val_config.intermediate_velocity_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "INTERMEDIATE TRAJECTORY VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Maximum position tracking error: "
        << max_position_error
        << " m\n";

    std::cout
        << "Occurred at: "
        << max_position_error_time
        << " s\n";

    std::cout
        << "Position tolerance: "
        << val_config.intermediate_position_tolerance
        << " m\n";

    std::cout
        << "Result: "
        << (position_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "\nMaximum velocity tracking error: "
        << max_velocity_error
        << " m/s\n";

    std::cout
        << "Occurred at: "
        << max_velocity_error_time
        << " s\n";

    std::cout
        << "Velocity tolerance: "
        << val_config.intermediate_velocity_tolerance
        << " m/s\n";

    std::cout
        << "Result: "
        << (velocity_passed ? "PASS" : "FAIL")
        << "\n";

    if (position_passed && velocity_passed)
    {
        return "PASS";
    }

    return "FAIL";
}

// --------------------------------------------------------
// ACCELERATION CONSTRAINT
// --------------------------------------------------------
std::string validate_acceleration_constraint(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    double maximum_acceleration = 0.0;

    for (const SimulationStep& step : results)
    {
        double acceleration =
            calculate_magnitude(
                step.thrust_acceleration
            );

        if (acceleration > maximum_acceleration)
        {
            maximum_acceleration = acceleration;
        }
    }

    double constraint =
        sim_config.control_config.acceleration_constraint;

    bool passed =
        maximum_acceleration <=
        constraint + val_config.acceleration_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "ACCELERATION CONSTRAINT VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Maximum commanded thrust acceleration: "
        << maximum_acceleration
        << " m/s^2\n";

    std::cout
        << "Acceleration constraint: "
        << constraint
        << " m/s^2\n";

    std::cout
        << "Acceleration tolerance: "
        << val_config.acceleration_tolerance
        << " m/s^2\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// COMMAND DURATION
// --------------------------------------------------------
std::string validate_command_duration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.size() < 2)
    {
        return "N/A";
    }

    bool passed = true;

    double maximum_duration_error = 0.0;

    for (std::size_t i = 1; i < results.size(); ++i)
    {
        double expected_duration =
            results[i].time -
            results[i - 1].time;

        double actual_duration =
            results[i].command_duration;

        double duration_error =
            std::abs(
                actual_duration -
                expected_duration
            );

        if (duration_error >
            maximum_duration_error)
        {
            maximum_duration_error =
                duration_error;
        }

        if (duration_error >
            val_config.command_duration_tolerance)
        {
            passed = false;
        }
    }

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "COMMAND DURATION VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Maximum command duration error: "
        << maximum_duration_error
        << " s\n";

    std::cout
        << "Command duration tolerance: "
        << val_config.command_duration_tolerance
        << " s\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// FINAL POSITION
// --------------------------------------------------------
std::string validate_final_position(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    Vector2D desired_position =
        sim_config.maneuver.desired_state.position;

    Vector2D final_position =
        results.back().state.position;

    double position_error =
        calculate_displacement(
            desired_position,
            final_position
        );

    bool passed =
        position_error <=
        val_config.final_position_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "FINAL POSITION VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Desired final position: ("
        << desired_position.x
        << ", "
        << desired_position.y
        << ")\n";

    std::cout
        << "Simulated final position: ("
        << final_position.x
        << ", "
        << final_position.y
        << ")\n";

    std::cout
        << "Position error: "
        << position_error
        << " m\n";

    std::cout
        << "Position tolerance: "
        << val_config.final_position_tolerance
        << " m\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// FINAL VELOCITY
// --------------------------------------------------------
std::string validate_final_velocity(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    Vector2D desired_velocity =
        sim_config.maneuver.desired_state.velocity;

    Vector2D final_velocity =
        results.back().state.velocity;

    double velocity_error =
        calculate_displacement(
            desired_velocity,
            final_velocity
        );

    bool passed =
        velocity_error <=
        val_config.final_velocity_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "FINAL VELOCITY VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Desired final velocity: ("
        << desired_velocity.x
        << ", "
        << desired_velocity.y
        << ")\n";

    std::cout
        << "Simulated final velocity: ("
        << final_velocity.x
        << ", "
        << final_velocity.y
        << ")\n";

    std::cout
        << "Velocity error: "
        << velocity_error
        << " m/s\n";

    std::cout
        << "Velocity tolerance: "
        << val_config.final_velocity_tolerance
        << " m/s\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// MANEUVER DURATION
// --------------------------------------------------------
std::string validate_maneuver_duration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    double actual_duration =
        results.back().time;

    double expected_duration =
        sim_config.maneuver.maneuver_duration;

    double duration_error =
        std::abs(
            actual_duration -
            expected_duration
        );

    bool passed =
        duration_error <=
        val_config.maneuver_duration_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "MANEUVER DURATION VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Final simulation time: "
        << actual_duration
        << " s\n";

    std::cout
        << "Requested maneuver duration: "
        << expected_duration
        << " s\n";

    std::cout
        << "Duration error: "
        << duration_error
        << " s\n";

    std::cout
        << "Duration tolerance: "
        << val_config.maneuver_duration_tolerance
        << " s\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// SIMULATED TRAJECTORY INTEGRITY
// --------------------------------------------------------
std::string validate_simulated_trajectory(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    (void)val_config;

    if (results.empty())
    {
        return "N/A";
    }

    bool passed = true;

    double previous_time =
        results.front().time;

    for (const SimulationStep& step : results)
    {
        // Time must never move backward.
        if (step.time < previous_time)
        {
            passed = false;
        }

        // State must contain finite values.
        if (!std::isfinite(step.state.position.x) ||
            !std::isfinite(step.state.position.y) ||
            !std::isfinite(step.state.velocity.x) ||
            !std::isfinite(step.state.velocity.y))
        {
            passed = false;
        }

        // Recorded time cannot exceed requested duration.
        if (step.time >
            sim_config.maneuver.maneuver_duration +
            val_config.maneuver_duration_tolerance)
        {
            passed = false;
        }

        previous_time = step.time;
    }

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "SIMULATED TRAJECTORY INTEGRITY VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Recorded simulation steps: "
        << results.size()
        << "\n";

    std::cout
        << "Time ordering: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "State values finite: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    return passed ? "PASS" : "FAIL";
}

// --------------------------------------------------------
// GNC VALIDATION SUMMARY
// --------------------------------------------------------
void gnc_validate_simulation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const GNCValidationConfig& val_config
)
{
    std::string boundary_conditions_result =
        validate_boundary_conditions(
            results,
            sim_config,
            val_config
        );

    std::string intermediate_trajectory_result =
        validate_intermediate_trajectory(
            results,
            sim_config,
            val_config
        );

    std::string acceleration_constraint_result =
        validate_acceleration_constraint(
            results,
            sim_config,
            val_config
        );

    std::string command_duration_result =
        validate_command_duration(
            results,
            sim_config,
            val_config
        );

    std::string final_position_result =
        validate_final_position(
            results,
            sim_config,
            val_config
        );

    std::string final_velocity_result =
        validate_final_velocity(
            results,
            sim_config,
            val_config
        );

    std::string maneuver_duration_result =
        validate_maneuver_duration(
            results,
            sim_config,
            val_config
        );

    std::string simulated_trajectory_result =
        validate_simulated_trajectory(
            results,
            sim_config,
            val_config
        );

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "GNC VALIDATION RESULTS\n"
        << "============================================================\n";

    std::cout
        << "Boundary conditions:           "
        << boundary_conditions_result
        << "\n";

    std::cout
        << "Intermediate trajectory:       "
        << intermediate_trajectory_result
        << "\n";

    std::cout
        << "Acceleration constraint:       "
        << acceleration_constraint_result
        << "\n";

    std::cout
        << "Command duration:              "
        << command_duration_result
        << "\n";

    std::cout
        << "Final position:                "
        << final_position_result
        << "\n";

    std::cout
        << "Final velocity:                "
        << final_velocity_result
        << "\n";

    std::cout
        << "Maneuver duration:             "
        << maneuver_duration_result
        << "\n";

    std::cout
        << "Simulation integrity:          "
        << simulated_trajectory_result
        << "\n";
}