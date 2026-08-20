# Intended Final Product

A modular 2D spacecraft GNC simulation framework written in C++, capable of running configurable spacecraft dynamics and GNC experiments, validating simulation results against analytical and numerical expectations, and exporting simulation data for Python/MATLAB visualization and analysis.

## Capabilities

### Core Simulation

- 2D translational spacecraft state
- Position and velocity propagation
- Gravity
- Thrust acceleration
- Euler integration
- RK4 integration
- Configurable timestep
- Configurable simulation duration

### GNC

Already implemented:

- Cubic-trajectory guidance
- Acceleration-limited control

Future:

- PID control
- Potentially Kalman-filter-based state estimation

### Experiments

At minimum:

- Maneuver experiment
- Orbit experiment
- Integration convergence experiment

Potentially later:

- PID experiment
- State-estimation experiment
- GNC closed-loop experiment

### Validation

- Mathematical/boundary-condition validation
- Intermediate trajectory validation
- Final-state validation
- Acceleration constraint validation
- Maneuver-duration validation
- Numerical convergence validation
- Physics/orbital validation
- Configuration validation

### Output

- Human-readable console output
- Machine-readable simulation data
- Python/MATLAB visualization
- Experiment-specific plots
