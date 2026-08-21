/**
 * @file State.h
 * @brief Defines the physical state of the simulated spacecraft.
 *
 * Provides the state representation used throughout the simulation.
 * A spacecraft state consists of its position, velocity, and mass.
 */

#ifndef STATE_H
#define STATE_H

#include "Vector2D.h"

/**
 * @brief Represents the physical state of the spacecraft.
 *
 * The state contains the quantities required to describe the
 * spacecraft at a particular instant during the simulation.
 *
 * Position and velocity are represented as two-dimensional vectors.
 * Mass is represented separately because it is a scalar quantity.
 */
struct State
{
    /**
     * @brief Spacecraft position relative to the central body.
     *
     * Units: meters (m).
     */
    Vector2D position;

    /**
     * @brief Spacecraft velocity relative to the central body.
     *
     * Units: meters per second (m/s).
     */
    Vector2D velocity;

    /**
     * @brief Current spacecraft mass.
     *
     * Units: kilograms (kg).
     */
    double mass;
};

#endif