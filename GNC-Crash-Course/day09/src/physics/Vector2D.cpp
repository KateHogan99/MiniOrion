/**
 * @file Vector2D.cpp
 * @brief Implements two-dimensional vector operations.
 *
 * Provides the vector mathematics declared in Vector2D.h.
 */

#include "Vector2D.h"

#include <cmath>

Vector2D add_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
)
{
    return {
        vector_a.x + vector_b.x,
        vector_a.y + vector_b.y
    };
}

Vector2D subtract_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
)
{
    return {
        vector_a.x - vector_b.x,
        vector_a.y - vector_b.y
    };
}

Vector2D scale_vector(
    Vector2D vector, 
    double scalar
)
{
    return {
        vector.x * scalar,
        vector.y * scalar
    };
}

double calculate_magnitude(
    Vector2D vector
)
{
    return std::sqrt(
        vector.x * vector.x +
        vector.y * vector.y
    );
}