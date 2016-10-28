#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "potential.h"
#include <vector>
#include <armadillo>
#include <fstream>

class CellList;

class NeuralNetwork : public Potential {

public:
    NeuralNetwork(System *system, const char *filename, double cutOffDistance);
    void readFromFile();
    double network(double dataPoint);
    void calculateForces();
    arma::mat relu(arma::mat matrix);


private:
    const char *m_filename;
    int m_nLayers;
    int m_nNodes;
    std::vector<arma::mat> m_weights = std::vector<arma::mat>();
    std::vector<arma::mat> m_biases  = std::vector<arma::mat>();
    std::ofstream m_outFile;
    CellList *m_cellList = nullptr;
    int    m_updateLists = 0;
    double m_cutOffDistance = 0.0;
};

#endif // NEURALNETWORK_H