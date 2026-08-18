# Day 2 — Vector-Based State Representation

## Objective

Refactor the basic kinematics model from Day 1 into a more physically meaningful vector-based representation.

The primary goals of this iteration were:

- Replace individual x/y quantities with a reusable `Vector2D` structure.
- Represent spacecraft position, velocity, and acceleration as vectors.
- Build reusable vector operations.
- Use vector mathematics to calculate displacement and speed.
- Continue practicing functions, structs, and state propagation in C++.
- Begin organizing the simulation around concepts that will eventually be useful for orbital mechanics.

---

## What I Built

Day 2 represents the spacecraft state using three two-dimensional vectors:

- Position
- Velocity
- Acceleration

Instead of treating the x and y components as separate physical quantities throughout the program, vector operations are used to manipulate them together.

The spacecraft state can therefore be represented conceptually as:

$$
\mathbf{r} = \begin{bmatrix}x \\y\end{bmatrix}
$$

$$
\mathbf{v} = \begin{bmatrix}v_x \\v_y\end{bmatrix}
$$

$$
\mathbf{a} = \begin{bmatrix}a_x \\a_y\end{bmatrix}
$$

The state is then propagated using the constant-acceleration equations:

$$
\mathbf{r}(t) = \mathbf{r}_0 + \mathbf{v}_0t + \frac{1}{2}\mathbf{a}t^2
$$

$$
\mathbf{v}(t) = \mathbf{v}_0 + \mathbf{a}t
$$

---

## Vector Mathematics

I implemented several basic vector operations.

### Vector Addition

For two vectors:

$$
\mathbf{a} = \begin{bmatrix}a_x\\a_y\end{bmatrix},\qquad\mathbf{b} =\begin{bmatrix}b_x\\b_y\end{bmatrix}
$$

their sum is:

$$
\mathbf{a} + \mathbf{b} = \begin{bmatrix}a_x + b_x\\a_y + b_y\end{bmatrix}
$$

This is used throughout the simulation to combine position, velocity, and acceleration contributions.

### Vector Subtraction

$$
\mathbf{a}-\mathbf{b}
=
\begin{bmatrix}
a_x-b_x \\
a_y-b_y
\end{bmatrix}
$$

This is used to determine the displacement vector between two positions.

### Scalar Multiplication

A vector can be multiplied by a scalar:

$$
c\mathbf{a}
=
\begin{bmatrix}
ca_x \\
ca_y
\end{bmatrix}
$$

This allows quantities such as velocity multiplied by time or acceleration multiplied by time to be represented naturally.

### Vector Magnitude

The magnitude of a two-dimensional vector is:

$$
|\mathbf{a}|
=
\sqrt{a_x^2+a_y^2}
$$

This is used to calculate quantities such as:

- Speed
- Displacement magnitude
- Distance from the origin

---

## Displacement

The displacement between two positions is calculated by first finding the displacement vector:

$$
\Delta\mathbf{r}
=
\mathbf{r}_f-\mathbf{r}_i
$$

and then calculating its magnitude:

$$
|\Delta\mathbf{r}|
=
\sqrt{
(\Delta x)^2+
(\Delta y)^2
}
$$

This differs from distance traveled. Displacement measures the straight-line separation between the initial and final positions.

---

## State Propagation

The spacecraft state is propagated one timestep at a time.

At each timestep:

1. Calculate the new position.
2. Calculate the new velocity.
3. Preserve the constant acceleration.
4. Store the new state.
5. Use the new state as the initial state for the next timestep.

### Code Reference

> **Add code snippet here:** `propagate_state()` implementation from `main.cpp`.

---

## What Changed From Day 1

Day 1 represented the spacecraft state using six individual scalar variables:

- x
- y
- vx
- vy
- ax
- ay

Day 2 groups related quantities into vectors.

This produces a more natural representation of physical state and makes the mathematical structure of the simulation clearer.

For example, instead of separately calculating x and y displacement, the program can calculate a displacement vector and then take its magnitude.

This is an important step toward the mathematical representation used in actual dynamics and guidance, navigation, and control systems.

---

## Test Configuration

The simulation begins with:

- Position: (100, 200)
- Velocity: (10, 5)
- Acceleration: (2, 1)
- Timestep: 1 second
- Simulation duration: 20 seconds

The acceleration remains constant throughout the simulation.

---

## Results

The simulation prints the following quantities at every timestep:

- Time
- Position
- Displacement from the initial position
- Velocity
- Speed
- Acceleration

The expected behavior is a continuously accelerating trajectory because the acceleration remains constant.

The trajectory is not yet an orbital simulation. No gravitational acceleration is included at this stage.

---

## Key Concepts Learned

### C++ Concepts

- `struct`
- Functions accepting structs
- Returning structs from functions
- Nested structures
- Passing vectors between functions
- Basic numerical computation
- Separating mathematical operations into reusable functions

### Physics Concepts

- Position
- Velocity
- Acceleration
- Displacement
- Speed
- Vector representation
- Constant-acceleration kinematics

---

## Why This Matters for the Project

This iteration establishes the mathematical representation that the later orbital mechanics model will build upon.

A spacecraft's position and velocity are naturally vectors, and treating them as vectors makes it possible to introduce:

- Gravity
- Orbital dynamics
- Numerical integration
- Orbital energy
- Angular momentum
- Trajectory propagation

without having to redesign the fundamental state representation later.

---

## Limitations

The model still assumes constant acceleration.

There is currently:

- No gravity
- No orbital mechanics
- No numerical integration
- No changing acceleration
- No vehicle dynamics
- No validation against an independent solution

These limitations are intentional. The purpose of Day 2 is to establish a reusable vector-based foundation before introducing more complicated dynamics.

---

## Next Iteration

Day 3 will introduce gravitational acceleration and begin transforming the kinematics demonstration into an actual orbital mechanics simulation.

The next major physics model will be point-mass gravitational acceleration:

$$
\mathbf{a}_g
=
-\frac{\mu}{r^3}\mathbf{r}
$$

where:

- $\mu$ is the gravitational parameter
- $\mathbf{r}$ is the spacecraft position vector
- $r=|\mathbf{r}|$

This will allow the simulation to model trajectories produced by gravity rather than only prescribed constant acceleration.
