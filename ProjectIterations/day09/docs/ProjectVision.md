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

## Architecture

The final product show follow this data flow:

```text
                        Experiment
                             │
                             ▼
                      Configuration
                             │
                             ▼
                         Simulation
                             │
             ┌───────────────┼───────────────┐
             ▼               ▼               ▼
         Guidance         Control         Dynamics
             │               │               │
             └───────────────┼───────────────┘
                             ▼
                        Integrator
                             │
                             ▼
                     Simulation Results
                             │
                    ┌────────┴────────┐
                    ▼                 ▼
                Validation           Output
                                      │
                             ┌────────┴────────┐
                             ▼                 ▼
                           Python           MATLAB
```

### Dependencies

Dependencies should generally point downward. 

For example:

- `Guidance` can use physics/state types.
- `Control` can use Guidance output.
- `Simulation` can use Dynamics and Integrator.
- `Validation` can inspect simulation results.
- `Output` can display/export results.

But:

- Physics should not know about experiments.
- Dynamics should not know about validation.
- Integrators should not know about console output.
- Validation should not perform simulation.
- Output should not calculate physics.
