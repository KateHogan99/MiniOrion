/**
 * @file Simulation.h
 * @brief Defines spacecraft simulation configuration and execution.
 *
 * The Simulation module orchestrates guidance, control, dynamics,
 * numerical integration, and result collection.
 *
 * It does not implement the underlying physics or control algorithms.
 * Instead, it coordinates the specialized modules that perform those
 * calculations.
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Control.h"
#include "Guidance.h"
#include "Integrator.h"
#include "OrbitalMechanics.h"
#include "State.h"
#include "Vector2D.h"

#include <vector>

/**
 * @brief Defines the configuration of a spacecraft simulation.
 */
struct SimulationConfig
{
    /**
     * @brief Standard gravitational parameter of the central body.
     *
     * Units: m^3/s^2.
     */
    double mu;

    /**
     * @brief Initial spacecraft state.
     */
    State initial_state;

    /**
     * @brief Constant thrust acceleration for an unguided simulation.
     *
     * Units: m/s^2.
     */
    Vector2D thrust_acceleration;

    /**
     * @brief Simulation timestep.
     *
     * Units: seconds (s).
     */
    double time_step;

    /**
     * @brief Number of integration steps.
     */
    int number_of_steps;

    /**
     * @brief Numerical integration method.
     */
    Integrator integrator;
};

/**
 * @brief Stores the results and derived quantities for one simulation step.
 */
struct SimulationStep
{
    /**
     * @brief Simulation time.
     *
     * Units: seconds (s).
     */
    double time;

    /**
     * @brief Spacecraft state at this timestep.
     */
    State state;

    /**
     * @brief Gravitational acceleration.
     *
     * Units: m/s^2.
     */
    Vector2D gravity_acceleration;

    /**
     * @brief Applied thrust acceleration.
     *
     * Units: m/s^2.
     */
    Vector2D thrust_acceleration;

    /**
     * @brief Total acceleration.
     *
     * Units: m/s^2.
     */
    Vector2D total_acceleration;

    /**
     * @brief Distance from the central body.
     *
     * Units: meters (m).
     */
    double radius;

    /**
     * @brief Spacecraft speed.
     *
     * Units: m/s.
     */
    double speed;

    /**
     * @brief Specific orbital energy.
     *
     * Units: J/kg.
     */
    double orbital_energy;

    /**
     * @brief Specific angular momentum.
     *
     * Units: m^2/s.
     */
    double angular_momentum;

    /**
     * @brief Distance traveled during this timestep.
     *
     * Units: meters (m).
     */
    double distance_this_step;

    /**
     * @brief Total distance traveled since simulation start.
     *
     * Units: meters (m).
     */
    double total_distance;

    /**
     * @brief Duration of the applied control command.
     *
     * Units: seconds (s).
     */
    double command_duration;
};

/**
 * @brief Calculates the magnitude of displacement between two positions.
 *
 * @param initial_position Initial position.
 * @param final_position Final position.
 *
 * @return Displacement magnitude.
 *
 * Units: meters (m).
 */
double calculate_displacement(
    Vector2D initial_position,
    Vector2D final_position
);

/**
 * @brief Calculates the diagnostic values for one simulation state.
 *
 * @param state Current spacecraft state.
 * @param time Current simulation time.
 * @param distance_this_step Distance traveled during this step.
 * @param total_distance Total distance traveled so far.
 * @param command_duration Duration of the current thrust command.
 * @param config Simulation configuration.
 *
 * @return Populated simulation-step result.
 */
SimulationStep calculate_simulation_step(
    State state,
    double time,
    double distance_this_step,
    double total_distance,
    double command_duration,
    SimulationConfig config
);

/**
 * @brief Propagates the spacecraft state by one simulation timestep.
 *
 * @param state Current spacecraft state.
 * @param thrust_acceleration Applied thrust acceleration.
 * @param config Simulation configuration.
 *
 * @return State after one integration step.
 */
State propagate_state(
    State state,
    Vector2D thrust_acceleration,
    SimulationConfig config
);

/**
 * @brief Runs an unguided spacecraft simulation.
 *
 * Uses the constant thrust acceleration specified in SimulationConfig.
 *
 * @param config Simulation configuration.
 *
 * @return Simulation results for every recorded timestep.
 */
std::vector<SimulationStep> run_simulation(
    SimulationConfig config
);

/**
 * @brief Runs a closed-loop guided maneuver.
 *
 * At each timestep:
 *
 *     1. Guidance generates the reference trajectory.
 *     2. Control calculates tracking errors and generates a command.
 *     3. Dynamics evaluates the equations of motion.
 *     4. Integrator propagates the spacecraft state.
 *
 * @param maneuver Maneuver boundary conditions.
 * @param control_config Control configuration and constraints.
 * @param simulation_config Simulation configuration.
 *
 * @return Simulation results for every recorded timestep.
 */
std::vector<SimulationStep> run_guided_maneuver(
    ManeuverRequest maneuver,
    ControlConfig control_config,
    SimulationConfig simulation_config
);

#endif