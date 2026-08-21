#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <string>

// ============================================================
// DATA STRUCTURES
// ============================================================

struct Vector2D
{
    double x;
    double y;
};

struct State
{
    Vector2D position;
    Vector2D velocity;
};

struct StateDerivative
{
    Vector2D position_derivative;
    Vector2D velocity_derivative;
};

enum class Integrator
{
    Euler,
    RK4
};

struct SimulationConfig
{
    double mu;

    Vector2D initial_position;
    Vector2D initial_velocity;
    Vector2D thrust_acceleration;

    double time_step;
    int number_of_steps;

    Integrator integrator;
};

struct SimulationStep
{
    double time;

    State state;

    Vector2D gravity_acceleration;
    Vector2D thrust_acceleration;
    Vector2D total_acceleration;

    double radius;
    double speed;
    double orbital_energy;
    double angular_momentum;

    double distance_this_step;
    double total_distance;
};

// ============================================================
// VECTOR FUNCTIONS
// ============================================================

Vector2D add_vectors(Vector2D vector_a, Vector2D vector_b)
{
    return {
        vector_a.x + vector_b.x,
        vector_a.y + vector_b.y
    };
}

Vector2D subtract_vectors(Vector2D vector_a, Vector2D vector_b)
{
    return {
        vector_a.x - vector_b.x,
        vector_a.y - vector_b.y
    };
}

Vector2D scale_vector(Vector2D vector, double scale)
{
    return {
        vector.x * scale,
        vector.y * scale
    };
}

double calculate_magnitude(Vector2D vector)
{
    return std::sqrt(
        vector.x * vector.x +
        vector.y * vector.y
    );
}

// ============================================================
// STATE FUNCTIONS
// ============================================================

State apply_state_derivative(State state, StateDerivative derivative, double d_time)
{
    return {
        add_vectors(
            state.position,
            scale_vector(
                derivative.position_derivative,
                d_time
            )
        ),

        add_vectors(
            state.velocity,
            scale_vector(
                derivative.velocity_derivative,
                d_time
            )
        )
    };
}

// ============================================================
// DYNAMICS
// ============================================================

Vector2D calculate_gravity_acceleration(Vector2D position, double mu)
{
    if (mu == 0.0)
    {
        return {0.0, 0.0};
    }

    double radius = calculate_magnitude(position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate gravity: spacecraft is at the origin."
        );
    }

    return scale_vector(
        position,
        -mu / (radius * radius * radius)
    );
}

StateDerivative calculate_state_derivative(State spacecraft, Vector2D thrust_acceleration, double mu)
{
    Vector2D gravity_acceleration =
        calculate_gravity_acceleration(
            spacecraft.position,
            mu
        );

    Vector2D total_acceleration =
        add_vectors(
            gravity_acceleration,
            thrust_acceleration
        );

    return {
        spacecraft.velocity,
        total_acceleration
    };
}

// ============================================================
// ORBITAL MECHANICS
// ============================================================

// Specific orbital mechanical energy:
// epsilon = v^2 / 2 - mu / r
// Units: energy per unit mass.
// For an ideal two-body orbit with no thrust,
// specific orbital energy should remain constant.

double calculate_specific_orbital_energy(State spacecraft, double mu)
{
    double radius = calculate_magnitude(spacecraft.position);

    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate orbital energy: "
            "spacecraft is at the origin."
        );
    }

    double speed = calculate_magnitude(spacecraft.velocity);

    return
        (speed * speed / 2.0) -
        (mu / radius);
}

// Specific angular momentum:
// h = r x v
// In 2D, only the z-component exists:
// h_z = x * v_y - y * v_x
// This is a scalar.

double calculate_specific_angular_momentum(State spacecraft)
{
    return
        spacecraft.position.x * spacecraft.velocity.y -
        spacecraft.position.y * spacecraft.velocity.x;
}

// ============================================================
// ANALYTICAL VALIDATION
// ============================================================

// Constant-acceleration analytical solution:
// r(t) = r0 + v0*t + 1/2*a*t^2
// This is NOT used for propagation.
// It exists to validate the numerical integrators.

State calculate_analytical_state(State initial_state, Vector2D acceleration, double time)
{
    Vector2D position =
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

    Vector2D velocity =
        add_vectors(
            initial_state.velocity,
            scale_vector(
                acceleration,
                time
            )
        );

    return {
        position,
        velocity
    };
}

// ============================================================
// NUMERICAL INTEGRATION
// ============================================================

State euler_integrate(State current_state, Vector2D thrust_acceleration, double mu, double d_time)
{
    StateDerivative derivative =
        calculate_state_derivative(
            current_state,
            thrust_acceleration,
            mu
        );

    return apply_state_derivative(
        current_state,
        derivative,
        d_time
    );
}

State rk4_integrate(State current_state, Vector2D thrust_acceleration, double mu, double d_time)
{
    // --------------------------------------------------------
    // k1: derivative at beginning of timestep
    // --------------------------------------------------------

    StateDerivative k1 =
        calculate_state_derivative(
            current_state,
            thrust_acceleration,
            mu
        );

    // --------------------------------------------------------
    // k2: derivative at midpoint using k1
    // --------------------------------------------------------

    State state_2 =
        apply_state_derivative(
            current_state,
            k1,
            d_time / 2.0
        );

    StateDerivative k2 =
        calculate_state_derivative(
            state_2,
            thrust_acceleration,
            mu
        );

    // --------------------------------------------------------
    // k3: derivative at midpoint using k2
    // --------------------------------------------------------

    State state_3 =
        apply_state_derivative(
            current_state,
            k2,
            d_time / 2.0
        );

    StateDerivative k3 =
        calculate_state_derivative(
            state_3,
            thrust_acceleration,
            mu
        );

    // --------------------------------------------------------
    // k4: derivative at end of timestep using k3
    // --------------------------------------------------------

    State state_4 =
        apply_state_derivative(
            current_state,
            k3,
            d_time
        );

    StateDerivative k4 =
        calculate_state_derivative(
            state_4,
            thrust_acceleration,
            mu
        );

    // --------------------------------------------------------
    // Weighted RK4 combination
    // y_next = y + dt/6 * (k1 + 2k2 + 2k3 + k4)
    // --------------------------------------------------------

    StateDerivative weighted_derivative;

    weighted_derivative.position_derivative = {
        (
            k1.position_derivative.x +
            2.0 * k2.position_derivative.x +
            2.0 * k3.position_derivative.x +
            k4.position_derivative.x
        ) / 6.0,

        (
            k1.position_derivative.y +
            2.0 * k2.position_derivative.y +
            2.0 * k3.position_derivative.y +
            k4.position_derivative.y
        ) / 6.0
    };

    weighted_derivative.velocity_derivative = {
        (
            k1.velocity_derivative.x +
            2.0 * k2.velocity_derivative.x +
            2.0 * k3.velocity_derivative.x +
            k4.velocity_derivative.x
        ) / 6.0,

        (
            k1.velocity_derivative.y +
            2.0 * k2.velocity_derivative.y +
            2.0 * k3.velocity_derivative.y +
            k4.velocity_derivative.y
        ) / 6.0
    };

    return apply_state_derivative(
        current_state,
        weighted_derivative,
        d_time
    );
}

// ============================================================
// PROPAGATION
// ============================================================

State propagate_state(State spacecraft, SimulationConfig config)
{
    switch (config.integrator)
    {
        case Integrator::Euler:
            return euler_integrate(
                spacecraft,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        case Integrator::RK4:
            return rk4_integrate(
                spacecraft,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        default:
            throw std::runtime_error(
                "Unknown integration method."
            );
    }
}

// ============================================================
// SIMULATION ANALYSIS
// ============================================================

double calculate_displacement(Vector2D initial_position, Vector2D final_position)
{
    return calculate_magnitude(
        subtract_vectors(
            final_position,
            initial_position
        )
    );
}

SimulationStep calculate_simulation_step(State state, State previous_state, double time, double total_distance, SimulationConfig config)
{
    Vector2D gravity_acceleration =
        calculate_gravity_acceleration(
            state.position,
            config.mu
        );

    Vector2D total_acceleration =
        add_vectors(
            gravity_acceleration,
            config.thrust_acceleration
        );

    double radius =
        calculate_magnitude(state.position);

    double speed =
        calculate_magnitude(state.velocity);

    double orbital_energy = 0.0;

    if (config.mu != 0.0)
    {
        orbital_energy =
            calculate_specific_orbital_energy(
                state,
                config.mu
            );
    }

    double angular_momentum =
        calculate_specific_angular_momentum(state);

    double distance_this_step =
        calculate_displacement(
            previous_state.position,
            state.position
        );

    return {
        time,
        state,
        gravity_acceleration,
        config.thrust_acceleration,
        total_acceleration,
        radius,
        speed,
        orbital_energy,
        angular_momentum,
        distance_this_step,
        total_distance
    };
}

// ============================================================
// TEST CASES
// ============================================================

SimulationConfig test_no_gravity()
{
    SimulationConfig config;

    config.mu = 0.0;

    config.initial_position = {0.0, 0.0};
    config.initial_velocity = {10.0, 5.0};

    config.thrust_acceleration = {2.0, 1.0};

    double simulation_duration = 10.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration /
            config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}

SimulationConfig test_gravity_only()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};
    config.initial_velocity = {0.0, 0.0};

    config.thrust_acceleration = {0.0, 0.0};

    double simulation_duration = 10.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration /
            config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}

SimulationConfig test_circular_orbit()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};

    // Circular orbital velocity:
    // v = sqrt(mu / r)

    double radius =
        calculate_magnitude(
            config.initial_position
        );

    double circular_velocity =
        std::sqrt(
            config.mu / radius
        );

    config.initial_velocity = {
        0.0,
        circular_velocity
    };

    config.thrust_acceleration = {
        0.0,
        0.0
    };

    double simulation_duration = 100.0;

    config.time_step = 1.0;

    config.number_of_steps =
        static_cast<int>(
            simulation_duration /
            config.time_step
        );

    config.integrator = Integrator::RK4;

    return config;
}

// ============================================================
// OUTPUT
// ============================================================

std::string integrator_name(Integrator integrator)
{
    switch (integrator)
    {
        case Integrator::Euler:
            return "Euler";

        case Integrator::RK4:
            return "RK4";

        default:
            return "Unknown";
    }
}

void print_results_table(const std::vector<SimulationStep>& results)
{
    std::cout << "\n";

    std::cout
        << std::left
        << std::setw(8)  << "Time"
        << std::setw(12) << "X"
        << std::setw(12) << "Y"
        << std::setw(12) << "VX"
        << std::setw(12) << "VY"
        << std::setw(12) << "Radius"
        << std::setw(12) << "Speed"
        << std::setw(14) << "Energy"
        << std::setw(14) << "Ang. Mom."
        << std::setw(14) << "Step Dist."
        << std::setw(14) << "Total Dist."
        << "\n";

    std::cout
        << std::string(138, '-')
        << "\n";

    for (const SimulationStep& result : results)
    {
        std::cout
            << std::left
            << std::setw(8)  << result.time
            << std::setw(12) << result.state.position.x
            << std::setw(12) << result.state.position.y
            << std::setw(12) << result.state.velocity.x
            << std::setw(12) << result.state.velocity.y
            << std::setw(12) << result.radius
            << std::setw(12) << result.speed
            << std::setw(14) << result.orbital_energy
            << std::setw(14) << result.angular_momentum
            << std::setw(14) << result.distance_this_step
            << std::setw(14) << result.total_distance
            << "\n";
    }
}

void print_summary(const std::vector<SimulationStep>& results, SimulationConfig config)
{
    const SimulationStep& initial = results.front();
    const SimulationStep& final = results.back();

    double final_displacement =
        calculate_displacement(
            initial.state.position,
            final.state.position
        );

    std::cout << "\n";
    std::cout
        << "============================================================\n"
        << "SIMULATION SUMMARY\n"
        << "============================================================\n";

    std::cout
        << "Integrator: "
        << integrator_name(config.integrator)
        << "\n";

    std::cout
        << "Time step: "
        << config.time_step
        << "\n";

    std::cout
        << "Simulation duration: "
        << final.time
        << "\n";

    std::cout
        << "\nInitial position: ("
        << initial.state.position.x
        << ", "
        << initial.state.position.y
        << ")\n";

    std::cout
        << "Final position:   ("
        << final.state.position.x
        << ", "
        << final.state.position.y
        << ")\n";

    std::cout
        << "\nInitial velocity: ("
        << initial.state.velocity.x
        << ", "
        << initial.state.velocity.y
        << ")\n";

    std::cout
        << "Final velocity:   ("
        << final.state.velocity.x
        << ", "
        << final.state.velocity.y
        << ")\n";

    std::cout
        << "\nInitial radius: "
        << initial.radius
        << "\n";

    std::cout
        << "Final radius:   "
        << final.radius
        << "\n";

    std::cout
        << "Final displacement: "
        << final_displacement
        << "\n";

    std::cout
        << "Total distance traveled: "
        << final.total_distance
        << "\n";

    if (config.mu != 0.0)
    {
        double energy_change =
            final.orbital_energy -
            initial.orbital_energy;

        double angular_momentum_change =
            final.angular_momentum -
            initial.angular_momentum;

        std::cout
            << "\nInitial specific orbital energy: "
            << initial.orbital_energy
            << "\n";

        std::cout
            << "Final specific orbital energy:   "
            << final.orbital_energy
            << "\n";

        std::cout
            << "Change in specific orbital energy: "
            << energy_change
            << "\n";

        std::cout
            << "\nInitial specific angular momentum: "
            << initial.angular_momentum
            << "\n";

        std::cout
            << "Final specific angular momentum:   "
            << final.angular_momentum
            << "\n";

        std::cout
            << "Change in specific angular momentum: "
            << angular_momentum_change
            << "\n";
    }
}

// ============================================================
// VALIDATION
// ============================================================

void validate_constant_acceleration(const std::vector<SimulationStep>& results, SimulationConfig config)
{
    if (config.mu != 0.0)
    {
        return;
    }

    State initial_state = results.front().state;

    State analytical_state =
        calculate_analytical_state(
            initial_state,
            config.thrust_acceleration,
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
        << "Velocity error: "
        << velocity_error
        << "\n";
}

// ============================================================
// SIMULATION
// ============================================================

std::vector<SimulationStep> run_simulation(SimulationConfig config)
{
    State spacecraft;

    spacecraft.position =
        config.initial_position;

    spacecraft.velocity =
        config.initial_velocity;

    if (config.mu != 0.0 &&
        calculate_magnitude(spacecraft.position) == 0.0)
    {
        throw std::runtime_error(
            "Invalid initial condition: "
            "spacecraft cannot start at the origin "
            "when gravity is enabled."
        );
    }

    std::vector<SimulationStep> results;

    results.reserve(
        config.number_of_steps + 1
    );

    double total_distance = 0.0;

    // Store initial state at t = 0.

    SimulationStep initial_result =
        calculate_simulation_step(
            spacecraft,
            spacecraft,
            0.0,
            0.0,
            config
        );

    results.push_back(initial_result);

    // Propagate.

    for (int step = 1;
         step <= config.number_of_steps;
         ++step)
    {
        State previous_state =
            spacecraft;

        spacecraft =
            propagate_state(
                spacecraft,
                config
            );

        double distance_this_step =
            calculate_displacement(
                previous_state.position,
                spacecraft.position
            );

        total_distance +=
            distance_this_step;

        double elapsed_time =
            step * config.time_step;

        SimulationStep result =
            calculate_simulation_step(
                spacecraft,
                previous_state,
                elapsed_time,
                total_distance,
                config
            );

        results.push_back(result);
    }

    return results;
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    int test_case;

    std::cout << "Select simulation:\n";
    std::cout << "1. No gravity\n";
    std::cout << "2. Gravity only\n";
    std::cout << "3. Circular orbit\n";
    std::cout << "Selection: ";

    std::cin >> test_case;

    SimulationConfig config;

    switch (test_case)
    {
        case 1:
            config = test_no_gravity();
            break;

        case 2:
            config = test_gravity_only();
            break;

        case 3:
            config = test_circular_orbit();
            break;

        default:
            std::cout
                << "Invalid test case.\n";

            return 1;
    }

    try
    {
        std::cout
            << std::fixed
            << std::setprecision(4);

        std::vector<SimulationStep> results =
            run_simulation(config);

        print_results_table(results);

        print_summary(
            results,
            config
        );

        validate_constant_acceleration(
            results,
            config
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
