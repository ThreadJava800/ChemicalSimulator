#include "objects.h"

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

void BaseMolecule::setSpeed(double velX, double velY) {
    ON_ERROR(!this, "Object pointer was null!",);

    if (velY == 0) return;

    this->velocityX *= velX;
    this->velocityY *= velY;
}

void BaseMolecule::resize(sf::RenderTexture& texture) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->size = this->weight * DEFAULT_SIZE;
}

void BaseMolecule::move(sf::RenderTexture& texture, double press, double temperature) {
    ON_ERROR(!this, "Object pointer was null!",);

    this->wallCollision(texture, press);

    double coeff = sqrt(3 * temperature / this->weight);

    this->x += velocityX * coeff;
    this->y += velocityY * coeff;
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

void CircleMolecule::wallCollision(sf::RenderTexture& texture, double press) {
    ON_ERROR(!this, "Object pointer was null!",);

    double rightWall = texture.getSize().x - FRAME_WIDTH;
    double downWall  = texture.getSize().y - FRAME_WIDTH;

    if (this->x <= FRAME_WIDTH) this->velocityX = fabs(this->velocityX);
    if (this->y <= FRAME_WIDTH + press) this->velocityY = fabs(this->velocityY);    

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

void SquareMolecule::wallCollision(sf::RenderTexture& texture, double press) {
    ON_ERROR(!this, "Object pointer was null!",);

    double rightWall = texture.getSize().x - FRAME_WIDTH;
    double downWall  = texture.getSize().y - FRAME_WIDTH;

    if (this->x <= FRAME_WIDTH) this->velocityX = fabs(this->velocityX);
    if (this->y <= FRAME_WIDTH + press) this->velocityY = fabs(this->velocityY);    

    if (this->x + this->size >= rightWall) this->velocityX = -1 * fabs(this->velocityX);
    if (this->y + this->size >= downWall)  this->velocityY = -1 * fabs(this->velocityY);
}