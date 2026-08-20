# Simulation Validation

## Generic validation

`Validation.cpp`

- scalar comparisons
- vector error
- relative/absolute error
- tolerance checks
- convergence error

## GNC validation

`GNCValidation.cpp`

- boundary conditions
- intermediate trajectory
- acceleration constraints
- command duration
- final position
- final velocity
- simulated-vs-planned trajectory

## Physics validation

`PhysicsValidation.cpp`

- gravity direction
- circular orbit behavior
- orbital radius
- orbital period
- conservation checks where appropriate

## Numerical validation

`NumericalValidation.cpp`

- Euler convergence
- RK4 convergence
- observed convergence order
- timestep sensitivity
