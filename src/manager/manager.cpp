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
    pressY   (FRAME_WIDTH),
    buttons  (nullptr),
    btnCnt   (0) 
    {}

Manager::Manager(List_t* molecules, double pressY, Button** buttons, unsigned int btnCnt) :
    molecules(molecules),
    pressY   (pressY + FRAME_WIDTH),
    buttons  (buttons),
    btnCnt   (btnCnt)
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

void Manager::setButtons(Button** buttons, unsigned int btnCnt) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->buttons = buttons;
    this->btnCnt  = btnCnt;
}

void Manager::drawMolecules(sf::RenderTexture& moleculeTexture) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    // draw frame
    sf::RectangleShape frame = sf::RectangleShape(sf::Vector2f(moleculeTexture.getSize().x - 2 * FRAME_WIDTH, moleculeTexture.getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    moleculeTexture.draw(frame);

    // draw press
    sf::RectangleShape press = sf::RectangleShape(sf::Vector2f(moleculeTexture.getSize().x - 2 * FRAME_WIDTH, FRAME_WIDTH));
    press.setFillColor   (sf::Color::Transparent);
    press.setOutlineColor(sf::Color::Red);
    press.setOutlineThickness(FRAME_WIDTH);
    press.setPosition(FRAME_WIDTH, this->pressY);
    moleculeTexture.draw(press);

    long moleculeCount = this->molecules->size;

    for (long i = 0; i <= moleculeCount; i++) {
        for (long j = i + 1; j <= moleculeCount; j++) {
            checkCollision(moleculeTexture, i, j);
        }
    }

    // draw molecules
    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* molecule = this->molecules->values[i].value;
        if (molecule) molecule->draw(moleculeTexture);
    }
}

void Manager::drawButtons(sf::RenderTexture& buttonTexture) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->buttons, "Button array is null!",);

    for (unsigned int i = 0; i < this->btnCnt; i++) {
        Button* btn = this->buttons[i];
        if (btn) btn->draw(buttonTexture);
    }

    //draw frame
    sf::RectangleShape frame = sf::RectangleShape(sf::Vector2f(buttonTexture.getSize().x - 2 * FRAME_WIDTH, buttonTexture.getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::Red);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    buttonTexture.draw(frame);
}

void Manager::drawAll(sf::RenderTexture& moleculeTexture, sf::RenderTexture& buttonTexture) {
    ON_ERROR(!this, "Object pointer was null!",);

    drawMolecules(moleculeTexture);
    drawButtons  (buttonTexture);
}

void Manager::registerClick (sf::RenderTexture& moleculeTexture, sf::RenderTexture& buttonTexture, sf::Vector2f spriteStart) {
    ON_ERROR(!this, "Object pointer was null!",);

    for (unsigned int i = 0; i < this->btnCnt; i++) {
        Button* btn = this->buttons[i];
        if (btn) {
            sf::Vector2i mousePositionTexture = 
                sf::Mouse::getPosition() - sf::Vector2i(spriteStart) - sf::Vector2i(0, 2 * FRAME_WIDTH);

            if (btn->isInside(mousePositionTexture)) {
                (btn->getFunc())(*this, moleculeTexture);
            }
        }
    }
}

void Manager::moveAllObjects(sf::RenderTexture& texture) {
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

            delete molecule1;
            delete molecule2;
        }
        return;
    }
    if (molecule1->getType() == CIRCLE && molecule2->getType() == SQUARE) {
        if (collideSquareCircle(molecule2, molecule1)) {
                listPointer->values[ind1].value = nullptr;
                listPointer->values[ind2].value->addWeight(molecule1->getWeight());

                delete molecule1;
            }
        return;
    }
    if (molecule1->getType() == SQUARE && molecule2->getType() == CIRCLE) {
        if (collideSquareCircle(molecule1, molecule2)) {
                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value->addWeight(molecule2->getWeight());

                delete molecule2;
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

                delete molecule1;
                delete molecule2;

                double angle = 2 * M_PI / circleAmount;
                for (unsigned int i = 0; i < circleAmount; i++) {
                    double speedY = cos(angle * i);
                    double speedX = sin(angle * i);

                    double circleX = createCrlcPointX + diameter * speedX;
                    double circleY = createCrlcPointY + diameter * speedY;

                    addCircle(circleX, circleY, speedX, speedY);
                }
            }
        return;
    }
}

void Manager::addCircle(double x, double y, double velX, double velY) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    CircleMolecule* molecule = new CircleMolecule(x, y, DEFAULT_WEIGHT);
    molecule->setSpeed(velX, velY);

    listPushBack(this->molecules, molecule);
}

void Manager::addSquare(double x, double y, double velX, double velY) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    SquareMolecule* molecule = new SquareMolecule(x, y, DEFAULT_WEIGHT);
    molecule->setSpeed(velX, velY);

    listPushBack(this->molecules, molecule);
}

void Manager::pressUp(double shift) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->pressY -= shift;
}

void Manager::pressDown(double shift) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->pressY += shift;
}

void Manager::tempUp(double shift) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->temperature += shift;
}

void Manager::tempDown(double shift) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->temperature -= shift;
}

void addCircle(Manager& manager, sf::RenderTexture& moleculeTexture) {
    manager.addCircle(rand() % (moleculeTexture.getSize().x - int(DEFAULT_SIZE * 2)), 
                      rand() % (moleculeTexture.getSize().y - int(DEFAULT_SIZE * 2)));
}

void addSquare(Manager& manager, sf::RenderTexture& moleculeTexture) {
    manager.addSquare(rand() % (moleculeTexture.getSize().x - int(DEFAULT_SIZE * 2)), 
                      rand() % (moleculeTexture.getSize().y - int(DEFAULT_SIZE * 2)));
}

void pressUp(Manager& manager) {
    manager.pressUp(PRESS_SHIFT);
}

void pressDown(Manager& manager) {
    manager.pressDown(PRESS_SHIFT);
}

void tempUp(Manager& manager) {
    manager.tempUp(TEMP_SHIFT);
}

void tempDown(Manager& manager) {
    manager.tempDown(TEMP_SHIFT);
}