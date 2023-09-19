#include "manager.h"

List_t* createEmptyList() {
    List_t* list = new List_t;
    _listCtor(list, 2, true);

    return list;
}

double generateRandDouble(double left, double right) {
    return ((rand() % 10) / (double)10) * (right - left) + left;
}

Manager::Manager() :
    molecules(createEmptyList()),
    pressY   (FRAME_WIDTH)    
    {}

Manager::Manager(List_t* molecules, double pressY) :
    molecules(molecules),
    pressY   (pressY + FRAME_WIDTH)
    {}

Manager::~Manager() {
    ON_ERROR(!this, "Object pointer was null!",);

    if (!(this->molecules)) return;

    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* value = this->molecules->values[i].value;
        if (value) delete value;
    }

    listDtor(this->molecules);
    delete this->molecules;
    this->molecules = nullptr;
}

void Manager::drawAll(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    // draw frame
    sf::RectangleShape frame = sf::RectangleShape(sf::Vector2f(texture.getSize().x - 2 * FRAME_WIDTH, texture.getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    texture.draw(frame);

    // draw press
    sf::RectangleShape press = sf::RectangleShape(sf::Vector2f(texture.getSize().x - 2 * FRAME_WIDTH, FRAME_WIDTH));
    press.setFillColor   (sf::Color::Transparent);
    press.setOutlineColor(sf::Color::Red);
    press.setOutlineThickness(FRAME_WIDTH);
    press.setPosition(FRAME_WIDTH, this->pressY);
    texture.draw(press);

    long moleculeCount = this->molecules->size;

    for (long i = 0; i <= moleculeCount; i++) {
        for (long j = i + 1; j <= moleculeCount; j++) {
            checkCollision(texture, i, j);
        }
    }

    // draw molecules
    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* molecule = this->molecules->values[i].value;
        if (molecule) molecule->draw(texture);
    }
}

void Manager::moveAll(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* molecule = this->molecules->values[i].value;
        if (molecule) molecule->move(texture, this->pressY, this->temperature);
    }
}

bool collideCircles(BaseMolecule* mol1, BaseMolecule* mol2) {
    ON_ERROR(!mol1 || !mol2, "Object pointer was null!", false);

    double centre1x = mol1->getX() + mol1->getSize();
    double centre1y = mol1->getY() + mol1->getSize();
    double centre2x = mol2->getX() + mol2->getSize();
    double centre2y = mol2->getY() + mol2->getSize();

    double radSum = mol1->getSize() + mol2->getSize();

    return (centre1x - centre2x) * (centre1x - centre2x) 
         + (centre1y - centre2y) * (centre1y - centre2y) <= radSum * radSum;
}

bool collideSquareCircle(BaseMolecule* square, BaseMolecule* circle) {
    ON_ERROR(!square || !circle, "Object pointer was null!", false);

    double rectCentreX = square->getX() + square->getSize() / 2;
    double rectCentreY = square->getY() + square->getSize() / 2;
    double circCentreX = circle->getX() + circle->getSize();
    double circCentreY = circle->getY() + circle->getSize();

    double distanceX = abs(rectCentreX - circCentreX);
    double distanceY = abs(rectCentreY - circCentreY);

    if ((distanceX > square->getSize() / 2 + circle->getSize()) ||
        (distanceY > square->getSize() / 2 + circle->getSize())) return false;

    if ((distanceX <= square->getSize() / 2) ||
        (distanceY <= square->getSize() / 2)) return true;

    double distToCorner = 
        2 * (circCentreX - square->getSize() / 2) * (circCentreX - square->getSize() / 2); 

    return distToCorner <= circle->getSize() * circle->getSize();
        
}

bool collideSquares(BaseMolecule* mol1, BaseMolecule* mol2) {
    ON_ERROR(!mol1 || !mol2, "Object pointer was null!", false);

    double mol1CentreX = mol1->getX() + mol1->getSize();
    double mol1CentreY = mol1->getY() + mol1->getSize();
    double mol2CentreX = mol2->getX() + mol2->getSize();
    double mol2CentreY = mol2->getY() + mol2->getSize();

    return (abs(mol1CentreX - mol2CentreX) < mol1->getSize() / 2 + mol2->getSize() / 2) &&
           (abs(mol1CentreY - mol2CentreY) < mol1->getSize() / 2 + mol2->getSize() / 2);
}

void Manager::checkCollision(sf::RenderTexture& texture, long ind1, long ind2) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    List_t* listPointer = this->molecules;
    ON_ERROR(ind1 > listPointer->size || ind2 > listPointer->size, "Overflow attempt",);

    BaseMolecule* molecule1 = listPointer->values[ind1].value;
    BaseMolecule* molecule2 = listPointer->values[ind2].value;

    if (!molecule1 || !molecule2) return;

    if (molecule1->getType() == CIRCLE && molecule2->getType() == CIRCLE) {
        if (collideCircles(molecule1, molecule2)) {
            double createX = (molecule1->getX() + molecule2->getX()) / 2;
            double createY = (molecule1->getY() + molecule2->getY()) / 2;

            listPushBack(listPointer, new SquareMolecule(createX, createY, molecule1->getWeight() + molecule2->getWeight()));
            listPointer->values[ind1].value = nullptr;
            listPointer->values[ind2].value = nullptr;
        }
        return;
    }
    if (molecule1->getType() == CIRCLE && molecule2->getType() == SQUARE) {
        if (collideSquareCircle(molecule2, molecule1)) {
                listPointer->values[ind1].value = nullptr;
                listPointer->values[ind2].value->addWeight(molecule1->getWeight());
            }
        return;
    }
    if (molecule1->getType() == SQUARE && molecule2->getType() == CIRCLE) {
        if (collideSquareCircle(molecule1, molecule2)) {
                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value->addWeight(molecule2->getWeight());
            }
        return;
    }
    if (molecule1->getType() == SQUARE && molecule2->getType() == SQUARE) {
        if (collideSquares(molecule1, molecule2)) {
                unsigned int circleAmount = molecule1->getWeight() + molecule2->getWeight();
                double createCrlcPointX = (molecule1->getX() + molecule2->getX()) / 2;
                double createCrlcPointY = (molecule1->getY() + molecule2->getY()) / 2;
                double diameter = molecule1->getSize() * 2;

                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value = nullptr;

                double angle = 2 * M_PI / circleAmount;
                for (unsigned int i = 0; i < circleAmount; i++) {
                    double speedY = cos(angle * i);
                    double speedX = sin(angle * i);

                    double circleX = createCrlcPointX + diameter * speedX;
                    double circleY = createCrlcPointY + diameter * speedY;

                    addMolecule(texture, circleX, circleY, speedX, speedY);
                }
            }
        return;
    }
}

void Manager::addMolecule(sf::RenderTexture& texture, double x, double y, double velX, double velY) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    CircleMolecule* molecule = new CircleMolecule(x, y, DEFAULT_WEIGHT);
    molecule->setSpeed(velX, velY);

    listPushBack(this->molecules, molecule);
}