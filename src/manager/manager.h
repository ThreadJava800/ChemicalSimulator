#ifndef _GRAPH_h_
#define _GRAPH_h_

#include "../constants.h"
#include "../objects/objects.h"
#include "../../LinkedList/list.h"
#include "../buttons/button.h"

class Manager {
private:
    List_t* molecules   = nullptr;
    double  pressY      = 0;
    double  temperature = 273.15;   // kelvins (0 degrees celsium)

    Button** buttons    = nullptr;
    unsigned int btnCnt = 0;

    void drawMolecules(sf::RenderTexture& moleculeTexture);
    void drawButtons  (sf::RenderTexture& buttonTexture);
 
public:
    explicit Manager();
    explicit Manager(List_t* molecules, double pressY, Button** buttons, unsigned int btnCnt);

    void setButtons(Button** buttons, unsigned int btnCnt);

    ~Manager();

    void drawAll       (sf::RenderTexture& moleculeTexture, sf::RenderTexture& buttonTexture);
    void registerClick (sf::RenderTexture& moleculeTexture, sf::RenderTexture& buttonTexture, sf::Vector2f spriteStart);
    void moveAllObjects(sf::RenderTexture& texture);
    void checkCollision(sf::RenderTexture& texture, long ind1, long ind2);
    void addCircle     (double x, double y, double velX = 1, double velY = 1);
    void addSquare     (double x, double y, double velX = 1, double velY = 1);
    void pressUp       (double shift);
    void pressDown     (double shift);
    void tempUp        (double shift);
    void tempDown      (double shift);
};

void addCircle(Manager& manager, sf::RenderTexture& moleculeTexture);
void addSquare(Manager& manager, sf::RenderTexture& moleculeTexture);
void pressUp  (Manager& manager, sf::RenderTexture& moleculeTexture);
void pressDown(Manager& manager, sf::RenderTexture& moleculeTexture);
void tempUp   (Manager& manager, sf::RenderTexture& moleculeTexture);
void tempDown (Manager& manager, sf::RenderTexture& moleculeTexture);

List_t* createEmptyList();

double generateRandDouble(double left, double right);

bool collideCircles     (BaseMolecule* mol1,   BaseMolecule* mol2);
bool collideSquareCircle(BaseMolecule* square, BaseMolecule* circle);
bool collideSquares     (BaseMolecule* mol1,   BaseMolecule* mol2);

#endif