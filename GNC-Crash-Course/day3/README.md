# Day 3 — Gravitational Dynamics

## Objective

Extend the constant-acceleration simulation into a basic 2D gravitational orbital mechanics model.

The major change in this iteration is that acceleration is no longer necessarily constant. Instead, gravitational acceleration is calculated from the spacecraft's current position.

## Concepts Studied

### Physics

- Newtonian point-mass gravity
- Gravitational parameter ($\mu$)
- Radial distance
- Inverse-square gravitational acceleration
- Orbital motion
- Circular orbital velocity

### C++

- Configuration structures
- Test-case functions
- Runtime error handling
- Separating simulation configuration from simulation execution
- Basic input-driven program flow

## Gravitational Model

The simulation uses the standard two-body point-mass gravitational model.

The gravitational acceleration is:

$$
\mathbf{a}\_g =
-\frac{\mu}{r^3}\mathbf{r}
$$

where:

- $\mathbf{r}$ is the spacecraft position vector
- $r = |\mathbf{r}|$ is the spacecraft's distance from the central body
- $\mu$ is the gravitational parameter

The implementation calculates the spacecraft's current radius and uses it to determine the acceleration at that location.

## Why $\mu$ Is Used

Rather than separately representing the gravitational constant and central-body mass, the simulation uses:

$$
\mu = GM
$$

This is the conventional gravitational parameter used in orbital mechanics.

It also makes the equations more convenient for a spacecraft simulation.

## Thrust

The model also allows an externally specified thrust acceleration.

The total acceleration is calculated as:

$$
\mathbf{a}\_{total}
=
\mathbf{a}\_{gravity}
+
\mathbf{a}\_{thrust}
$$

This establishes the basic structure needed for future powered-flight simulations.

## Test Cases

Three initial scenarios were implemented.

### 1. No Gravity

$$
\mu = 0
$$

The spacecraft experiences only the specified thrust acceleration.

This provides a simple comparison with the constant-acceleration model from the previous iterations.

### 2. Gravity Only

The spacecraft experiences gravitational acceleration with no thrust.

This demonstrates that acceleration changes as the spacecraft moves.

### 3. Circular Orbit

The initial velocity is selected using:

$$
v_c = \sqrt{\frac{\mu}{r}}
$$

For the selected initial conditions, this produces an ideal circular orbit in the continuous two-body model.

The simulation uses a smaller timestep for this case to reduce numerical propagation error.

## Error Handling

The gravitational model is undefined at:

$$
r = 0
$$

The program therefore throws an exception if gravitational acceleration is requested while the spacecraft is at the origin.

This also prevents invalid initial conditions when gravity is enabled.

## Simulation Output

The simulation reports:

- Position
- Velocity
- Speed
- Gravity acceleration
- Thrust acceleration
- Total acceleration
- Distance traveled during the current timestep
- Final displacement
- Total distance traveled

## What I Learned

This iteration changed the problem from simple kinematics into a dynamical simulation.

Acceleration is now a function of the spacecraft's current state:

$$
\mathbf{a} = f(\mathbf{r}, \mathbf{v}, t)
$$

This is an important conceptual transition because future numerical integration methods will need to evaluate the dynamics repeatedly as the spacecraft moves through the simulation.

## Limitations

The propagation method is still based directly on the constant-acceleration equations.

This is not appropriate for accurately propagating a general gravitational trajectory because gravitational acceleration changes throughout the timestep.

The next iteration will address this by representing the equations of motion as state derivatives and introducing numerical integration.

## Next Step

Implement the equations of motion in derivative form and introduce an explicit numerical integration method.
