# Day 6 — Modular Simulation Architecture

## Overview

Day 6 is the first major architectural refactoring of the MiniOrion project.

The earlier iterations developed the mathematical and numerical foundations of the simulator in a single `main.cpp` file. Day 6 reorganizes those capabilities into separate modules with `.h` header files and corresponding `.cpp` implementation files.

The goal of this iteration was not to introduce a large amount of new physics. Instead, the focus was on software architecture, separation of responsibilities, reusable interfaces, and preparing the simulator for continued development.

The result is a small modular spacecraft simulation framework in which the major components of the simulation are separated into independent modules.

## Goals

### Primary Goals

- Refactor the simulation from a single source file into multiple modules.
- Separate physics calculations from numerical integration.
- Separate simulation management from orbital mechanics calculations.
- Create reusable data structures for spacecraft state and vectors.
- Separate test-case configuration from the simulation itself.
- Separate output formatting from simulation logic.
- Create a dedicated validation module.
- Establish an architecture that can support future iterations without continuing to grow a single `main.cpp`.

### C++ Goals

This iteration also focused on becoming more comfortable with:

- Header files
- Source files
- Include guards
- Function declarations and definitions
- `enum class`
- Modular compilation
- Interfaces between source files
- Separating data structures from implementation
- Managing dependencies between modules

## Architecture

Day 6 divides the simulator into the following major components:

```text
main.cpp
    |
    +------------------+
    |                  |
    v                  v
Simulation          TestCases
    |
    +-------------------+
    |                   |
    v                   v
Integrator           Dynamics
    |                   |
    |                   v
    |              Orbital Mechanics
    |
    v
State
    |
    v
Vector2D

Simulation
    |
    +----> Output
    |
    +----> Validation
```

The exact dependency structure is implemented through the header files and source files in this directory.

The important architectural idea is that each module has a relatively narrow responsibility.

## Module Responsibilities

### `Vector2D`

#### Files

- `Vector2D.h`
- `Vecror2D.cpp`

This module provides the basic two-dimensional vector operations used throughout the simulator.

It contains functions for:

- Vector addition
- Vector subtraction
- Vector scaling
- Vector magnitude

This creates a common mathematical interface that other modules can use instead of repeatedly implementing vector arithmetic.

#### Why this matters

Vector operations appear throughout the simulation, including:

- Position propagation
- Velocity propagation
- Gravity calculations
- Acceleration calculations
- Distance calculations
- Orbital mechanics calculations

Centralizing these operations reduces duplicated code and makes the rest of the simulator easier to read.

### `State`

#### Files

- `State.h`

The `State` module defines the mathematical representation of the spacecraft's translational state.

A spacecraft state consists of:

- Position
- Velocity

The module also defines `StateDerivative`, which represents the time derivatives of those quantities:

- Position derivative
- Velocity derivative

For this simulation:

- The derivative of position is velocity.
- The derivative of velocity is acceleration.

#### Why this matters

The state representation provides the common interface used by the numerical integrators and dynamics model.

This also begins to resemble the mathematical structure used in real numerical simulation and GNC systems, where a system is represented as a state vector and propagated according to its equations of motion.

### `Dynamics`

#### Files

- `Dynamics.h`
- `Dynamics.cpp`

The Dynamics module calculates the forces/accelerations acting on the spacecraft.

Currently, the primary environmental model is central-body point-mass gravity.

The module provides functions for:

- Calculating gravitational acceleration
- Calculating the derivative of the spacecraft state

The gravitational acceleration follows the two-body point-mass model:

$$$
\vec{a}_{gravity} = -\frac{\mu}{r^3}\vec{r}
$$$

where:

- $\vec{r}$ is the spacecraft position vector.
- $r$ is the magnitude of the position vector.
- $\mu$ is the gravitational parameter.

#### Why this matters

### `Integrator`

#### Files

- `Integrator.h`
- `Integrator.cpp`



#### Why this matters

### `OrbitalMechanics`

#### Files

- `OrbitalMechanics.h`
- `OrbitalMechanics.cpp`



#### Why this matters

### `Simulation`

#### Files

- `Simulation.h`
- `Simulation.cpp`



#### Why this matters

### `TestCases`

#### Files

- `TestCases.h`
- `TestCases.cpp`



#### Why this matters

### `Validation`

#### Files

- `Validation.h`
- `Validation.cpp`



#### Why this matters

### `Output`

#### Files

- `Output.h`
- `Output.cpp`



#### Why this matters
