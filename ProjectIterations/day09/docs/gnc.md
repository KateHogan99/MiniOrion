# Guidance, Navigation, and Control

## Overview

The GNC portion of the library determines how the simulated spacecraft
should move and converts that desired motion into a bounded acceleration
command.

The current implementation contains:

- `Guidance`
- `Control`

Navigation is not yet implemented. A future navigation component may
introduce state estimation using simulated measurements and a Kalman filter.

The current architecture is:

    Guidance → Control → Simulation

The spacecraft state flows in the opposite direction:

    Simulation → Guidance

This creates a feedback loop in which Guidance can compare the current
spacecraft state with the desired trajectory.

---

# Guidance

## Purpose

Guidance determines the desired trajectory of the spacecraft.

It answers:

> Where should the spacecraft be, and how should it be moving, at this
> point in the maneuver?

The current implementation uses a cubic polynomial trajectory.

---

## Maneuver Definition

A maneuver is defined by:

- initial position
- initial velocity
- desired final position
- desired final velocity
- maneuver duration

These form four boundary conditions for each spatial axis.

---

# Cubic Trajectory

For each axis, the reference position is:

    r(t) = r₀ + v₀t + c₂t² + c₃t³

The corresponding velocity is:

    v(t) = v₀ + 2c₂t + 3c₃t²

The corresponding acceleration is:

    a(t) = 2c₂ + 6c₃t

The coefficients are selected so that:

    r(0) = r₀
    v(0) = v₀
    r(T) = r_f
    v(T) = v_f

This allows the trajectory to satisfy both position and velocity
requirements at the beginning and end of the maneuver.

---

# Guidance Command

A `GuidanceCommand` contains:

- position tracking error
- velocity tracking error
- desired position
- desired velocity
- desired acceleration
- remaining maneuver time

The tracking errors are defined as:

    position_error = desired_position - current_position

    velocity_error = desired_velocity - current_velocity

The desired acceleration is the acceleration of the reference trajectory,
not necessarily the acceleration that the spacecraft will actually
receive.

This distinction is important because the Control module may constrain
the requested acceleration.

---

# Control

## Purpose

Control converts the desired acceleration from Guidance into a physically
bounded spacecraft command.

It answers:

> Given the desired acceleration, what acceleration can the spacecraft
> actually command?

---

## Acceleration Constraint

The current controller imposes a maximum acceleration magnitude.

If:

    |a_desired| <= a_max

the requested acceleration is passed through unchanged.

If:

    |a_desired| > a_max

the vector is scaled:

    a_command =
        a_desired * (a_max / |a_desired|)

This preserves the direction of the desired acceleration while limiting
its magnitude.

---

# Current Control Architecture

The current control system is intentionally simple.

    Guidance
       │
       │ desired acceleration
       ▼
    Control
       │
       │ acceleration constraint
       ▼
    ControlCommand
       │
       ▼
    Simulation

This provides a simple open-loop control mechanism while establishing an
interface that can later support feedback controllers.

---

# Future PID Control

A PID controller is planned as a future extension of the Control layer.

The eventual controller can use tracking error to generate a corrective
control command.

A PID controller has three terms:

    u(t) =
        Kp e(t)
        + Ki ∫e(t)dt
        + Kd de(t)/dt

where:

- Kp is the proportional gain
- Ki is the integral gain
- Kd is the derivative gain
- e(t) is the tracking error

The PID controller would operate inside the Control layer rather than
changing the fundamental Guidance interface.

This allows the current trajectory generator to remain responsible for
reference generation while the controller determines how to track it.

---

# Future Navigation and Kalman Filtering

A future navigation component may simulate imperfect measurements of the
spacecraft state and estimate the actual state from those measurements.

A Kalman filter is a candidate algorithm for this layer.

The conceptual architecture would become:

    True spacecraft state
             │
             ▼
       Measurement model
             │
             ▼
        Kalman Filter
             │
             ▼
       Estimated state
             │
             ▼
         Guidance
             │
             ▼
          Control
             │
             ▼
        Spacecraft

The Kalman filter is therefore conceptually different from the current
Guidance and Control algorithms.

It would estimate:

> Where am I?

Guidance determines:

> Where should I go?

Control determines:

> What should I command to get there?

---

# Design Philosophy

The GNC architecture intentionally separates three responsibilities:

### Guidance

Generates the desired trajectory.

### Navigation

Estimates the spacecraft's state.

### Control

Generates commands that cause the spacecraft to follow the desired
trajectory.

Only Guidance and Control are currently implemented.

This separation is intended to allow increasingly sophisticated algorithms
to be introduced without rewriting the simulation or physics layers.
