# Day 4 — Equations of Motion and Euler Integration

## Objective

Replace the direct constant-acceleration propagation approach with an explicit formulation of the spacecraft's equations of motion and a numerical integration method.

This iteration introduces the first numerical integrator: Euler's method.

## Concepts Studied

### Physics / Numerical Methods

- Equations of motion
- State derivatives
- First-order ordinary differential equations
- Euler integration
- Numerical propagation
- Time discretization

### C++

- `StateDerivative` structures
- Separating dynamics from integration
- Additional vehicle data structures
- Function-based architecture

## State Representation

The spacecraft state is represented as:

$$
\mathbf{x}
=
\begin{bmatrix}
\mathbf{r} \\
\mathbf{v}
\end{bmatrix}
$$

where:

- $\mathbf{r}$ is position
- $\mathbf{v}$ is velocity

The derivative of the state is:

$$
\dot{\mathbf{x}}
=
\begin{bmatrix}
\mathbf{v} \\
\mathbf{a}
\end{bmatrix}
$$

A `StateDerivative` structure was introduced to represent this explicitly.

## Equations of Motion

The simulation uses:

$$
\dot{\mathbf{r}} = \mathbf{v}
$$

and

$$
\dot{\mathbf{v}} =
\mathbf{a}_{gravity}
+
\mathbf{a}_{thrust}
$$

The gravitational acceleration remains:

$$
\mathbf{a}_{gravity}
=
-\frac{\mu}{r^3}\mathbf{r}
$$

## Euler Integration

Euler's method approximates the state at the next timestep using:

$$
\mathbf{x}_{n+1}
=
\mathbf{x}_n
+
\dot{\mathbf{x}}_n\Delta t
$$

The implementation separates this operation into:

- `calculate_state_derivative()`
- `euler_integrate()`
- `propagate_state()`

This separation is important because it allows the dynamics model and numerical integration method to be changed independently.

## Vehicle Model

A preliminary `Vehicle` structure was also introduced containing:

- Dry mass
- Propellant mass
- Maximum thrust
- Specific impulse

At this stage, these values are not yet used to model propellant consumption or variable thrust.

A basic vehicle-mass calculation was implemented:

$$
m_{total}
=
m_{dry}
+
m_{propellant}
$$

This establishes the foundation for later work involving propulsion and $\Delta v$.

## Test Cases

The simulation continues to support:

1. No gravity
2. Gravity only
3. Circular orbit

The circular-orbit case demonstrates an important limitation of Euler integration: numerical error accumulates over time, and the simulated orbit does not perfectly preserve the ideal circular trajectory.

## What I Learned

The most important concept from this iteration was separating:

**Dynamics**

> What are the derivatives of the spacecraft state?

from:

**Integration**

> Given those derivatives, how should the state be advanced in time?

This separation is fundamental to numerical simulation and makes it possible to compare different integration algorithms using the same equations of motion.

## Limitations

Euler integration is simple and useful for learning, but it introduces significant numerical error for orbital propagation.

The simulation does not yet:

- Compare multiple integrators
- Quantify numerical error
- Automatically validate results
- Model changing vehicle mass
- Model actual thrust force
- Perform guidance or control

## Next Step

The next iteration will implement a higher-order numerical integration method and introduce simulation data structures that make trajectory analysis and validation possible.
