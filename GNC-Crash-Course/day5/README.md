# Day 5 — Numerical Integration and Orbital Analysis

## Objective

Introduce numerical integration into the orbital simulation and begin treating the program as a numerical dynamics system rather than a collection of analytical equations.

The primary goals of this iteration were:

- Implement Euler integration.
- Implement fourth-order Runge-Kutta integration (RK4).
- Create a common state-derivative representation.
- Allow the simulation to select between integration methods.
- Record simulation results for later analysis.
- Calculate specific orbital energy.
- Calculate specific angular momentum.
- Add an analytical solution for validation.
- Begin separating simulation, analysis, and validation concepts.

---

## What I Built

Day 5 is the first iteration where the simulation has a general numerical propagation structure.

The spacecraft state is represented as:

$$
\mathbf{x} =
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
\dot{\mathbf{x}} =
\begin{bmatrix}
\mathbf{v} \\
\mathbf{a}
\end{bmatrix}
$$

where acceleration is determined by the dynamics model.

For the current two-body model with optional thrust:

$$
\mathbf{a} = \mathbf{a}_{gravity} + \mathbf{a}_{thrust}
$$

---

# State Derivatives

The state derivative is represented explicitly rather than calculating position and velocity updates independently.

Conceptually:

$$
\dot{\mathbf{r}}=\mathbf{v}
$$

and

$$
\dot{\mathbf{v}}=\mathbf{a}
$$

This representation is important because numerical integrators operate on derivatives of the state.

### Code Reference

> **Add code snippet here:** `StateDerivative` structure and `calculate_state_derivative()` implementation from `main.cpp`.

---

# Euler Integration

The first numerical integrator implemented in this iteration is the forward Euler method.

For a general state:

$$
\mathbf{x}_{n+1} = \mathbf{x}_n + \dot{\mathbf{x}}_n\Delta t
$$

For position:

$$
\mathbf{r}_{n+1} = \mathbf{r}_n + \mathbf{v}_n\Delta t
$$

For velocity:

$$
\mathbf{v}_{n+1} = \mathbf{v}_n + \mathbf{a}_n\Delta t
$$

Euler integration is simple and computationally inexpensive, but its accuracy is limited.

It is particularly useful in this project as a baseline against which a higher-order method can be compared.

### Code Reference

> **Add code snippet here:** `euler_integrate()` implementation from `main.cpp`.

---

# Runge-Kutta 4 Integration

The second integrator implemented is classical fourth-order Runge-Kutta (RK4).

RK4 evaluates the state derivative four times during each timestep:

$$
k_1=f(t_n,x_n)
$$

$$
k_2=f\left(t_n+\frac{\Delta t}{2},
x_n+\frac{\Delta t}{2}k_1\right)
$$

$$
k_3=f\left(t_n+\frac{\Delta t}{2},
x_n+\frac{\Delta t}{2}k_2\right)
$$

$$
k_4=f(t_n+\Delta t,
x_n+\Delta tk_3)
$$

The final state is calculated using the weighted combination:

$$
x_{n+1} = x_n + \frac{\Delta t}{6}\left(k_1 + 2k_2 + 2k_3 + k_4\right)
$$

The four evaluations allow RK4 to obtain a much better approximation of the changing dynamics within the timestep than forward Euler.

### Code Reference

> **Add code snippet here:** `rk4_integrate()` implementation from `main.cpp`.

---

# Why RK4 Matters

Orbital dynamics are nonlinear.

The gravitational acceleration depends on the spacecraft's current position:

$$
\mathbf{a}_{gravity}
=
-\frac{\mu}{r^3}\mathbf{r}
$$

As the spacecraft moves, its position changes, which changes its acceleration.

Therefore, the assumption that acceleration remains constant throughout a timestep becomes increasingly inaccurate.

RK4 addresses this by evaluating the dynamics at multiple points within each timestep.

This makes it substantially more accurate than Euler for the same timestep in many ordinary orbital propagation problems.

---

# Integrator Selection

The simulation introduces an `Integrator` enumeration with two options:

- Euler
- RK4

The propagation function selects the appropriate integration method based on the configuration.

### Code Reference

> **Add code snippet here:** `Integrator` enum and `propagate_state()` implementation from `main.cpp`.

---

# Orbital Mechanics

Day 5 expands the orbital analysis beyond position and velocity.

The simulation calculates two important conserved quantities for an ideal two-body orbit.

---

## Specific Orbital Energy

Specific orbital mechanical energy is:

$$
\epsilon
=
\frac{v^2}{2}
-
\frac{\mu}{r}
$$

where:

- $\epsilon$ is specific orbital energy
- $v$ is spacecraft speed
- $\mu$ is gravitational parameter
- $r$ is distance from the central body

Specific orbital energy is energy per unit spacecraft mass.

For an ideal two-body system with no thrust:

$$
\epsilon = \text{constant}
$$

Therefore, monitoring energy provides a useful diagnostic for numerical propagation.

If the simulation produces a significant change in orbital energy when no physical mechanism should be changing it, this indicates numerical error or a problem with the implementation.

### Code Reference

> **Add code snippet here:** `calculate_specific_orbital_energy()` implementation from `main.cpp`.

---

# Specific Angular Momentum

Specific angular momentum is:

$$
\mathbf{h}
=
\mathbf{r}
\times
\mathbf{v}
$$

Because this project is currently two-dimensional, only the z-component is nonzero:

$$
h_z
=
xv_y-yv_x
$$

The simulation therefore stores angular momentum as a scalar.

For an ideal two-body system with no external torque:

$$
h = \text{constant}
$$

Like orbital energy, angular momentum provides an additional diagnostic of numerical accuracy.

### Code Reference

> **Add code snippet here:** `calculate_specific_angular_momentum()` implementation from `main.cpp`.

---

# Analytical Validation

The simulation retains the constant-acceleration analytical solution from previous iterations:

$$
\mathbf{r}(t)
=
\mathbf{r}_0
+
\mathbf{v}_0t
+
\frac{1}{2}\mathbf{a}t^2
$$

$$
\mathbf{v}(t)
=
\mathbf{v}_0
+
\mathbf{a}t
$$

This analytical solution is not used to propagate the spacecraft.

Instead, it provides an independent reference solution for a special case where the acceleration is constant.

The numerical result can then be compared against the analytical result.

### Code Reference

> **Add code snippet here:** `calculate_analytical_state()` implementation from `main.cpp`.

---

# Simulation Results

Day 5 introduces a `SimulationStep` structure to store the state and calculated quantities associated with each timestep.

Each recorded step contains:

- Time
- Position
- Velocity
- Gravity acceleration
- Thrust acceleration
- Total acceleration
- Radius
- Speed
- Specific orbital energy
- Specific angular momentum
- Distance traveled during the timestep
- Total accumulated distance

This is an important architectural change because the simulation is no longer required to immediately print every result.

Instead, results can be stored and analyzed after propagation.

---

# Test Cases

Three test cases are currently included.

## 1. No Gravity

The gravitational parameter is set to:

$$
\mu=0
$$

The spacecraft experiences only the prescribed thrust acceleration.

This case provides a situation where the constant-acceleration analytical solution can be used as a reference.

---

## 2. Gravity Only

The spacecraft experiences gravitational acceleration with no thrust:

$$
\mathbf{a}_{thrust}=0
$$

This allows the numerical integrator to be tested against a nonlinear dynamical system.

---

## 3. Circular Orbit

The initial velocity is selected using the circular-orbit relationship:

$$
v_c
=
\sqrt{\frac{\mu}{r}}
$$

The spacecraft begins with velocity perpendicular to its position vector.

For an ideal circular orbit:

$$
r=\text{constant}
$$

and the spacecraft should maintain a nearly constant orbital radius.

This provides another useful physical sanity check.

---

# Data Collection

The simulation stores the initial state at:

$$
t=0
$$

and then records every subsequent propagated state.

This produces a vector of `SimulationStep` objects containing the complete simulation history.

### Code Reference

> **Add code snippet here:** `run_simulation()` result-storage logic from `main.cpp`.

---

# Output

The program produces:

1. A timestep-by-timestep results table.
2. A simulation summary.
3. Analytical validation information for the constant-acceleration case.

The summary includes:

- Selected integrator
- Timestep
- Simulation duration
- Initial and final position
- Initial and final velocity
- Initial and final radius
- Final displacement
- Total distance traveled
- Initial and final orbital energy
- Change in orbital energy
- Initial and final angular momentum
- Change in angular momentum

This provides significantly more information for evaluating the behavior of the simulation.

---

# Important Numerical Concepts Learned

## Local vs. Global Error

Numerical integration introduces error at each timestep.

A numerical method's local truncation error describes the error introduced during an individual step, while global error describes how those errors accumulate over an entire simulation.

Euler is first-order accurate:

$$
Error \sim O(\Delta t)
$$

Classical RK4 is fourth-order accurate:

$$
Error \sim O(\Delta t^4)
$$

This means that reducing the timestep generally improves the accuracy of both methods, but RK4 should converge much more rapidly as the timestep decreases.

---

# Conservation as a Numerical Diagnostic

One of the most important lessons from this iteration is that physical conservation laws can be used to evaluate a numerical simulation.

For an ideal two-body system without thrust:

$$
\epsilon \approx \text{constant}
$$

and

$$
h \approx \text{constant}
$$

The values will not necessarily be perfectly constant because numerical integration introduces error.

The goal is therefore not exact equality, but controlled numerical deviation.

This concept will become important in later validation work.

---

# Key C++ Concepts Learned

- Enumerations with `enum class`
- Vectors of structures
- Returning complex structures from functions
- Passing configuration objects between functions
- Switch statements
- Numerical integration implementation
- Separation of propagation and analysis
- Storing simulation history
- Formatted console output
- `std::vector`
- `std::string`
- `static_cast`

---

# Key Physics and Numerical Concepts Learned

- State-space representation
- State derivatives
- Numerical integration
- Forward Euler
- Runge-Kutta 4
- Local and global numerical error
- Specific orbital energy
- Specific angular momentum
- Circular orbit velocity
- Two-body gravitational dynamics
- Conservation laws as validation tools
- Analytical versus numerical solutions

---

# Why This Matters for GNC

A spacecraft GNC system ultimately needs to predict how the spacecraft's state evolves over time.

That requires a dynamics model and a numerical propagation method.

The structure developed here is beginning to resemble that architecture:

$$
\text{State}
\rightarrow
\text{Dynamics}
\rightarrow
\text{State Derivative}
\rightarrow
\text{Numerical Integrator}
\rightarrow
\text{New State}
$$

The same basic pattern will later support more sophisticated components such as:

- Guidance
- Navigation
- Control
- State estimation
- Maneuver modeling
- Trajectory prediction

The current model is intentionally much simpler than a real spacecraft GNC system, but the underlying numerical structure is relevant.

---

# Limitations

The simulation is still a simplified two-dimensional model.

It currently assumes:

- Two-dimensional motion
- A single point-mass gravitational body
- Constant gravitational parameter
- No atmospheric effects
- No perturbations
- No spacecraft attitude
- No rotational dynamics
- No changing spacecraft mass
- No realistic propulsion model
- No guidance algorithm
- No navigation or state estimation
- No feedback controller

The thrust model is currently a prescribed acceleration rather than a physical propulsion system.

These limitations are intentional. The project is being developed incrementally, with each iteration introducing another fundamental concept.

---

# Next Steps

The next iterations will focus on turning the numerical simulation into a more robust engineering-style project.

Planned improvements include:

- More systematic validation
- Relative error and tolerance handling
- Comparison of numerical integrators
- Improved orbital test cases
- Better code organization
- More automated testing
- Visualization of trajectories and numerical error
- Additional orbital mechanics calculations

The long-term goal is to build a small but credible GNC-oriented spacecraft simulation that demonstrates independent learning in C++, numerical methods, physics, and orbital mechanics.
