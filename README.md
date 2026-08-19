# MiniOrion

> A from-scratch C++ spacecraft dynamics and GNC learning project.

**Status: Work in Progress — currently developing Iteration 7 of a planned 12-iteration project.**

---

## About This Project

Mini-Orion is a personal learning project focused on developing practical skills in:

- C++
- Numerical methods
- Orbital mechanics
- Spacecraft dynamics
- Guidance, Navigation, and Control (GNC)
- Scientific and engineering software development

The project is being developed incrementally, starting with basic two-dimensional kinematics and building toward a small spacecraft simulation with a GNC-oriented architecture.

The goal is not to reproduce a professional spacecraft flight software system. Instead, the goal is to build a technically credible, understandable project that demonstrates my ability to independently learn unfamiliar technical material, translate mathematical concepts into working software, validate numerical results, and progressively improve the architecture of a C++ application.

---

# About Me

I am a software and systems engineer transitioning toward work involving C++, aerospace, GNC, and physics-based simulation.

My previous professional experience has primarily involved engineering work in the defense sector and government-contract environments. My programming experience includes Python, MATLAB, data analysis, and engineering-oriented computational work.

C++ and spacecraft GNC are areas I am deliberately developing outside of my previous professional experience.

Rather than treating the transition as something I can accomplish by simply listing C++ or GNC on a resume, I wanted to build something substantial enough to demonstrate the work involved in learning these subjects.

Mini-Orion is that project.

---

# Why I Started Mini-Orion

I wanted a project that would force me to learn several things simultaneously:

1. How to write increasingly sophisticated C++.
2. How to translate physics and mathematics into software.
3. How numerical integration actually works.
4. How orbital mechanics can be modeled computationally.
5. How to determine whether a numerical simulation is behaving correctly.
6. How to structure a growing C++ project so that it remains maintainable.
7. How the pieces of a spacecraft GNC system fit together.

I am intentionally building the project incrementally rather than starting with an existing simulation framework.

The early iterations are therefore intentionally simple and sometimes inelegant. They document the process of learning and provide a progression from basic programming concepts toward a more complete engineering application.

---

# Project Goals

## Learning Goals

By the end of the project, I want to have developed a working understanding of:

### C++

- Structs and classes
- References and pointers
- Object-oriented design where appropriate
- Header/source separation
- Namespaces
- Enumerations
- Standard library containers
- Error handling
- File I/O
- CMake and build systems
- Testing
- Debugging
- Basic software architecture
- Writing maintainable scientific/engineering code

### Mathematics and Numerical Methods

- Vectors and coordinate systems
- Ordinary differential equations
- State-space representations
- Numerical integration
- Euler integration
- Runge-Kutta methods
- Numerical error
- Absolute and relative tolerances
- Convergence
- Stability
- Validation and verification

### Orbital Mechanics

- Two-body dynamics
- Gravitational acceleration
- Orbital elements
- Specific orbital energy
- Specific angular momentum
- Eccentricity
- Circular, elliptical, and hyperbolic trajectories
- Orbital propagation
- Maneuvers and Δv
- Reference frames and coordinate transformations

### GNC

I ultimately want to develop a foundational understanding of the three major GNC functions:

**Guidance**

Determining where the spacecraft should go and what trajectory or maneuver is required.

**Navigation**

Determining the spacecraft's estimated state:

- Position
- Velocity
- Potentially attitude and other state variables

**Control**

Determining what commands or forces should be applied to make the spacecraft follow the desired trajectory.

The final project will not attempt to reproduce the complexity of a real spacecraft GNC system. Instead, it will implement simplified versions of these concepts appropriate to the scope of this project.

---

# Final Project Goal

The eventual goal for Mini-Orion is a small but coherent two-dimensional spacecraft simulation with a basic GNC architecture.

The intended system will eventually contain components representing:

```text
             Mission / Scenario
                    │
                    ▼
                Guidance
                    │
             Desired State
                    │
                    ▼
                Controller
                    │
              Command / Δv
                    │
                    ▼
             Spacecraft Dynamics
                    │
                    ▼
                 Truth State
                    │
                    ▼
               Navigation
                    │
              Estimated State
                    │
                    └───────────────┐
                                    │
                                    ▼
                                Guidance
```

The exact architecture will evolve as I learn more.

The final system is intended to demonstrate concepts rather than simulate a flight-qualified spacecraft.

---

# Current Scope

The project currently uses a simplified two-dimensional model.

The current simulation includes concepts such as:

- Two-dimensional position and velocity
- Point-mass gravitational dynamics
- Prescribed thrust acceleration
- State derivatives
- Numerical propagation
- Euler integration
- RK4 integration
- Circular orbits
- Elliptical trajectories
- Escape trajectories
- Specific orbital energy
- Specific angular momentum
- Numerical validation
- Configurable simulation scenarios

The project is currently focused on building a reliable dynamics and propagation foundation before adding higher-level GNC functionality.

---

# Development Philosophy

Mini-Orion is being developed as a learning project rather than as a demonstration of pre-existing expertise.

Each iteration has a specific technical objective.

The progression is intentional:

```text
Basic C++ / Kinematics
        ↓
Vector Mathematics
        ↓
Gravity
        ↓
State Derivatives
        ↓
Numerical Integration
        ↓
Orbital Analysis
        ↓
Modular Architecture
        ↓
Numerical Validation
        ↓
Improved Simulation
        ↓
Guidance
        ↓
Control
        ↓
Integrated GNC Demonstration
```

The project is designed so that earlier iterations remain available as a record of that progression.

---

# Development Status

## Current Iteration: 8 / 12

The project has progressed from a single-file kinematics demonstration to a modular two-dimensional orbital simulation with numerical integration and independent validation routines.

### Completed

- [x] Basic C++ kinematics
- [x] Vector-based state representation
- [x] Two-dimensional gravitational model
- [x] Spacecraft state representation
- [x] State derivative formulation
- [x] Euler integration
- [x] RK4 integration
- [x] Simulation result storage
- [x] Orbital energy calculation
- [x] Angular momentum calculation
- [x] Circular orbit test case
- [x] Elliptical orbit test case
- [x] Escape trajectory test case
- [x] Analytical constant-acceleration validation
- [x] Conservation-based validation
- [x] Modular C++ project structure
- [x] Validation framework

### In Progress

- [ ]  Improve validation tolerance handling
- [ ]  Relative error analysis
- [ ]  More systematic numerical testing
- [ ]  Better visualization of simulation results
- [ ]  Improved project documentation
- [ ]  Additional orbital mechanics functionality

### Planned

- [ ]  More rigorous integrator comparison
- [ ]  Numerical convergence analysis
- [ ]  Maneuver / Δv modeling
- [ ]  Guidance logic
- [ ]  Basic control system
- [ ]  Navigation / state estimation concepts
- [ ]  Integrated GNC demonstration
- [ ]  Final portfolio documentation

This checklist will be updated as the project develops.

---

# Project Structure

The repository is currently organized around two major areas:

```text
MiniOrion/
│
├── README.md
│
└── GNC-Crash-Course/
    │
    ├── day01/
    ├── day02/
    ├── day03/
    ├── day04/
    ├── day05/
    ├── day06/
    ├── day07/
    ├── day08/
    ├── day09/
    ├── day10/
    ├── day11/
    └── day12/
```

The `GNC-Crash-Course` directory documents the project's incremental development.

Each iteration focuses on a specific set of programming, numerical, or GNC concepts.

---

# Navigating the GNC Crash Course

The individual iteration directories are intended to be read chronologically.

## Day 1 — Basic Kinematics

Introduces:

- C++ fundamentals
- Structures
- Functions
- Position
- Velocity
- Acceleration
- Constant-acceleration equations

Start here to see the original baseline implementation.

---

## Day 2 — Vector Mathematics

Introduces:

- `Vector2D`
- Vector addition
- Vector subtraction
- Scalar multiplication
- Vector magnitude
- Vector-based state representation

This establishes the mathematical representation used by the later orbital mechanics code.

---

## Day 3 — Gravitational Dynamics

Introduces:

- Gravitational parameter μ
- Point-mass gravity
- Position-dependent acceleration
- Orbital motion
- Initial orbital test cases

This is where the project first becomes an orbital mechanics simulation.

---

## Day 4 — State Derivatives and Euler Integration

Introduces:

- State derivatives
- Dynamics formulation
- Forward Euler integration
- Separation of dynamics from propagation

This establishes the numerical propagation framework.

---

## Day 5 — RK4 and Orbital Analysis

Introduces:

- Fourth-order Runge-Kutta integration
- Integrator selection
- Simulation history
- Specific orbital energy
- Specific angular momentum
- Analytical validation
- More structured simulation output

This is the point where the simulation begins to resemble a numerical dynamics application rather than a simple physics exercise.

---

## Day 6 — C++ Project Architecture

Introduces:

- Header/source separation
- Modularization
- Separation of concerns
- Reusable components
- Project organization

This iteration focuses primarily on software engineering rather than adding another major physics model.

---

## Day 7 — Numerical Validation

Introduces:

- Validation architecture
- Analytical validation
- Conservation-law validation
- Orbital test cases
- PASS / FAIL / N/A results
- Error tolerances
- Relative error concepts

This iteration asks a more important engineering question:

> How do I know that the simulation is producing trustworthy results?

---

## Day 8 — Guidance, Control, and GNC Validation

Introduces:

- Introduce a basic Guidance module.
- Introduce a basic Control module.
- Connect Guidance and Control to the existing simulation.
- Implement a cubic trajectory for point-to-point maneuvers.
- Constrain commanded acceleration to a maximum value.
- Preserve the validation-first mindset established in Day 7.

The major goal of this iteration was to move beyond simply simulating a spacecraft's motion and begin answering a more important question:

> Can the system determine where the spacecraft should go, generate an appropriate control command, execute that command through the simulation, and validate that the resulting trajectory behaves as expected?

---

## Days 9–12

These iterations are still under development.

They will progressively expand the project toward:

- More rigorous numerical analysis
- Orbital mechanics
- Maneuvers
- Guidance
- Control
- Navigation
- Integrated GNC behavior
- Final portfolio presentation

The exact implementation will evolve as I learn more.

---

# Technical Approach

The project is intentionally being developed from relatively fundamental components rather than relying on a high-level aerospace simulation framework.

The core numerical loop is conceptually:

```text
Current State
     │
     ▼
Dynamics Model
     │
     ▼
State Derivative
     │
     ▼
Numerical Integrator
     │
     ▼
Next State
     │
     ▼
Validation / Analysis
```

This structure allows individual pieces of the simulation to be tested and improved independently.

---

# Validation Philosophy

A major goal of this project is to avoid treating "the program runs without crashing" as evidence that the simulation is correct.

Where possible, the simulation is validated using independent physical or mathematical expectations.

Examples include:

### Analytical Solutions

Comparing numerical propagation against a known analytical solution for constant acceleration.

### Conservation Laws

Checking whether quantities that should remain constant in an ideal two-body system remain sufficiently constant numerically.

Examples:

- Specific orbital energy
- Specific angular momentum

### Physical Expectations

Checking whether trajectories behave as expected for:

- Circular orbits
- Elliptical orbits
- Hyperbolic / escape trajectories

As the project develops, I intend to make these tests increasingly quantitative.

# Why the Project Is 2D

The current project deliberately uses two-dimensional motion.

This keeps the mathematical and software complexity manageable while I learn the underlying concepts.

A three-dimensional implementation introduces additional considerations including:

- 3D vector mathematics
- Reference frames
- Coordinate transformations
- Orbital plane geometry
- 3D visualization
- Attitude-related concepts

The long-term goal is to understand these concepts, but adding a 3D visualization simply for appearance would not necessarily make the GNC simulation more meaningful.

I am therefore prioritizing correct and well-understood dynamics and GNC concepts over visual complexity.

A 3D extension may be considered after the core two-dimensional system is complete.

---

# What I Am Trying to Demonstrate

This project is intended to demonstrate more than the ability to produce working C++ code.

In particular, I want the finished project to demonstrate that I can:

- Teach myself an unfamiliar technical subject.
- Translate mathematical equations into software.
- Learn and apply C++ incrementally.
- Work with numerical methods.
- Build and analyze physical simulations.
- Identify limitations in my own implementation.
- Validate numerical results independently.
- Refactor code as requirements become more complex.
- Document technical decisions.
- Investigate problems rather than hiding them.
- Build toward a larger engineering system incrementally.

The progression itself is part of the project.

---

# Current Limitations

MiniOrion is an educational and portfolio project, not flight software.

The current implementation does not model many aspects of a real spacecraft system, including:

- Three-dimensional dynamics
- Spacecraft attitude
- Rotational dynamics
- Reaction wheels or control moment gyroscopes
- Realistic propulsion hardware
- Mass depletion
- Atmospheric drag
- J2 perturbations
- Third-body gravity
- Solar radiation pressure
- Real navigation sensors
- Kalman filtering
- Flight computers
- Fault management
- Real-time constraints
- Hardware interfaces

These may be appropriate topics for future learning, but they are outside the current scope unless they directly support the project's final goals.

---

# Portfolio Goal

The intended final product is a small, self-contained GNC simulation that can be shown during a technical interview.

The final repository should allow someone unfamiliar with the project to understand:

1. What problem the software is solving.
2. What physics it models.
3. How the numerical propagation works.
4. How the simulation is validated.
5. How guidance and control interact with the dynamics.
6. What assumptions and limitations exist.
7. Why particular implementation decisions were made.

The final project should be understandable enough that I can explain the architecture and mathematics during an interview rather than simply demonstrating that the program runs.

---

# Feedback Welcome

This project is being developed publicly as a learning exercise.

Feedback is particularly useful regarding:

- C++ design
- Numerical methods
- Orbital mechanics
- GNC architecture
- Validation methodology
- Software organization
- Engineering practices
- Areas where my assumptions or implementation are incorrect

I am especially interested in feedback that identifies weaknesses or technically questionable decisions. The purpose of the project is to improve my understanding, not simply to produce a predetermined result.

---

# Project Status

Mini-Orion is an active work in progress.

The current implementation represents approximately the first half of the planned learning progression. The repository will continue to evolve as new concepts are learned, implemented, tested, and documented.
