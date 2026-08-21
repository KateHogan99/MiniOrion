# Physics Module

## Overview

The Physics module provides the mathematical and physical foundations
used by the spacecraft simulation.

It contains three primary components:

- `Vector2D` — two-dimensional vector mathematics.
- `State` — spacecraft position, velocity, and mass representation.
- `OrbitalMechanics` — analytical orbital mechanics and gravitational
  calculations.

The current simulation uses a two-dimensional Cartesian coordinate system,
SI units, and an idealized point-mass gravitational model.

---

## Coordinate System

The simulation represents spacecraft motion in a two-dimensional Cartesian
coordinate system.

Position:

    r = (x, y)

Velocity:

    v = (vx, vy)

Acceleration:

    a = (ax, ay)

The coordinate origin represents the center of the central gravitational
body.

All physical quantities use SI units:

| Quantity | Unit |
|----------|------|
| Position | m |
| Velocity | m/s |
| Acceleration | m/s² |
| Mass | kg |
| Time | s |
| Gravitational parameter | m³/s² |

---

# Vector2D

`Vector2D` provides the fundamental mathematical operations used by the
rest of the simulation.

## Vector Addition

    a + b = (ax + bx, ay + by)

## Vector Subtraction

    a - b = (ax - bx, ay - by)

## Scalar Multiplication

    c a = (c ax, c ay)

## Magnitude

    |a| = sqrt(ax² + ay²)

These operations are intentionally kept independent of spacecraft physics
so they can be reused by every higher-level module.

---

# State

The `State` structure represents the physical state of the spacecraft
at a particular instant.

    State = {position, velocity, mass}

Position and velocity describe translational motion. Mass is included so
that future propulsion models can account for changing spacecraft mass.

---

# Orbital Mechanics

The `OrbitalMechanics` module provides analytical relationships used to
model and analyze gravitational motion.

## Gravitational Parameter

The simulation represents a central body using its standard gravitational
parameter:

    μ = GM

where:

- G is the universal gravitational constant.
- M is the mass of the central body.

---

## Point-Mass Gravity

The gravitational acceleration is:

    a_g = -μ/r³ * r

where:

    r = ||r||

The negative sign causes the acceleration vector to point toward the
central body.

The point-mass model becomes undefined at r = 0, which the implementation
rejects.

---

## Specific Orbital Energy

Specific orbital energy is:

    ε = v²/2 - μ/r

For an ideal two-body system without thrust, specific orbital energy
should remain constant.

The sign identifies the broad orbital regime:

- ε < 0 — bound orbit
- ε = 0 — parabolic escape
- ε > 0 — hyperbolic trajectory

This provides an important analytical validation quantity for orbital
experiments.

---

## Specific Angular Momentum

In the two-dimensional model:

    h = x vy - y vx

The result represents the component of specific angular momentum
perpendicular to the simulation plane.

Its sign indicates the direction of orbital motion.

---

## Orbital Eccentricity

Eccentricity is calculated using:

    e = sqrt(1 + 2εh²/μ²)

The resulting value characterizes the orbit:

- e = 0 — circular
- 0 < e < 1 — elliptical
- e = 1 — parabolic
- e > 1 — hyperbolic

---

## Circular Orbital Velocity

For a circular orbit:

    v_circular = sqrt(μ/r)

This relationship is useful for constructing initial conditions for
circular-orbit experiments.

---

## Escape Velocity

Escape velocity is:

    v_escape = sqrt(2μ/r)

This provides a useful analytical reference when constructing escape
trajectory experiments.

---

# Physics Model Limitations

The current physics model intentionally simplifies real spacecraft
dynamics.

It currently assumes:

- two-dimensional motion
- a single central gravitational body
- point-mass gravity
- Newtonian mechanics
- no atmospheric drag
- no third-body gravity
- no relativistic effects
- no rotational dynamics
- no attitude dynamics
- simplified propulsion

These limitations are intentional. The purpose of the current project is
to establish a clear and testable numerical simulation framework before
adding additional physical complexity.
