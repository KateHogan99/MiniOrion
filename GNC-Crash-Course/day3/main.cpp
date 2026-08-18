#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>

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

struct SimulationConfig
{
    double mu;

    Vector2D initial_position;
    Vector2D initial_velocity;
    Vector2D thrust_acceleration;

    double time_step;
    int number_of_steps;
};

// ============================================================
// VECTOR FUNCTIONS
// ============================================================

Vector2D add_vectors(Vector2D vector_a, Vector2D vector_b)
{
    Vector2D sum_vector;

    sum_vector.x = vector_a.x + vector_b.x;
    sum_vector.y = vector_a.y + vector_b.y;

    return sum_vector;
}

Vector2D subtract_vectors(Vector2D vector_a, Vector2D vector_b)
{
    Vector2D difference_vector;

    difference_vector.x = vector_a.x - vector_b.x;
    difference_vector.y = vector_a.y - vector_b.y;

    return difference_vector;
}

Vector2D scale_vector(Vector2D vector, double scale)
{
    Vector2D scaled_vector;

    scaled_vector.x = vector.x * scale;
    scaled_vector.y = vector.y * scale;

    return scaled_vector;
}

double calculate_magnitude(Vector2D vector)
{
    return std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

// ============================================================
// KINEMATICS
// ============================================================

Vector2D calculate_position(Vector2D position, Vector2D velocity, Vector2D acceleration, double time)
{
    return add_vectors(
        add_vectors(
            position,
            scale_vector(velocity, time)
        ),
        scale_vector(acceleration, 0.5 * time * time)
    );
}

Vector2D calculate_velocity(Vector2D velocity, Vector2D acceleration, double time)
{
    return add_vectors(velocity, scale_vector(acceleration, time));
}

// ============================================================
// GRAVITY
// ============================================================

Vector2D calculate_gravity_acceleration(Vector2D position, double mu)
{
    // If MU is zero, gravity is disabled.
    if (mu == 0.0)
    {
        return {0.0, 0.0};
    }

    double radius = calculate_magnitude(position);

    // The point-mass gravity model is undefined at the origin.
    if (radius == 0.0)
    {
        throw std::runtime_error(
            "Cannot calculate gravity: spacecraft is at the origin."
        );
    }

    return scale_vector(position, -mu / std::pow(radius, 3.0));
}

// ============================================================
// PROPAGATION
// ============================================================

State propagate_state(State spacecraft, Vector2D thrust_acceleration, double mu, double d_time)
{
    State propagated_spacecraft;

    Vector2D gravity_acceleration = calculate_gravity_acceleration(spacecraft.position, mu);
    Vector2D total_acceleration = add_vectors(gravity_acceleration, thrust_acceleration);

    propagated_spacecraft.position = calculate_position(spacecraft.position, spacecraft.velocity, total_acceleration, d_time);
    propagated_spacecraft.velocity = calculate_velocity(spacecraft.velocity, total_acceleration, d_time);

    return propagated_spacecraft;
}

// ============================================================
// TEST / ANALYSIS FUNCTIONS
// ============================================================

double calculate_displacement(Vector2D initial_position, Vector2D final_position)
{
    return calculate_magnitude(subtract_vectors(final_position, initial_position));
}

double calculate_distance_this_step(Vector2D previous_position, Vector2D current_position)
{
    return calculate_displacement(previous_position, current_position);
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

    config.time_step = 1.0;
    config.number_of_steps = 10;

    return config;
}

SimulationConfig test_gravity_only()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};
    config.initial_velocity = {0.0, 0.0};

    config.thrust_acceleration = {0.0, 0.0};

    config.time_step = 1.0;
    config.number_of_steps = 10;

    return config;
}

SimulationConfig test_circular_orbit()
{
    SimulationConfig config;

    config.mu = 10000.0;

    config.initial_position = {100.0, 0.0};
    config.initial_velocity = {0.0, 10.0};

    config.thrust_acceleration = {0.0, 0.0};

    // Smaller timestep gives a better approximation
    // of the circular orbit.
    config.time_step = 0.1;
    config.number_of_steps = 1000;

    return config;
}

// ============================================================
// SIMULATION
// ============================================================

void run_simulation(SimulationConfig config)
{
    State spacecraft;

    spacecraft.position = config.initial_position;
    spacecraft.velocity = config.initial_velocity;

    // Check initial position if gravity is enabled.
    if (config.mu != 0.0 &&
        calculate_magnitude(spacecraft.position) == 0.0)
    {
        throw std::runtime_error(
            "Invalid initial condition: spacecraft cannot "
            "start at the origin when gravity is enabled."
        );
    }

    State initial_state = spacecraft;

    double total_distance = 0.0;

    std::cout << std::fixed << std::setprecision(4);

    for (int step = 1;
         step <= config.number_of_steps;
         step++)
    {
        State previous_state = spacecraft;

        State spacecraft_t =
            propagate_state(
                spacecraft,
                config.thrust_acceleration,
                config.mu,
                config.time_step
            );

        double distance_this_step =
            calculate_distance_this_step(
                previous_state.position,
                spacecraft_t.position
            );

        total_distance += distance_this_step;

        Vector2D gravity_acceleration =
            calculate_gravity_acceleration(
                previous_state.position,
                config.mu
            );

        Vector2D total_acceleration =
            add_vectors(
                gravity_acceleration,
                config.thrust_acceleration
            );

        double elapsed_time =
            step * config.time_step;

        std::cout << "Time: " << elapsed_time << "\n";

        std::cout << "Position: "
                  << spacecraft_t.position.x << ", "
                  << spacecraft_t.position.y << "\n";

        std::cout << "Velocity: "
                  << spacecraft_t.velocity.x << ", "
                  << spacecraft_t.velocity.y << "\n";

        std::cout << "Speed: "
                  << calculate_magnitude(
                         spacecraft_t.velocity
                     )
                  << "\n";

        std::cout << "Gravity acceleration: "
                  << gravity_acceleration.x << ", "
                  << gravity_acceleration.y << "\n";

        std::cout << "Thrust acceleration: "
                  << config.thrust_acceleration.x << ", "
                  << config.thrust_acceleration.y << "\n";

        std::cout << "Total acceleration: "
                  << total_acceleration.x << ", "
                  << total_acceleration.y << "\n";

        std::cout << "Distance this step: "
                  << distance_this_step << "\n";

        std::cout << "\n";

        spacecraft = spacecraft_t;
    }

    double final_displacement =
        calculate_displacement(
            initial_state.position,
            spacecraft.position
        );

    std::cout << "========================================\n";
    std::cout << "SIMULATION SUMMARY\n";
    std::cout << "========================================\n";

    std::cout << "Initial position: "
              << initial_state.position.x << ", "
              << initial_state.position.y << "\n";

    std::cout << "Final position: "
              << spacecraft.position.x << ", "
              << spacecraft.position.y << "\n";

    std::cout << "Initial velocity: "
              << initial_state.velocity.x << ", "
              << initial_state.velocity.y << "\n";

    std::cout << "Final velocity: "
              << spacecraft.velocity.x << ", "
              << spacecraft.velocity.y << "\n";

    std::cout << "Final displacement: "
              << final_displacement << "\n";

    std::cout << "Total distance traveled: "
              << total_distance << "\n";
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
            std::cout << "Invalid test case.\n";
            return 1;
    }

    try
    {
        run_simulation(config);
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "Simulation error: "
                  << error.what()
                  << "\n";

        return 1;
    }

    return 0;
}
