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
