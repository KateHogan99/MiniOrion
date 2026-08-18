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
\text{Relative Error (\\%)} = \left| \frac{x - x_{ref}}{x_{ref}} \right| \times 100
$$

where:

- $x$ is the calculated value
- $x_{ref}$ is the reference value

Relative error is particularly useful when quantities may have very different scales.

For example, an absolute error of $0.1$ might be insignificant for a quantity near $1000$, but extremely significant for a quantity near $0.01$.

### Validation Configuration

Validation tolerances are now stored separately from the simulation configuration.

The `ValidationConfig` structure contains tolerances for:

- Constant-acceleration position error
- Constant-acceleration velocity error
- Circular-orbit radius deviation
- Circular-orbit speed deviation
- Orbital energy conservation
- Angular momentum conservation
- Elliptical-orbit eccentricity deviation

The current relative-error tolerances configured in main.cpp are:

$$
0.01\\%
$$

for:

- Constant-acceleration position
- Constant-acceleration velocity
- Circular-orbit radius
- Circular-orbit speed
- Orbital energy
- Angular momentum

The elliptical-orbit validation currently uses:

$$
0.001
$$

as an absolute eccentricity tolerance.

#### Important distinction

Not all Day 7 tolerances use the same definition.

Most validation tolerances are specified as relative percentage errors, while the elliptical-orbit eccentricity tolerance is currently an absolute difference in eccentricity.

This distinction is intentional and is an area that can be refined in future iterations.

### Validation Tests

#### Constant Acceleration

The numerical result is compared against the analytical constant-acceleration solution.

The analytical solution is:

$$
r(t) = r_0 + v_0t + \frac{1}{2}at^2
$$

and

$$
v(t) = v_0 + at
$$

The numerical and analytical results are compared using relative error.

This provides a known-answer test for the numerical integration system.

## Circular Orbit Validation

The circular-orbit validation checks whether the initial conditions actually represent a circular orbit.

The initial speed is compared against:

$$
v_c = \sqrt{\frac{\mu}{r}}
$$

If the initial conditions are not sufficiently close to the expected circular velocity, the test is not considered applicable.

The simulation then evaluates the maximum deviation of orbital radius from the initial radius.

The result passes if the deviation remains within the configured tolerance.

## Orbital Energy Conservation

For an unforced two-body system, specific orbital energy should remain constant.

The validation process determines the largest deviation from the initial orbital energy over the simulation.

The relative change is then compared against the configured tolerance.

This provides a useful measure of numerical integration error.

## Angular Momentum Conservation

The simulation similarly evaluates conservation of specific angular momentum.

For an ideal two-body system with no external torque:

$$
h = \text{constant}
$$

The validation process determines the largest deviation from the initial angular momentum and compares it against the configured relative tolerance.

This provides a second independent conservation-law check.

## Elliptical Orbit Validation

The elliptical-orbit validation calculates eccentricity at the beginning and end of the simulation.

The change is:

$$
\Delta e = \left| e_{final} - e_{initial} \right|
$$

The simulation passes if this change remains within the configured eccentricity tolerance.

This is intended to verify that a bound elliptical trajectory maintains approximately the same orbital shape over the simulation.

## Escape Trajectory Validation

The escape trajectory test is designed to represent an unbound orbit.

The initial conditions are expected to produce:

$$
\epsilon > 0
$$

and:

$$
e > 1
$$

The validation also examines the spacecraft's radial behavior near the end of the simulation.

A successful escape trajectory should demonstrate that the spacecraft is moving outward rather than returning toward the central body.

## Validation Flow

The validation process now follows a structure approximately like:

```text
Simulation Results
       |
       v
validate_simulation()
       |
       +----> Constant Acceleration
       |
       +----> Circular Orbit
       |
       +----> Orbital Energy Conservation
       |
       +----> Angular Momentum Conservation
       |
       +----> Elliptical Orbit
       |
       +----> Escape Trajectory
       |
       v
Validation Results
```

Each individual validation function returns a result such as:

- `PASS`
- `FAIL`
- `N/A`

`N/A` allows a validation to indicate that its physical assumptions do not apply to the current simulation.

For example, conservation-of-orbital-energy validation is not applicable when thrust is active because the spacecraft is no longer an unforced two-body system.

## Separation of Configuration

Day 7 introduces an important distinction between two types of configuration.

### `SimulationConfig`

Controls the simulation itself:

- Initial conditions
- Gravitational parameter
- Thrust
- Timestep
- Number of steps
- Numerical integrator

### `ValidationConfig`

Controls how simulation results are evaluated:

- Acceptable analytical error
- Acceptable orbital deviations
- Conservation tolerances
- Eccentricity tolerance

This separation prevents the physical simulation parameters from becoming mixed with the criteria used to judge the simulation.

## Current Program Flow

The Day 7 `main.cpp` follows a significantly simpler structure than earlier versions.

The general flow is:

1. Create a predefined simulation configuration.
2. Create a validation configuration.
3. Run the simulation.
4. Pass the resulting simulation history to the validation framework.
5. Run all applicable validation checks.
6. Report the validation results.

The simulation itself is now largely hidden behind the modular interfaces.

This is an important architectural improvement because `main.cpp` no longer needs to know how gravity, integration, orbital mechanics, or validation are implemented.

## Directory Structure

Day 7 also introduces a more conventional separation between header files and source files.

```text
day7/
├── include/
│   ├── Dynamics.h
│   ├── Integrator.h
│   ├── OrbitalMechanics.h
│   ├── Simulation.h
│   ├── State.h
│   ├── TestCases.h
│   ├── Validation.h
│   └── Vector2D.h
│
├── src/
│   ├── Dynamics.cpp
│   ├── Integrator.cpp
│   ├── OrbitalMechanics.cpp
│   ├── Simulation.cpp
│   ├── TestCases.cpp
│   ├── Validation.cpp
│   ├── Vector2D.cpp
│   └── main.cpp
│
└── README.md
```

This structure keeps interfaces and implementations separate and provides a cleaner foundation for future development.

## What I Learned

The major lesson of Day 7 was that a numerical simulation needs a way to determine whether its results are actually trustworthy.

A trajectory that visually resembles an orbit is not sufficient evidence that the numerical solution is accurate.

The project now uses multiple independent checks:

- Analytical solutions
- Orbital geometry
- Orbital energy
- Angular momentum
- Orbital eccentricity
- Relative numerical error

These checks provide different perspectives on the quality of the simulation.

Day 7 also reinforced the importance of separating simulation configuration from validation criteria. The spacecraft simulation should produce a result independently of how strict the test used to evaluate that result happens to be.

## Limitations

Day 7 remains a simplified spacecraft dynamics simulation.

Current limitations include:

- 2D translational dynamics only
- Point-mass central gravity
- No atmospheric effects
- No perturbing bodies
- No attitude dynamics
- No rotational state
- No sensor models
- No navigation solution
- No guidance algorithm
- No closed-loop controller
- No actuator dynamics
- Fixed timestep integration
- Limited automated testing
- No graphical visualization
- No automated test runner

Additionally, validation currently focuses primarily on endpoint values or maximum deviations rather than performing a comprehensive statistical analysis of the entire trajectory.

These limitations are intentional and define areas for future iterations.

## Next Steps

The Day 7 architecture provides the foundation for moving beyond basic orbital propagation.

Future iterations can build on this by introducing improvements such as:

- More rigorous numerical accuracy testing
- Systematic timestep studies
- Automated comparison of Euler and RK4
- Improved validation reporting
- More robust orbital classification
- Additional physical models
- Data export for analysis and visualization
- Improved simulation configuration
- More sophisticated trajectory analysis
- Eventually, guidance and control functionality

The longer-term objective is to evolve the project from an orbital mechanics simulator into a small-scale demonstration of spacecraft Guidance, Navigation, and Control (GNC) concepts.

## Summary

Day 7 represents a shift from simulating orbital motion toward verifying orbital simulations.

The project now has:

- Five predefined orbital test scenarios
- Central-body gravitational dynamics
- Euler and RK4 numerical integration
- Specific orbital energy calculations
- Specific angular momentum calculations
- Orbital eccentricity calculations
- Analytical validation
- Conservation-law validation
- Circular-orbit validation
- Elliptical-orbit validation
- Escape-trajectory validation
- Configurable validation tolerances
- Relative-error calculations
- Separate simulation and validation configuration

The central concept introduced in this iteration is:

> A simulation is only as useful as the evidence that its results are correct.

Day 7 therefore establishes the validation framework that future MiniOrion iterations can build upon as the physics and GNC functionality become more sophisticated.
