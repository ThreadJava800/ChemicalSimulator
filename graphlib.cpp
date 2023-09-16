#include "graphlib.h"

BaseMolecule::BaseMolecule(double x, double y, double size, unsigned int weight, MoleculeType type) :
    x     (x),
    y     (y),
    size  (size),
    weight(weight),
    type  (type)
    {}

CircleMolecule::CircleMolecule() : BaseMolecule(0, 0, 0, 0, CIRCLE) {}
CircleMolecule::CircleMolecule(double x, double y, unsigned int weight) :
                BaseMolecule  (x, y, DEFAULT_SIZE, weight, CIRCLE)  {}

CircleMolecule::~CircleMolecule() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->x      = NAN;
    this->y      = NAN;
    this->size   = NAN;
    this->weight = 0;
    this->type   = ERROR_TYPE;
}

void CircleMolecule::draw() {

}

SquareMolecule::SquareMolecule() : BaseMolecule(0, 0, 0, 0, SQUARE) {}
SquareMolecule::SquareMolecule(double x, double y, unsigned int weight) :
                BaseMolecule  (x, y, DEFAULT_SIZE, weight, SQUARE)  {}

SquareMolecule::~SquareMolecule() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->x      = NAN;
    this->y      = NAN;
    this->size   = NAN;
    this->weight = 0;
    this->type   = ERROR_TYPE;
}

void SquareMolecule::draw() {

}

Manager::Manager() :
    molecules    (nullptr),
    moleculeCount(0)
    {}

Manager::Manager(BaseMolecule* molecules, size_t moleculeCount) :
    molecules    (molecules),
    moleculeCount(moleculeCount)
    {}

void Manager::drawAll() {

}

void Manager::checkCollision() {

}