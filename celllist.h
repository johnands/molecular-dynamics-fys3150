#ifndef CELLLIST_H
#define CELLLIST_H
#include "math/vec3.h"
#include "config.h"
#include <functional>
#include <vector>
using std::function;
using std::vector;

class Atom; class System;

class Cell
{
private:

public:
    Cell();
    unsigned int numberOfAtoms;
    float x[MAXNUMATOMS];
    float fx[MAXNUMATOMS];
    float y[MAXNUMATOMS];
    float fy[MAXNUMATOMS];
    float z[MAXNUMATOMS];
    float fz[MAXNUMATOMS];

    int   cellIndex[MAXNUMATOMS];

    float vx[MAXNUMATOMS];
    float vy[MAXNUMATOMS];
    float vz[MAXNUMATOMS];

    float mass[MAXNUMATOMS];
    int   index[MAXNUMATOMS];

    void addAtom(vec3 position, vec3 velocity, float mass, unsigned int atomIndex);
    void removeAtom(unsigned int i);
    void addAtom(float x, float y, float z, float vx, float vy, float vz, float mass, unsigned int atomIndex);
    void addAtomFromCell(Cell &cell, unsigned int i);
};

class CellList
{
private:
    System *m_system;
    int m_numberOfCellsX;
    int m_numberOfCellsY;
    int m_numberOfCellsZ;
    float m_oneOverLengthX;
    float m_oneOverLengthY;
    float m_oneOverLengthZ;
    vector<Cell> m_cells;

public:
    CellList();
    void setup(System *system, float rCut);
    float lengthX() { return 1.0/m_oneOverLengthX; }
    float lengthY() { return 1.0/m_oneOverLengthY; }
    float lengthZ() { return 1.0/m_oneOverLengthZ; }

    int numberOfCellsX() { return m_numberOfCellsX; }
    int numberOfCellsY() { return m_numberOfCellsY; }
    int numberOfCellsZ() { return m_numberOfCellsZ; }

    void index3d(unsigned int &index, unsigned int &i, unsigned int &j, unsigned int &k) {
        k = (index / (m_numberOfCellsX*m_numberOfCellsY));
        j = (index / m_numberOfCellsX) % m_numberOfCellsY;
        i = index % m_numberOfCellsX;
    }

    unsigned int indexf(float x, float y, float z);
    unsigned int index(unsigned int cx, unsigned int cy, unsigned int cz) { return cx + cy*m_numberOfCellsX + cz*m_numberOfCellsY*m_numberOfCellsX; }
    unsigned int indexPeriodic(int cx, int cy, int cz) { return ( (cx+m_numberOfCellsX) % m_numberOfCellsX) + ( (cy+m_numberOfCellsY) % m_numberOfCellsY)*m_numberOfCellsX + ( (cz+m_numberOfCellsZ) % m_numberOfCellsZ)*m_numberOfCellsX*m_numberOfCellsY; }
    Cell &operator[](int index) { return m_cells[index]; }
    vector<Cell> &cells();
    void addAtom(vec3 position, vec3 velocity, float mass, unsigned int atomIndex);
    void forEachAtom(std::function<void(Cell &, unsigned int)> action);
    void update();
};

#endif // CELLLIST_H
