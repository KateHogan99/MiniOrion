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

## New Test Cases

The TestCases module now provides five predefined scenarios. 

### 1. No Gravity

This scenario sets:

$$
\mu = 0
$$

and applies a constant thrust acceleration.

This provides a case with a known analytical solution and is therefore useful for validating the numerical integrator.

### 2. Gravity Only

This scenario enables central-body gravity while applying no thrust.

It provides a basic test of the gravitational dynamics model.

### 3. Circular Orbit

The initial velocity is selected using the circular-orbit relationship:

$$
v_c = \sqrt{\frac{\mu}{r}}
$$

With the appropriate initial conditions and no thrust, the spacecraft should remain approximately at a constant orbital radius.

This makes circular-orbit radius stability a useful validation criterion.

### 4. Elliptical Orbit

Day 7 introduces an initial condition corresponding to a bound, non-circular orbit.

For an elliptical orbit:

$$
0 < e < 1
$$

The spacecraft's radius should vary throughout the orbit while remaining within a bounded trajectory.

The validation framework uses orbital eccentricity to assess whether the trajectory remains consistent with an elliptical orbit.

### 5. Escape Trajectory

Day 7 also introduces an unbound trajectory.

For an escape trajectory:

$$
e > 1
$$

and the specific orbital energy is positive:

$$
\epsilon > 0
$$

The test case provides a way to examine whether the simulation correctly represents motion that is no longer gravitationally bound.

## Validation Framework

The most significant development in Day 7 is the expanded validation framework.

Instead of having one validation function, the project now contains separate validation functions for different physical behaviors.

The central validation process calls each individual check and reports its result.

### Relative Error

Day 7 introduces a reusable relative-error calculation:

$$
\text{Relative Error (\%)} = \abs{\frac{x - x_{ref}}{x_{ref}}} \times 100
$$

where:

- $x$ is the calculated value
- $x_{ref}$ is the reference value

Relative error is particularly useful when quantities may have very different scales.

For example, an absolute error of $0.1$ might be insignificant for a quantity near $1000$, but extremely significant for a quantity near $0.01$.

### Validation Configuration

#### Important distinction

### Validation Tests

#### Constant Acceleration

## Circular Orbit Validation

## Orbital Energy Conservation

## Angular Momentum Conservation

## Elliptical Orbit Validation

## Escape Trajectory Validation

## Validation Flow

## Separation of Configuration

## Current Program Flow

## Directory Structure

## What I Learned

## Limitations

## Next Steps

## Summary
