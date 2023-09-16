#ifndef _GRAPH_h_
#define _GRAPH_h_

#include <SFML/Graphics.hpp>
#include <cmath>

const double DEFAULT_SIZE  = 5;
const sf::Color CIRCLE_COL = sf::Color::Blue;
const sf::Color SQUARE_COL = sf::Color::Red;

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

    virtual void draw(sf::RenderTexture& texture) = 0;
};

class CircleMolecule : BaseMolecule {
public:
    explicit CircleMolecule();
    explicit CircleMolecule(double x, double y, unsigned int weight);

    ~CircleMolecule();

    void draw(sf::RenderTexture& texture) override;
};

class SquareMolecule : BaseMolecule {
public:
    explicit SquareMolecule();
    explicit SquareMolecule(double x, double y, unsigned int weight);

    ~SquareMolecule();

    void draw(sf::RenderTexture& texture) override;
};

class Manager {
private:
    BaseMolecule* molecules     = nullptr;
    size_t        moleculeCount = 0;

public:
    explicit Manager();
    explicit Manager(BaseMolecule* molecules, size_t moleculeCount);

    void drawAll(sf::RenderWindow& window);
    void checkCollision(BaseMolecule& molecule1, BaseMolecule& molecule2);
    void addMolecule(BaseMolecule& molecule);
    void removeMolecule(BaseMolecule& molecule);
};

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif