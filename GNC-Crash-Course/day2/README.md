# Day 2 — Vector and State Abstractions

## Objective

Refactor the Day 1 kinematics simulation to represent physical quantities using 2D vectors rather than separate X and Y variables.

The goal was to make the code more closely resemble the mathematical representation of spacecraft motion while reducing repeated component-by-component calculations.

## Concepts Studied

### Physics

- 2D vectors
- Vector addition and subtraction
- Vector scaling
- Vector magnitude
- Displacement as a vector difference
- Position and velocity as vector quantities

### C++

- Structures containing other structures
- Passing structures to functions
- Returning structures from functions
- Breaking calculations into reusable functions

## Implementation

A `Vector2D` structure was introduced:

```cpp
struct Vector2D
{
    double x;
    double y;
};
