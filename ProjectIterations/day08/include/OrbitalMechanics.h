#ifndef ORBITALMECHANICS_H
#define ORBITALMECHANICS_H

#include "State.h"

double calculate_specific_orbital_energy(
    State state,
    double mu
);

double calculate_specific_angular_momentum(
    State state
);

double calculate_eccentricity(
    State state,
    double mu
);

#endif