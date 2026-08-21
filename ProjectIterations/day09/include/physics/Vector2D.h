/**
 * @file Vector2D.h
 * @brief Defines a two-dimensional vector and common vector operations.
 * 
 * Provides the basic vector mathematics used throughout the simulation, 
 * including position, velocity, acceleration, and displacement calculations.
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

/**
 * @brief Represents a two-dimensional vector.
 *
 * Components represent different physical quantities depending on
 * context. Within the simulation, vectors are commonly used for
 * position (m), velocity (m/s), acceleration (m/s^2), displacement (m),
 * and other two-dimensional quantities.
 */
struct Vector2D
{
    double x;
    double y;
};

/**
 * @brief Calculates the sum of two two-dimensional vectors.
 * 
 * @param vector_a The first vector to be added.
 * @param vector_b The second vector to be added.
 * 
 * @return A two-dimensional vector of the sum.
 */
Vector2D add_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
);

/**
 * @brief Calculates the difference of two two-dimensional vectors.
 * 
 * @param vector_a The first vector, from which the second will be subtracted.
 * @param vector_b The second, to be subtracted from the first.
 * 
 * @return A two-dimensional vector of the difference.
 */
Vector2D subtract_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
);

/**
 * @brief Multiplies a two-dimensional vector by a given scalar.
 * 
 * @param vector The vector to be scaled.
 * @param scalar The value to multiply each component of the vector by.
 * 
 * @return A two-dimensional vector of the scaled values.
 */
Vector2D scale_vector(
    Vector2D vector, 
    double scalar
);

/**
 * @brief Calculates the magnitude of a two-dimensional vector.
 * 
 * @param vector Vector whose magnitude will be calculated.
 * 
 * @return Euclidean magnitude of the vector.
 */
double calculate_magnitude(
    Vector2D vector
);

#endif