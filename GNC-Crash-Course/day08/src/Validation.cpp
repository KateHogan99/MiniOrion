#include "Validation.h"

#include "OrbitalMechanics.h"

#include <algorithm>
#include <cmath>
#include <iostream>

// --------------------------------------------------------
// RELATIVE ERROR
// --------------------------------------------------------
double calculate_relative_error_percent(
    double error,
    double reference
)
{
    double reference_magnitude =
        std::abs(reference);

    if (reference_magnitude == 0.0)
    {
        return 0.0;
    }

    return
        std::abs(error) /
        reference_magnitude *
        100.0;
}


// --------------------------------------------------------
// CONSTANT ACCELERATION
// --------------------------------------------------------
State calculate_analytical_state(
    State initial_state,
    Vector2D acceleration,
    double time
)
{
    // Constant-acceleration analytical solution:
    //
    // r(t) = r0 + v0*t + 1/2*a*t^2
    //
    // This is NOT used for propagation.
    // It exists to validate the numerical integrators.

    State analytical_state;

    analytical_state.position =
        add_vectors(
            add_vectors(
                initial_state.position,
                scale_vector(
                    initial_state.velocity,
                    time
                )
            ),
            scale_vector(
                acceleration,
                0.5 * time * time
            )
        );

    analytical_state.velocity =
        add_vectors(
            initial_state.velocity,
            scale_vector(
                acceleration,
                time
            )
        );

    return analytical_state;
}


std::string validate_constant_acceleration(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    if (sim_config.mu != 0.0)
    {
        return "N/A";
    }

    if (results.empty())
    {
        return "N/A";
    }

    State initial_state =
        results.front().state;

    State analytical_state =
        calculate_analytical_state(
            initial_state,
            sim_config.thrust_acceleration,
            results.back().time
        );

    State numerical_state =
        results.back().state;

    double position_error =
        calculate_displacement(
            analytical_state.position,
            numerical_state.position
        );

    double velocity_error =
        calculate_displacement(
            analytical_state.velocity,
            numerical_state.velocity
        );

    double analytical_position_magnitude =
        calculate_magnitude(
            analytical_state.position
        );

    double analytical_velocity_magnitude =
        calculate_magnitude(
            analytical_state.velocity
        );

    double position_error_percent =
        calculate_relative_error_percent(
            position_error,
            analytical_position_magnitude
        );

    double velocity_error_percent =
        calculate_relative_error_percent(
            velocity_error,
            analytical_velocity_magnitude
        );

    bool position_passed =
        position_error_percent <=
        val_config.constant_acceleration_tolerance_position_percent;

    bool velocity_passed =
        velocity_error_percent <=
        val_config.constant_acceleration_tolerance_velocity_percent;

    bool passed =
        position_passed &&
        velocity_passed;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "ANALYTICAL VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Analytical final position: ("
        << analytical_state.position.x
        << ", "
        << analytical_state.position.y
        << ")\n";

    std::cout
        << "Numerical final position:  ("
        << numerical_state.position.x
        << ", "
        << numerical_state.position.y
        << ")\n";

    std::cout
        << "Position error: "
        << position_error
        << "\n";

    std::cout
        << "Position relative error: "
        << position_error_percent
        << "%\n";

    std::cout
        << "Position tolerance: "
        << val_config.constant_acceleration_tolerance_position_percent
        << "%\n";

    std::cout
        << "Position result: "
        << (position_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Velocity error: "
        << velocity_error
        << "\n";

    std::cout
        << "Velocity relative error: "
        << velocity_error_percent
        << "%\n";

    std::cout
        << "Velocity tolerance: "
        << val_config.constant_acceleration_tolerance_velocity_percent
        << "%\n";

    std::cout
        << "Velocity result: "
        << (velocity_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Overall result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// CIRCULAR ORBIT
// --------------------------------------------------------
std::string validate_circular_orbit(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    double thrust_magnitude =
        calculate_magnitude(
            sim_config.thrust_acceleration
        );

    if (thrust_magnitude != 0.0)
    {
        return "N/A";
    }

    if (sim_config.mu == 0.0)
    {
        return "N/A";
    }

    if (results.empty())
    {
        return "N/A";
    }

    double initial_radius =
        calculate_magnitude(
            sim_config.initial_position
        );

    if (initial_radius == 0.0)
    {
        return "N/A";
    }

    double initial_speed =
        calculate_magnitude(
            sim_config.initial_velocity
        );

    double circular_speed =
        std::sqrt(
            sim_config.mu /
            initial_radius
        );

    double speed_error =
        std::abs(
            initial_speed -
            circular_speed
        );

    double speed_error_percent =
        calculate_relative_error_percent(
            speed_error,
            circular_speed
        );

    if (speed_error_percent >
        val_config.circular_orbit_speed_tolerance_percent)
    {
        return "N/A";
    }

    auto [min_it, max_it] =
        std::minmax_element(
            results.begin(),
            results.end(),
            [](const SimulationStep& a,
               const SimulationStep& b)
            {
                return a.radius < b.radius;
            }
        );

    double largest_deviation =
        std::max(
            std::abs(
                min_it->radius -
                initial_radius
            ),
            std::abs(
                max_it->radius -
                initial_radius
            )
        );

    double radius_error_percent =
        calculate_relative_error_percent(
            largest_deviation,
            initial_radius
        );

    bool passed =
        radius_error_percent <=
        val_config.circular_orbit_tolerance_percent;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "CIRCULAR ORBIT VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Initial radius: "
        << initial_radius
        << "\n";

    std::cout
        << "Minimum radius: "
        << min_it->radius
        << "\n";

    std::cout
        << "Maximum radius: "
        << max_it->radius
        << "\n";

    std::cout
        << "Largest radius deviation: "
        << largest_deviation
        << "\n";

    std::cout
        << "Relative radius error: "
        << radius_error_percent
        << "%\n";

    std::cout
        << "Tolerance: "
        << val_config.circular_orbit_tolerance_percent
        << "%\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// CONSERVATION OF ORBITAL ENERGY
// --------------------------------------------------------
std::string validate_orbital_conservation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    double thrust_magnitude =
        calculate_magnitude(
            sim_config.thrust_acceleration
        );

    if (thrust_magnitude != 0.0)
    {
        return "N/A";
    }

    if (sim_config.mu == 0.0)
    {
        return "N/A";
    }

    if (results.empty())
    {
        return "N/A";
    }

    double initial_orbital_energy =
        results.front().orbital_energy;

    auto [min_it, max_it] =
        std::minmax_element(
            results.begin(),
            results.end(),
            [](const SimulationStep& a,
               const SimulationStep& b)
            {
                return a.orbital_energy <
                       b.orbital_energy;
            }
        );

    double largest_deviation =
        std::max(
            std::abs(
                min_it->orbital_energy -
                initial_orbital_energy
            ),
            std::abs(
                max_it->orbital_energy -
                initial_orbital_energy
            )
        );

    double energy_error_percent =
        calculate_relative_error_percent(
            largest_deviation,
            initial_orbital_energy
        );

    bool passed =
        energy_error_percent <=
        val_config.orbital_energy_tolerance_percent;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "CONSERVATION OF ORBITAL ENERGY VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Initial orbital energy: "
        << initial_orbital_energy
        << "\n";

    std::cout
        << "Minimum orbital energy: "
        << min_it->orbital_energy
        << "\n";

    std::cout
        << "Maximum orbital energy: "
        << max_it->orbital_energy
        << "\n";

    std::cout
        << "Largest orbital energy deviation: "
        << largest_deviation
        << "\n";

    std::cout
        << "Relative energy error: "
        << energy_error_percent
        << "%\n";

    std::cout
        << "Tolerance: "
        << val_config.orbital_energy_tolerance_percent
        << "%\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// CONSERVATION OF ANGULAR MOMENTUM
// --------------------------------------------------------
std::string validate_angular_conservation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    double thrust_magnitude =
        calculate_magnitude(
            sim_config.thrust_acceleration
        );

    if (thrust_magnitude != 0.0)
    {
        return "N/A";
    }

    if (sim_config.mu == 0.0)
    {
        return "N/A";
    }

    if (results.empty())
    {
        return "N/A";
    }

    double initial_angular_momentum =
        results.front().angular_momentum;

    auto [min_it, max_it] =
        std::minmax_element(
            results.begin(),
            results.end(),
            [](const SimulationStep& a,
               const SimulationStep& b)
            {
                return a.angular_momentum <
                       b.angular_momentum;
            }
        );

    double largest_deviation =
        std::max(
            std::abs(
                min_it->angular_momentum -
                initial_angular_momentum
            ),
            std::abs(
                max_it->angular_momentum -
                initial_angular_momentum
            )
        );

    double angular_momentum_error_percent =
        calculate_relative_error_percent(
            largest_deviation,
            initial_angular_momentum
        );

    bool passed =
        angular_momentum_error_percent <=
        val_config.angular_momentum_tolerance_percent;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "CONSERVATION OF ANGULAR MOMENTUM VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Initial angular momentum: "
        << initial_angular_momentum
        << "\n";

    std::cout
        << "Minimum angular momentum: "
        << min_it->angular_momentum
        << "\n";

    std::cout
        << "Maximum angular momentum: "
        << max_it->angular_momentum
        << "\n";

    std::cout
        << "Largest angular momentum deviation: "
        << largest_deviation
        << "\n";

    std::cout
        << "Relative angular momentum error: "
        << angular_momentum_error_percent
        << "%\n";

    std::cout
        << "Tolerance: "
        << val_config.angular_momentum_tolerance_percent
        << "%\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// ELLIPTICAL ORBIT
// --------------------------------------------------------
std::string validate_elliptical_orbit(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    if (sim_config.mu == 0.0)
    {
        return "N/A";
    }

    double thrust_magnitude =
        calculate_magnitude(
            sim_config.thrust_acceleration
        );

    if (thrust_magnitude != 0.0)
    {
        return "N/A";
    }

    double initial_energy =
        results.front().orbital_energy;

    if (initial_energy >= 0.0)
    {
        return "N/A";
    }

    double initial_eccentricity =
        calculate_eccentricity(
            results.front().state,
            sim_config.mu
        );

    if (initial_eccentricity < 0.0 ||
        initial_eccentricity >= 1.0)
    {
        return "N/A";
    }

    double final_eccentricity =
        calculate_eccentricity(
            results.back().state,
            sim_config.mu
        );

    double eccentricity_deviation =
        std::abs(
            final_eccentricity -
            initial_eccentricity
        );

    bool passed =
        eccentricity_deviation <=
        val_config.elliptical_orbit_tolerance;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "ELLIPTICAL ORBIT VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Initial eccentricity: "
        << initial_eccentricity
        << "\n";

    std::cout
        << "Final eccentricity: "
        << final_eccentricity
        << "\n";

    std::cout
        << "Eccentricity deviation: "
        << eccentricity_deviation
        << "\n";

    std::cout
        << "Tolerance: "
        << val_config.elliptical_orbit_tolerance
        << "\n";

    std::cout
        << "Result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// ESCAPE TRAJECTORY
// --------------------------------------------------------
std::string validate_escape_trajectory(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    if (results.empty())
    {
        return "N/A";
    }

    if (sim_config.mu == 0.0)
    {
        return "N/A";
    }

    double thrust_magnitude =
        calculate_magnitude(
            sim_config.thrust_acceleration
        );

    if (thrust_magnitude != 0.0)
    {
        return "N/A";
    }

    double initial_energy =
        results.front().orbital_energy;

    if (initial_energy <= 0.0)
    {
        return "N/A";
    }

    double initial_eccentricity =
        calculate_eccentricity(
            results.front().state,
            sim_config.mu
        );

    if (initial_eccentricity <= 1.0)
    {
        return "N/A";
    }

    double initial_radius =
        results.front().radius;

    double final_radius =
        results.back().radius;

    if (final_radius == 0.0)
    {
        return "N/A";
    }

    bool radius_passed =
        final_radius >
        initial_radius;

    Vector2D final_position =
        results.back().state.position;

    Vector2D final_velocity =
        results.back().state.velocity;

    double final_radial_velocity =
        (
            final_position.x *
            final_velocity.x
            +
            final_position.y *
            final_velocity.y
        ) / final_radius;

    bool velocity_passed =
        final_radial_velocity > 0.0;

    bool passed =
        radius_passed &&
        velocity_passed;

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "ESCAPE TRAJECTORY VALIDATION\n"
        << "============================================================\n";

    std::cout
        << "Initial radius: "
        << initial_radius
        << "\n";

    std::cout
        << "Final radius: "
        << final_radius
        << "\n";

    std::cout
        << "Radius result: "
        << (radius_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Final radial velocity: "
        << final_radial_velocity
        << "\n";

    std::cout
        << "Velocity result: "
        << (velocity_passed ? "PASS" : "FAIL")
        << "\n";

    std::cout
        << "Overall result: "
        << (passed ? "PASS" : "FAIL")
        << "\n";

    if (passed)
    {
        return "PASS";
    }

    return "FAIL";
}


// --------------------------------------------------------
// VALIDATION SUMMARY
// --------------------------------------------------------
void validate_simulation(
    const std::vector<SimulationStep>& results,
    const SimulationConfig& sim_config,
    const ValidationConfig& val_config
)
{
    std::string constant_acceleration_result =
        validate_constant_acceleration(
            results,
            sim_config,
            val_config
        );

    std::string circular_orbit_result =
        validate_circular_orbit(
            results,
            sim_config,
            val_config
        );

    std::string orbital_conservation_result =
        validate_orbital_conservation(
            results,
            sim_config,
            val_config
        );

    std::string angular_conservation_result =
        validate_angular_conservation(
            results,
            sim_config,
            val_config
        );

    std::string elliptical_orbit_result =
        validate_elliptical_orbit(
            results,
            sim_config,
            val_config
        );

    std::string escape_trajectory_result =
        validate_escape_trajectory(
            results,
            sim_config,
            val_config
        );

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "VALIDATION RESULTS\n"
        << "============================================================\n";

    std::cout
        << "Constant acceleration:         "
        << constant_acceleration_result
        << "\n";

    std::cout
        << "Circular orbit:                "
        << circular_orbit_result
        << "\n";

    std::cout
        << "Orbital energy conservation:   "
        << orbital_conservation_result
        << "\n";

    std::cout
        << "Angular momentum conservation: "
        << angular_conservation_result
        << "\n";

    std::cout
        << "Elliptical orbit:              "
        << elliptical_orbit_result
        << "\n";

    std::cout
        << "Escape trajectory:             "
        << escape_trajectory_result
        << "\n";
}