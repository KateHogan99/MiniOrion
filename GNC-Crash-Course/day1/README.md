# Day 1 — Basic 2D Kinematics

## Objective

Build a minimal C++ program that propagates the position and velocity of a spacecraft through time under constant acceleration.

This iteration was intended to establish the basic physics and C++ concepts needed for the later orbital mechanics simulator.

## Concepts Studied

### Physics

- Position, velocity, and acceleration
- Constant-acceleration kinematics
- Displacement
- Speed
- Time-based state propagation

The primary equations used were:

### Position

$$
x(t) = x_0 + v_0t + \frac{1}{2}at^2
$$

### Velocity

$$
v(t) = v_0 + at
$$

For the 2D simulation, these equations are applied independently to the X and Y components.

## Implementation

The program defines a `State` structure containing:

- X position
- Y position
- X velocity
- Y velocity
- X acceleration
- Y acceleration

Several functions were created to separate the basic calculations:

- `calculate_position()`
- `calculate_velocity()`
- `calculate_displacement()`
- `calculate_speed()`
- `propagate_state()`

`propagate_state()` takes the current spacecraft state and advances it by a specified timestep.

## Simulation

The initial state is:

- Position: `(100, 200)`
- Velocity: `(10, 5)`
- Acceleration: `(2, 1)`

The simulation propagates the spacecraft in one-second increments for ten seconds.

At each timestep the program reports:

- Position
- Total displacement from the initial position
- Velocity
- Speed
- Acceleration

## What I Learned

This iteration introduced the basic structure of a state-propagation problem.

The main design lesson was separating individual physics calculations into functions instead of placing all calculations directly inside `main()`.

This also established the idea that a spacecraft's condition can be represented as a collection of state variables that are repeatedly propagated forward in time.

## Limitations

This is a deliberately simple model.

- Acceleration is constant.
- Gravity is not modeled.
- There is no numerical integration method yet.
- There are no orbital mechanics calculations.
- All code exists in a single source file.
- There is no automated validation.

These limitations are intentional and provide the starting point for later iterations.

## Next Step

The next iteration will introduce vector-based representations of position, velocity, and acceleration so that the simulation more closely resembles the mathematical representation used in orbital mechanics.
