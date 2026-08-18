#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "State.h"

enum class Integrator
{
    Euler,
    RK4
};

State apply_state_derivative(State state, StateDerivative derivative, double d_time);

State euler_integrate(State current_state, Vector2D thrust_acceleration, double mu, double d_time);

State rk4_integrate(State current_state, Vector2D thrust_acceleration, double mu, double d_time);

#endif