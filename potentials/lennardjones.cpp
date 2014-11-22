#include "potentials/lennardjones.h"
#include "celllist.h"
#include "neighborlist.h"
#include "cpelapsedtimer.h"
#include <cmath>
#include <iostream>
using namespace std;
LennardJones::LennardJones(float sigma, float epsilon, float cutoffRadius) :
    m_sigma(sigma),
    m_sigma6(pow(sigma, 6.0)),
    m_epsilon(epsilon),
    m_24epsilon(24*epsilon),
    m_rCutSquared(cutoffRadius*cutoffRadius),
    m_timeSinceLastNeighborListUpdate(0),
    m_potentialEnergyAtRcut(0)
{
    float oneOverDrCut2 = 1.0/m_rCutSquared;
    float oneOverDrCut6 = oneOverDrCut2*oneOverDrCut2*oneOverDrCut2;

    m_potentialEnergyAtRcut = 4*m_epsilon*m_sigma6*oneOverDrCut6*(m_sigma6*oneOverDrCut6 - 1);
}

void LennardJones::calculateForces(System *system)
{
    m_potentialEnergy = 0;
    m_pressureVirial = 0;
    vec3 systemSize = system->systemSize();
    vec3 systemSizeHalf = system->systemSize()*0.5;

    if(!m_timeSinceLastNeighborListUpdate || m_timeSinceLastNeighborListUpdate++ > 20) {
        system->neighborList().update();
        m_timeSinceLastNeighborListUpdate = 1;
    }

    CPElapsedTimer::calculateForces().start();
    const unsigned int numAtoms = system->atoms().numberOfAtoms;
    Atoms &atoms = system->atoms();

    for(unsigned int i=0; i<numAtoms; i++) {
        float x = atoms.x[i];
        float y = atoms.y[i];
        float z = atoms.z[i];

        const vector<unsigned int> &neighbors = system->neighborList().neighborsForAtomWithIndex(i);

        const unsigned int numNeighbors = neighbors.size();
        for(unsigned int j=0; j<numNeighbors; j++) {
            int neighborIndex = neighbors[j];
            float dx = x - atoms.x[neighborIndex];
            float dy = y - atoms.y[neighborIndex];
            float dz = z - atoms.z[neighborIndex];

            dx += systemSize[0]*( (dx < -systemSizeHalf[0] ) - (dx > systemSizeHalf[0]));
            dy += systemSize[1]*( (dy < -systemSizeHalf[1] ) - (dy > systemSizeHalf[1]));
            dz += systemSize[2]*( (dz < -systemSizeHalf[2] ) - (dz > systemSizeHalf[2]));

            const float dr2 = dx*dx + dy*dy + dz*dz;
            const float oneOverDr2 = 1.0f/dr2;
            const float oneOverDr6 = oneOverDr2*oneOverDr2*oneOverDr2;

            const float force = -m_24epsilon*m_sigma6*oneOverDr6*(2*m_sigma6*oneOverDr6 - 1)*oneOverDr2*(dr2 < m_rCutSquared);

            atoms.fx[i] -= dx*force;
            atoms.fy[i] -= dy*force;
            atoms.fz[i] -= dz*force;

            atoms.fx[neighborIndex] += dx*force;
            atoms.fy[neighborIndex] += dy*force;
            atoms.fz[neighborIndex] += dz*force;

            if(m_shouldComputeEnergyAndPressureVirial) {
                m_pressureVirial += force*sqrt(dr2)*dr2;
                m_potentialEnergy += (4*m_epsilon*m_sigma6*oneOverDr6*(m_sigma6*oneOverDr6 - 1) - m_potentialEnergyAtRcut)*(dr2 < m_rCutSquared);
            }
        }
    }

    CPElapsedTimer::calculateForces().stop();
}
