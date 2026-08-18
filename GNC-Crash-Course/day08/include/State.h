#ifndef STATE_H
#define STATE_H

#include "Vector2D.h"

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

#endif