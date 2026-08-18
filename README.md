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
