#include "atom.h"
#include "math/random.h"
#include <cmath>
#include <iostream>

Atom::Atom(double mass) :
    m_mass(mass)
{
    
}

void Atom::resetForce()
{
    force.setToZero();
}

void Atom::resetVelocityMaxwellian(double temperature)
{
    // Resetting the velocity according to a Maxwell-Boltzmann distribution (see http://en.wikipedia.org/wiki/Maxwell%E2%80%93Boltzmann_distribution )
    double boltzmannConstant = 1.0; // In these units, the boltzmann constant equals 1
    double standardDeviation = sqrt(boltzmannConstant*temperature/m_mass);
    velocity.randomGaussian(0, standardDeviation);
}

void Atom::resetVelocityUniform(double maxMinVelocity)
{
    velocity.randomUniform(maxMinVelocity);
}

void Atom::setCellIndicies(int i, int j, int k) {
    m_cellIndicies[0] = i;
    m_cellIndicies[1] = j;
    m_cellIndicies[2] = k;
}

void Atom::storeInitialPosition() {
    m_initialPosition = position;
}

void Atom::setIndex(int index) {
    m_index = index;
}

void Atom::adjustIndex(int change) {
    m_index += change;
}

void Atom::setMovingAtom(bool moving) {
    m_movingAtom = moving;
}

void Atom::setName(const char *name) {
    m_name = name;
}
