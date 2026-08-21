#ifndef ORBITALMECHANICS_H
#define ORBITALMECHANICS_H

#include "State.h"

double calculate_specific_orbital_energy(State spacecraft, double mu);

double calculate_specific_angular_momentum(State spacecraft);

#endif