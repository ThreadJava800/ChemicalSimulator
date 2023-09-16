#ifndef _GRAPH_h_
#define _GRAPH_h_

#include <SFML/Graphics.hpp>
#include <cmath>

const double DEFAULT_SIZE = 5;

enum MoleculeType {
    ERROR_TYPE,
    SQUARE,
    CIRCLE
};

class BaseMolecule {
protected:
    double       x      = 0;
    double       y      = 0;
    double       size   = 0;
    unsigned int weight = 0;
    MoleculeType type   = ERROR_TYPE;

public:
    explicit BaseMolecule(double x, double y, double size, unsigned int weight, MoleculeType type);

    virtual void draw() = 0;
};

class CircleMolecule : BaseMolecule {
public:
    explicit CircleMolecule();
    explicit CircleMolecule(double x, double y, unsigned int weight);

    ~CircleMolecule();

    void draw() override;
};

class SquareMolecule : BaseMolecule {
public:
    explicit SquareMolecule();
    explicit SquareMolecule(double x, double y, unsigned int weight);

    ~SquareMolecule();

    void draw() override;
};

class Manager {
private:
    BaseMolecule* molecules     = nullptr;
    size_t        moleculeCount = 0;

public:
    explicit Manager();
    explicit Manager(BaseMolecule* molecules, size_t moleculeCount);

    void drawAll();
    void checkCollision();
};

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif