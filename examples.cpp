#include "examples.h"
#include "math/random.h"
#include "potentials/lennardjones.h"
#include "potentials/lennardjonescelllist.h"
#include "potentials/neuralnetwork.h"
#include "potentials/manyneighbournn.h"
#include "potentials/tensorflownetwork.h"
#include "integrators/eulercromer.h"
#include "integrators/velocityverlet.h"
#include "thermostats/berendsen.h"
#include "thermostats/andersen.h"
#include "porosities/centeredcylinder.h"
#include "porosities/spheres.h"
#include "system.h"
#include "statisticssampler.h"
#include "atom.h"
#include "io.h"
#include "unitconverter.h"
#include "celllist.h"
#include <iostream>
#include <time.h>
#include <armadillo>
#include <fstream>
#include <string>

using std::cout;
using std::endl;

int Examples::lennardJonesFCC() {

    int numberOfUnitCells = 12;
    double initialTemperature = 1.0;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.26);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new LennardJones(system, 3.405, 1.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJFCC.xyz");

    return system->runSimulation();
}


int Examples::lennardJonesFCCCellList() {

    int numberOfUnitCells = 10;
    double initialTemperature = 2.5;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.26);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new LennardJonesCellList(system, 1.0, 1.0, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}


int Examples::lennardJonesFCCNeuralNetwork() {

    int numberOfUnitCells = 12;
    double initialTemperature = 1.0;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.26);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new NeuralNetwork(system, "../TensorFlow/TrainingData/28.11-17.40.35/graph.dat", 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();

}


int Examples::lennardJonesFCCManyNeighbourNeuralNetwork() {

    int numberOfUnitCells = 12;
    double initialTemperature = 1.0;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.72);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new ManyNeighbourNN(system, "../TensorFlow/TrainingData/13.12-12.59.51/graph.dat", 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    cout << UnitConverter::currentUnits << endl;
    //return system->runSimulation();

}

int Examples::lennardJonesFCCTensorFlow() {


    int numberOfUnitCells = 12;
    double initialTemperature = 1.0;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.26);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen
    string graphFileName ("../TensorFlow/TrainingData/18.11-16.12.57/frozen_graph.pb");

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    //system->setPotential(new TensorFlowNetwork(system, graphFileName, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}


int Examples::lennardJonesLiquid() {

    int numberOfUnitCells = 12;
    double initialTemperature = 0.851;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.72);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new LennardJonesCellList(system, 1.0, 1.0, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}

int Examples::loadFromFile() {

    int numberOfUnitCells = 12;
    double initialTemperature = 0.851;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.72);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = false;

    System *system = new System();
    system->readFromStateFile("thermalizedFluidNc20T15Nt1001.xyz", mass, latticeConstant,
                              numberOfUnitCells);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new LennardJonesCellList(system, 1.0, 1.0, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}


int Examples::lennardJonesFCCNanoCylinder() {

    int numberOfUnitCells = 12;
    double initialTemperature = 0.851;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.72);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = true;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new CenteredCylinder(system, usePores));

    system->setPotential(new LennardJonesCellList(system, 1.0, 1.0, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}


int Examples::lennardJonesFCCNanoSpheres() {

    int numberOfUnitCells = 12;
    double initialTemperature = 0.851;  // measured in Kelvin
    double latticeConstant    = UnitConverter::lengthFromAngstroms(5.72);
    double mass = UnitConverter::massFromSI(6.63352088e-26); // mass of Argon atom
    double dt = 0.01;  // Measured in seconds

    bool BoltzmannDist = true;             // initial velocities given by Boltzmann distribution
    double maxMinVelocity = 1.5;           // uniformly distributed velocities [-v, v]
    double tau = 10*dt;                    // relaxation time for Berendsen and Andersen

    bool usePores = true;

    System *system = new System();
    system->createFCCLattice(numberOfUnitCells, latticeConstant, initialTemperature,
                             mass, BoltzmannDist, maxMinVelocity);
    system->setPores(new Spheres(system, usePores));

    system->setPotential(new LennardJonesCellList(system, 1.0, 1.0, 2.5, 3.0));
    system->setTimeStep(dt);
    system->setIntegrator(new VelocityVerlet(system));
    system->setThermostat(new Berendsen(system, initialTemperature, tau));
    system->setUseThermoStat(false);
    system->setThermalization(0);

    system->setNumberOfTimeSteps(501);
    system->setTemperature(initialTemperature);
    system->removeTotalMomentum();

    system->setUseExternalForce(false);
    system->setWriteSample(false);
    system->setRadialDistribution(false);
    system->setMakeXYZ(false);
    system->setXYZName("LJNeighbourFCC.xyz");

    return system->runSimulation();
}


int Examples::computeTemperatureFluctuations() {

    return true;
}


int Examples::computeRadialDistributionFunction() {

    return true;
}


int Examples::compareNeuralNetworkError() {

    System *system = new System();

    // check if error of NN has same shape as in python
    int numberOfPoints = 2000;
    std::ofstream outFile;
    outFile.open("../TensorFlow/Tests/TrainLennardJones/energyAndDerivativeC.dat");
    arma::vec distances = arma::linspace<arma::vec>(3.2, 8.5125, numberOfPoints);
    NeuralNetwork *networkPotential = new NeuralNetwork(system, "../TensorFlow/TrainingData/LJValidation/graph.dat", 2.5, 3.0);
    for (int i=0; i < numberOfPoints; i++) {
        double energy = networkPotential->network(distances(i));
        double derivative = networkPotential->backPropagation();
        outFile << energy << " " << derivative << endl;
    }
    outFile.close();
    return true;
}


int Examples::compareManyNeighbourNeuralNetworkError() {

    System *system = new System();

    // check if error of NN has same shape as in python
    int numberOfNeighbours = 20;
    //std::ofstream outFile;
    //outFile.open("../TensorFlow/Tests/TrainLennardJones/ManyBodyNetwork/error20NeighboursLJC.dat");
    arma::vec distances = arma::linspace<arma::vec>(0.8, 2.5, numberOfNeighbours);
    arma::mat inputVector(1,numberOfNeighbours);
    for (int i=0; i < numberOfNeighbours; i++) {
        inputVector(0,i) = distances(i);
    }
    ManyNeighbourNN *networkPotential = new ManyNeighbourNN(system,
                                                            "../TensorFlow/TrainingData/08.12-15.40.29/graph.dat",
                                                            2.5, 3.0);

    arma::mat energy = networkPotential->network(inputVector);
    cout << energy << endl;

    //outFile.close();
    return true;
}



int Examples::testBackpropagation() {

    System *system = new System();
    //NeuralNetwork *networkPotential = new NeuralNetwork(system, "../TensorFlow/TrainingData/LJValidation/graph.dat", 2.5, 3.0);
    ManyNeighbourNN *networkPotential = new ManyNeighbourNN(system, "../TensorFlow/TrainingData/22.03-15.06.05/graph.dat", 2.5, 3.0);
    std::ifstream infile;
    infile.open("../TensorFlow/testForcesInput.txt");
    arma::mat inputVector(1,48);
    int i=0;
    for ( std::string line; std::getline(infile, line); ) {
        infile >> inputVector(0,i);
        i++;
    }
    cout << inputVector << endl;
    arma::mat energy = networkPotential->network(inputVector);
    cout << energy << endl;
    arma::mat derivative = networkPotential->backPropagation();
    cout << derivative << endl;

    return true;
}
