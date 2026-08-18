# Day 7 — Expanded Orbital Validation

## Overview

Day 7 expands the validation capabilities of the Mini-Orion spacecraft simulation.

Day 6 established a modular simulation architecture, separating the spacecraft state, dynamics model, numerical integrators, orbital mechanics, simulation management, test cases, and validation.

Day 7 builds on that architecture by making validation significantly more systematic.

The primary focus of this iteration was:

- Expanding the set of orbital test cases
- Adding orbital eccentricity calculations
- Introducing configurable validation tolerances
- Moving from fixed absolute tolerances toward relative error tolerances
- Creating separate validation checks for different physical behaviors
- Developing validation logic for circular, elliptical, and escape trajectories

The result is a simulation that can now evaluate not only whether it produces a trajectory, but whether that trajectory satisfies specific physical expectations.

## Goals

### Primary goals

- Expand the simulation beyond circular-orbit testing.
- Add elliptical-orbit and escape-trajectory scenarios.
- Calculate orbital eccentricity.
- Develop separate validation checks for different orbital behaviors.
- Replace hard-coded validation thresholds with configurable tolerances.
- Introduce relative error calculations.
- Use conservation of orbital energy and angular momentum as validation criteria.
- Create a centralized validation process that reports the result of each test.

### C++ goals

This iteration also focused on:

- Designing configuration structures
- Passing configuration data between modules
- Using std::string for validation results
- Returning structured test results
- Further separating simulation configuration from validation configuration
- Building reusable validation functions
- Managing dependencies between headers
- Continuing to practice modular C++ development

## What Changed from Day 6

Day 6 established the modular architecture.

Day 7 primarily expands what that architecture is capable of evaluating.

#### Day 6

The simulation could model:

- No-gravity motion
- Gravity-only motion
- Circular orbits

It could calculate:

- Position
- Velocity
- Acceleration
- Radius
- Speed
- Specific orbital energy
- Specific angular momentum

Validation was comparatively limited.

#### Day 7

The simulation now includes:

- No-gravity motion
- Gravity-only motion
- Circular orbits
- Elliptical orbits
- Escape trajectories

The orbital mechanics module now also calculates:

- Orbital eccentricity

The validation module now evaluates:

- Constant-acceleration accuracy
- Circular-orbit behavior
- Orbital energy conservation
- Angular momentum conservation
- Elliptical-orbit behavior

This represents a shift from simply running simulations toward testing simulations against expected physical behavior.

## Orbital Mechanics

### Specific Orbital Energy

The simulation continues to calculate specific orbital mechanical energy:

$$
\epsilon = \frac{v^2}{2} - \frac{\mu}{r}
$$

For an ideal two-body system with no thrust, specific orbital energy should remain constant.

The validation framework uses changes in this quantity as one measure of numerical propagation error.

### Specific Angular Momentum

For the 2D simulation, specific angular momentum is represented by the z-component:

$$
h_z = xv_y - yv_x
$$

For an ideal two-body system without external torque, angular momentum should remain constant.

The validation framework therefore checks how much this quantity changes over the course of a simulation.

### Orbital Eccentricity

Day 7 introduces calculation of orbital eccentricity.

The eccentricity of a two-body orbit can be calculated from the spacecraft's state and gravitational parameter.

Conceptually, eccentricity describes the shape of the orbit:

| Eccentricity | Orbit |
| :--- | :--- |
| $e=0$ | Circular |
| $0<e<1$ | Elliptical |
| $e=1$ | Parabolic |
| $e>1$ | Hyperbolic |

This provides a useful physical quantity for classifying the trajectories produced by the simulator.
