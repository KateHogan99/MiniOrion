#include <iostream>
#include <cmath>

struct State
{
    double x; // X position
    double y; // Y position

    double vx; // X velocity
    double vy; // Y velocity

    double ax; // X acceleration
    double ay; // Y acceleration
};

double calculate_position(double position, double velocity, double acceleration, double time)
{
    return position + (velocity * time) + (0.5 * acceleration * time * time);
}

double calculate_displacement(double initial_x, double initial_y, double final_x, double final_y)
{
    double x_distance = final_x - initial_x;
    double y_distance = final_y - initial_y;

    return std::sqrt((x_distance * x_distance) + (y_distance * y_distance));
}

double calculate_velocity(double velocity, double acceleration, double time)
{
    return velocity + acceleration * time;
}

double calculate_speed(double x_velocity, double y_velocity)
{
    return std::sqrt((x_velocity * x_velocity) + (y_velocity * y_velocity));
}

State propagate_state (State spacecraft, double d_time)
{
    State propagated_spacecraft;

    propagated_spacecraft.x = calculate_position(spacecraft.x, spacecraft.vx, spacecraft.ax, d_time);
    propagated_spacecraft.y = calculate_position(spacecraft.y, spacecraft.vy, spacecraft.ay, d_time);
    propagated_spacecraft.vx = calculate_velocity(spacecraft.vx, spacecraft.ax, d_time);
    propagated_spacecraft.vy = calculate_velocity(spacecraft.vy, spacecraft.ay, d_time);
    propagated_spacecraft.ax = spacecraft.ax;
    propagated_spacecraft.ay = spacecraft.ay;

    return propagated_spacecraft;
}

int main()
{
    State spacecraft;

    spacecraft.x = 100.0;
    spacecraft.y = 200.0;
    spacecraft.vx = 10.0;
    spacecraft.vy = 5.0;
    spacecraft.ax = 2.0;
    spacecraft.ay = 1.0;

    State spacecraft_0 = spacecraft;

    for (int time = 1; time <= 10; time++)
    {
        State spacecraft_t = propagate_state(spacecraft, 1);

        std::cout << "Time: " << time << "\n";
        
        std::cout << "Position: "
                << spacecraft_t.x << ", "
                << spacecraft_t.y << "\n";

        std::cout << "Total Displacement: "
                << calculate_displacement(spacecraft_0.x, spacecraft_0.y, spacecraft_t.x, spacecraft_t.y) << "\n";

        std::cout << "Velocity: "
                << spacecraft_t.vx << ", "
                << spacecraft_t.vy << "\n";

        std::cout << "Speed: "
                << calculate_speed(spacecraft_t.vx, spacecraft_t.vy) << "\n";

        std::cout << "Acceleration: "
                << spacecraft_t.ax << ", "
                << spacecraft_t.ay << "\n\n";

        spacecraft = spacecraft_t;
    }

    return 0;
}
