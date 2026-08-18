#ifndef DYNAMICS_H
#define DYNAMICS_H

#include "State.h"
#include "Vector2D.h"

Vector2D calculate_gravity_acceleration(
    Vector2D position, 
    double mu
);

StateDerivative calculate_state_derivative(
    State state, 
    Vector2D thrust_acceleration, 
    double mu
);

#endif
