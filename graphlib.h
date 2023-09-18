#ifndef _GRAPH_h_
#define _GRAPH_h_

#include <cmath>
#include <iostream>

#include "LinkedList/list.h"
#include "objects.h"

const double       DEFAULT_SIZE   = 50;
const int          FRAME_WIDTH    = 10;
const unsigned int DEFAULT_WEIGHT = 1;
const sf::Color    CIRCLE_COL     = sf::Color::Blue;
const sf::Color    SQUARE_COL     = sf::Color::Red;

class Manager {
private:
    List_t* molecules = nullptr;

public:
    explicit Manager();
    explicit Manager(List_t* molecules);

    ~Manager();

    void drawAll       (sf::RenderTexture& texture);
    void moveAll       (sf::RenderTexture& texture);
    void checkCollision(sf::RenderTexture& texture, long ind1, long ind2);
    void addMolecule   (sf::RenderTexture& texture);
};

List_t* createEmptyList();

bool collideCircles     (BaseMolecule* mol1, BaseMolecule* mol2);
bool collideSquareCircle(BaseMolecule* square, BaseMolecule* circle);
bool collideSquares     (BaseMolecule* mol1, BaseMolecule* mol2);

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif