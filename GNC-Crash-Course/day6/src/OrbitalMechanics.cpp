#include "OrbitalMechanics.h"

#include "Vector2D.h"

#include <stdexcept>

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