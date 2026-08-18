#ifndef VECTOR2D_H
#define VECTOR2D_H

struct Vector2D
{
    double x;
    double y;
};

Vector2D add_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
);

Vector2D subtract_vectors(
    Vector2D vector_a, 
    Vector2D vector_b
);

Vector2D scale_vector(
    Vector2D vector, 
    double scalar
);

double calculate_magnitude(
    Vector2D vector
);

#endif