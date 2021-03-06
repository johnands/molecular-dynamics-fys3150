#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "potential.h"
#include <vector>
#include <armadillo>
#include <fstream>

class CellList;

class NeuralNetwork : public Potential {

public:
    NeuralNetwork(System *system, const char *filename,
                  double rCut, double neighbourCut);
    void readFromFile();
    double network(double dataPoint);
    virtual void calculateForces();
    double backPropagation();

protected:
    const char *m_filename;
    int m_nLayers;
    int m_nNodes;
    std::vector<arma::mat> m_weights            = std::vector<arma::mat>();
    std::vector<arma::mat> m_weightsTransposed  = std::vector<arma::mat>();
    std::vector<arma::mat> m_biases             = std::vector<arma::mat>();
    std::vector<arma::mat> m_preActivations     = std::vector<arma::mat>();
    std::vector<arma::mat> m_activations        = std::vector<arma::mat>();
    std::vector<arma::mat> m_derivatives        = std::vector<arma::mat>();
    std::ofstream m_outFile;
    CellList *m_cellList = nullptr;
    int    m_updateLists = 0;
    double m_rCut = 0.0;
    double m_rCutSquared = 0.0;
    double m_neighbourCut = 0.0;
    double m_potentialCut = 0.0;
    int m_numberOfInputs = 0.0;
    int m_numberOfOutputs = 0.0;
};

#endif // NEURALNETWORK_H
