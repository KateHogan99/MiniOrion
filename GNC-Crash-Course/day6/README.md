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

### 2D Vector

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

### State

#### Files

- `State.h`

The State module defines the mathematical representation of the spacecraft's translational state.

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

### Dynamics

#### Files

- `Dynamics.h`
- `Dynamics.cpp`

The Dynamics module calculates the forces/accelerations acting on the spacecraft.

Currently, the primary environmental model is central-body point-mass gravity.

The module provides functions for:

- Calculating gravitational acceleration
- Calculating the derivative of the spacecraft state

The gravitational acceleration follows the two-body point-mass model:

$$
\vec{a}_{gravity} = -\frac{\mu}{r^3}\vec{r}
$$

where:

- $\vec{r}$ is the spacecraft position vector.
- $r$ is the magnitude of the position vector.
- $\mu$ is the gravitational parameter.

The total acceleration currently consists of:

$$
\vec{a}_{total} = \vec{a}_{gravity} + \vec{a}_{thrust}
$$

#### Why this matters

Separating dynamics from integration is an important architectural decision.

The dynamics model answers:

> "What is the spacecraft's acceleration at this state?"

The integrator answers:

> "Given that derivative, how should the state be advanced through time?"

Keeping those responsibilities separate will make it easier to introduce additional force models later.

### Integrator

#### Files

- `Integrator.h`
- `Integrator.cpp`

The Integrator module contains the numerical methods used to propagate the spacecraft state.

Two integration methods are currently available:

- Euler
- Fourth-order Runge-Kutta (RK4)

The module defines an 'Integrator' enumeration that allows the simulation configuration to specify which numerical method should be used.

##### Euler integration

Euler integration approximates the next state using the derivative at the beginning of the timestep.

It is simple and computationally inexpensive, but generally produces significantly more numerical error than RK4 for orbital propagation.

##### RK4 integration

RK4 evaluates the state derivative at four points within each timestep and combines those evaluations using a weighted average.

The four evaluations are commonly represented as:

$$
k_1, k_2, k_3, k_4
$$

with the final update taking the form:

$$
y_{n+1} = y_n + \frac{\Delta t}{6}\left(k_1 + 2k_2 + 2k_3 + k_4\right)
$$

#### Why this matters

The separation between Dynamics and Integrator is particularly important for future development.

The numerical integrator does not need to know the details of why an acceleration exists. It only needs the state derivative.

This makes it possible to improve or replace the dynamics model without rewriting the integration algorithm.

### Orbital Mechanics

#### Files

- `OrbitalMechanics.h`
- `OrbitalMechanics.cpp`

The Orbital Mechanics module calculates quantities used to characterize and analyze the spacecraft's orbit.

Currently implemented quantities include:

##### Specific orbital energy

$$
\epsilon = \frac{v^2}{2} - \frac{\mu}{r}
$$

For an ideal two-body system without thrust, specific orbital energy should remain constant.

##### Specific angular momentum

For the 2D simulation, the angular momentum is represented by the out-of-plane z-component:

$$
h_z = xv_y - yv_x
$$

For an ideal two-body system without external torque, this quantity should remain constant.

#### Why this matters

These quantities provide more meaningful ways of evaluating an orbital simulation than simply examining whether the spacecraft appears to follow a reasonable trajectory.

They will also become important inputs to the validation framework in later iterations.

### Simulation

#### Files

- `Simulation.h`
- `Simulation.cpp`

The Simulation module acts as the central coordinator for the spacecraft propagation process.

It defines two important data structures.

##### `SimulationConfig`

This contains the parameters required to configure a simulation, including:

- Fravitational parameter
- Initial position
- Initial velocity
- Thrust acceleration
- Timestep
- Number of timesteps
- Numerical integration method

##### `SimulationStep`

This stores the results associated with an individual simulation timestep.

It includes:

- Simulation time
- Spacecraft state
- Gravity acceleration
- Thrust acceleration
- Total acceleration
- Radius
- Speed
- Specific orbital energy
- Specific angular momentum
- Distance traveled during the timestep
- Total accumulated distance

The module also provides functions responsible for:

- Propagating a state
- Calculating displacement
- Calculating the diagnostic information associated with a simulation step
- Running the complete simulation

#### Why this matters

The Simulation module acts as the bridge between the underlying physics and the higher-level program.

Rather than having `main.cpp` perform the actual simulation work, `main.cpp` now primarily coordinates the major components.

### Test Cases

#### Files

- `TestCases.h`
- `TestCases.cpp`

The TestCases module provides predefined simulation configurations.

Current test cases include:

- No gravity
- Gravity only
- Circular orbit

Each test case returns a `SimulationConfig` containing the appropriate initial conditions and simulation parameters.

#### Why this matters

Separating test-case creation from the simulation engine means that new scenarios can be added without modifying the underlying propagation code.

This will become increasingly useful as the project develops more sophisticated validation scenarios.

### Validation

#### Files

- `Validation.h`
- `Validation.cpp`

The Validation module evaluates whether the numerical simulation behaves as expected.

The module currently includes:

- An analytical constant-acceleration solution
- Constant-acceleration validation
- A higher-level simulation validation function

The analytical solution provides an independent reference for cases where the exact solution is known:

$$
r\left(t\right) = r_0 +v_0t + \frac{1}{2}at^2
$$

and

$$
v\left(t\right) = v_0 + at
$$

The numerical simulation can then be compared against this known solution.

#### Why this matters

A numerical simulation should not be considered trustworthy simply because the trajectory looks reasonable.

The validation framework provides a way to quantitatively evaluate the numerical solution against known physical or analytical expectations.

This will be expanded substantially in later iterations.

### Output

#### Files

- `Output.h`
- `Output.cpp`

The Output module is responsible for displaying simulation results.

It provides functions for:

Converting the integrator enumeration into a readable name
Printing the timestep results
Printing a simulation summary

Moving output formatting out of the simulation engine prevents `Simulation.cpp` from becoming responsible for both computation and presentation.

#### Why this matters

This is another example of separation of concerns.

The simulation should calculate results.

The output module should decide how those results are presented to the user.

This will make it easier to eventually replace console output with other forms of data visualization or file output.

## Program Flow

The Day 6 program follows this general sequence:

1. `main.cpp` creates a simulation configuration using a predefined test case.
2. The simulation is executed through `run_simulation()`.
3. The simulation initializes the spacecraft state.
4. At each timestep, the dynamics model calculates the state derivative.
5. The selected numerical integrator advances the state.
6. Diagnostic quantities are calculated and stored in a `SimulationStep`.
7. The resulting simulation history is returned to `main.cpp`.
8. The Output module displays the results.
9. The Validation module evaluates the simulation.

This creates a much cleaner separation between:

$$
\text{configuration}
\rightarrow
\text{simulation}
\rightarrow
\text{physics}
\rightarrow
\text{integration}
\rightarrow
\text{analysis}
\rightarrow
\text{output}
\rightarrow
\text{validation}
$$

## What Changed from Day 5



#### Day 5



#### Day 6



## Key Design Decisions

### Separation of dynamics and integration



### Separation of simulation and output



### Separation of test cases from simulation logic



### Dedicated validation module



## C++ Concepts Practiced



## Physics Concepts Practiced



## Limitations



## Lessons Learned



## Next Steps



## Project Structure

The Day 6 directory is organized as follows:

```text
day6/
├── include/
│   ├── Dynamics.h
│   ├── Integrator.h
│   ├── OrbitalMechanics.h
│   ├── Output.h
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
│   ├── Output.cpp
│   ├── Simulation.cpp
│   ├── TestCases.cpp
│   ├── Validation.cpp
│   ├── Vector2D.cpp
│   └── main.cpp
│
└── README.md
```

## Summary
