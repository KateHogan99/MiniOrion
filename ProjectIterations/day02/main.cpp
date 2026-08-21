#include <iostream>
#include <cmath>

struct Vector2D
{
    double x;
    double y;
};

struct State
{
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
};

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

Vector2D calculate_position(Vector2D position, Vector2D velocity, Vector2D acceleration, double time)
{
    Vector2D calculated_position = add_vectors(
        add_vectors(
            position, 
            scale_vector(velocity, time)
        ),
        scale_vector(acceleration, 0.5 * time * time)
    );

    return calculated_position;
}

double calculate_magnitude(Vector2D vector)
{
    return std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

double calculate_displacement(Vector2D initial_position, Vector2D final_position)
{
    Vector2D displacement_vector = subtract_vectors(final_position, initial_position);

    return calculate_magnitude(displacement_vector);
}

Vector2D calculate_velocity(Vector2D velocity, Vector2D acceleration, double time)
{
    Vector2D calculated_velocity = add_vectors(
        velocity,
        scale_vector(acceleration, time)
    );

    return calculated_velocity;
}

State propagate_state (State spacecraft, double d_time)
{
    State propagated_spacecraft;

    propagated_spacecraft.position = calculate_position(spacecraft.position, spacecraft.velocity, spacecraft.acceleration, d_time);
    propagated_spacecraft.velocity = calculate_velocity(spacecraft.velocity, spacecraft.acceleration, d_time);
    propagated_spacecraft.acceleration = spacecraft.acceleration;

    return propagated_spacecraft;
}

int main()
{
    State spacecraft;

    spacecraft.position.x = 100.0;
    spacecraft.position.y = 200.0;
    spacecraft.velocity.x = 10.0;
    spacecraft.velocity.y = 5.0;
    spacecraft.acceleration.x = 2.0;
    spacecraft.acceleration.y = 1.0;

    State spacecraft_0 = spacecraft;

    for (int time = 1; time <= 20; time++)
    {
        State spacecraft_t = propagate_state(spacecraft, 1);

        std::cout << "Time: " << time << "\n";

        std::cout << "Position: "
                << spacecraft_t.position.x << ", "
                << spacecraft_t.position.y << "\n";

        std::cout << "Displacement: "
                << calculate_displacement(spacecraft_0.position, spacecraft_t.position) << "\n";

        std::cout << "Velocity: "
                << spacecraft_t.velocity.x << ", "
                << spacecraft_t.velocity.y << "\n";

        std::cout << "Speed: "
                << calculate_magnitude(spacecraft_t.velocity) << "\n";

        std::cout << "Acceleration: "
                << spacecraft_t.acceleration.x << ", "
                << spacecraft_t.acceleration.y << "\n\n";

        spacecraft = spacecraft_t;
    }

    return 0;
}
