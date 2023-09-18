#include "graphlib.h"

BaseMolecule::BaseMolecule(double x, double y, double size, unsigned int weight, MoleculeType type) :
    x        (x),
    y        (y),
    size     (size),
    velocityX(START_VELOCITY),
    velocityY(START_VELOCITY),
    weight   (weight),
    type     (type)
    {}

double BaseMolecule::getX() {
    ON_ERROR(!this, "Object pointer was null!", 0);
    return this->x;
}

double BaseMolecule::getY() {
    ON_ERROR(!this, "Object pointer was null!", 0);
    return this->y;
}

double BaseMolecule::getSize() {
    ON_ERROR(!this, "Object pointer was null!", 0);
    return this->size;
}

MoleculeType BaseMolecule::getType() {
    ON_ERROR(!this, "Object pointer was null!", ERROR_TYPE);
    return this->type;
}

double BaseMolecule::getWeight() {
    ON_ERROR(!this, "Object pointer was null!", 0);
    return this->weight;
}

void BaseMolecule::move(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->wallCollision(texture);

    this->x += velocityX;
    this->y += velocityY;
}

void BaseMolecule::addWeight(unsigned int weightAdd) {
    ON_ERROR(!this, "Object pointer was null!",);
    this->weight += weightAdd;
}

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

void CircleMolecule::draw(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    sf::CircleShape shape(this->size);
    shape.setPosition(this->x, this->y);
    shape.setFillColor(CIRCLE_COL);

    texture.draw(shape);
}

void CircleMolecule::wallCollision(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    double rightWall = texture.getSize().x - FRAME_WIDTH;
    double downWall  = texture.getSize().y - FRAME_WIDTH;

    if (this->x <= FRAME_WIDTH) this->velocityX = fabs(this->velocityX);
    if (this->y <= FRAME_WIDTH) this->velocityY = fabs(this->velocityY);    

    if (this->x + this->size * 2 >= rightWall) this->velocityX = -1 * fabs(this->velocityX);
    if (this->y + this->size * 2 >= downWall)  this->velocityY = -1 * fabs(this->velocityY);
}

SquareMolecule::SquareMolecule() : BaseMolecule(0, 0, 0, 0, SQUARE) {}
SquareMolecule::SquareMolecule(double x, double y, unsigned int weight) :
                BaseMolecule  (x, y, 2 * DEFAULT_SIZE, weight, SQUARE)  {}

SquareMolecule::~SquareMolecule() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->x      = NAN;
    this->y      = NAN;
    this->size   = NAN;
    this->weight = 0;
    this->type   = ERROR_TYPE;
}

void SquareMolecule::draw(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    sf::RectangleShape shape(sf::Vector2f(this->size, this->size));
    shape.setPosition(this->x, this->y);
    shape.setFillColor(SQUARE_COL);

    texture.draw(shape);
}

void SquareMolecule::wallCollision(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    double rightWall = texture.getSize().x - FRAME_WIDTH;
    double downWall  = texture.getSize().y - FRAME_WIDTH;

    if (this->x <= FRAME_WIDTH) this->velocityX = fabs(this->velocityX);
    if (this->y <= FRAME_WIDTH) this->velocityY = fabs(this->velocityY);    

    if (this->x + this->size >= rightWall) this->velocityX = -1 * fabs(this->velocityX);
    if (this->y + this->size >= downWall)  this->velocityY = -1 * fabs(this->velocityY);
}

List_t* createEmptyList() {
    List_t* list = new List_t;
    _listCtor(list, 2, true);

    return list;
}

Manager::Manager() :
    molecules    (createEmptyList())
    {}

Manager::Manager(List_t* molecules) :
    molecules    (molecules)
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
        if (molecule) molecule->move(texture);
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
            listPushBack(listPointer, new SquareMolecule(0, 0, molecule1->getWeight() + molecule2->getWeight()));
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
                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value->addWeight(molecule1->getWeight());
            }
        return;
    }
}

void Manager::addMolecule(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    int centreX = rand() % texture.getSize().x;
    int centreY = rand() % texture.getSize().y;
    CircleMolecule* molecule = new CircleMolecule(centreX, centreY, DEFAULT_WEIGHT);

    listPushBack(this->molecules, molecule);
}