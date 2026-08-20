# Experiments

## `ManeuverExperiment.cpp`

Responsible for:

1. Define maneuver scenario.
2. Define simulation configuration.
3. Define validation configuration.
4. Validate configuration.
5. Run guided maneuver.
6. Validate results.
7. Send results to output.

## `OrbitExperiment.cpp`

Responsible for:

1. Define orbital initial conditions.
2. Define simulation parameters.
3. Run simulation.
4. Invoke orbital validation.
5. Send results to output.

## `ConvergenceExperiment.cpp`

Responsible for:

1. Define one physical scenario.
2. Define list of timestep values.
3. Run Euler at each timestep.
4. Run RK4 at each timestep.
5. Collect errors.
6. Send results to output.
