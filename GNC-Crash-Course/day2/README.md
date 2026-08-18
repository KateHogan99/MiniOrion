# Day 2 — Vector-Based State Representation

## Overview

Day 2 refactors the basic kinematics model from Day 1 into a vector-based representation.

Instead of storing position, velocity, and acceleration as separate scalar X/Y values, the simulation now represents them using a reusable `Vector2D` structure.

This is an important step toward the mathematical representation used in orbital mechanics and spacecraft simulation, where position, velocity, and acceleration are naturally treated as vectors.

---

## Goals

The goals for Day 2 were to:

- Replace separate X/Y quantities with a reusable `Vector2D` structure.
- Represent spacecraft position, velocity, and acceleration as vectors.
- Create reusable vector operations.
- Use vector operations to simplify the kinematic equations.
- Continue practicing C++ structs, functions, and return values.
- Maintain the same constant-acceleration physics introduced on Day 1.

---

## What I Learned

### 1. Vector Representation

The Day 1 state stored every component independently:

```cpp
struct State
{
    double x;
    double y;

    double vx;
    double vy;

    double ax;
    double ay;
};
