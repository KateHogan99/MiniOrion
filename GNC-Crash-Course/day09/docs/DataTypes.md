# Project Date Types

## `Vector2D`



## `State`

"What is the spacecraft doing right now?"

Position, velocity, mass if you're still using it, etc.

## `SimulationStep`



## `ManeuverRequest`

"What do I want the spacecraft to accomplish?"

Initial state, desired state, duration.

## `ControlConfig`

"What restrictions/parameters apply to the controller?"

Acceleration constraint, eventually PID parameters.

## `ControlCommand`



## `GuidanceCommand`



## `SimulationConfig`

"How should the simulation numerically/physically operate?"

Gravity parameter, timestep, integrator, etc.

## `ValidationConfig`

"What constitutes an acceptable result?"

Tolerances.

## `GNCValidationConfig`
