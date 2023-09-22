#include "plot.h"

CoordinatePlane::CoordinatePlane(double xOrigin, double yOrigin, double xUnit, double yUnit) :
    xOrigin(xOrigin),
    yOrigin(yOrigin),
    xUnit  (xUnit),
    yUnit  (yUnit)             {}

CoordinatePlane::~CoordinatePlane() {
    this->xOrigin = NAN;
    this->yOrigin = NAN;
    this->xUnit   = NAN;
    this->yUnit   = NAN;
}

double CoordinatePlane::getXOrigin() {
    return this->xOrigin;
}
double CoordinatePlane::getYOrigin() {
    return this->yOrigin;
}
double CoordinatePlane::getXUnit() {
    return this->xUnit;
}
double CoordinatePlane::getYUnit() {
    return this->yUnit;
}

void CoordinatePlane::drawPoints(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    // unit points on x axis
    for (int i = xOrigin; i >= 0; i -= xUnit) {
        sf::RectangleShape point(sf::Vector2f(UNIT_POINT_RAD, UNIT_POINT_RAD));

        point.setPosition(sf::Vector2f(i - UNIT_POINT_RAD / 2, yOrigin - UNIT_POINT_RAD / 2));
        point.setFillColor(DEFAULT_UNIT_POINT_COL);

        texture.draw(point);
    }
    for (int i = xOrigin; i <= texture.getSize().x + coordStart.x; i += xUnit) {
        sf::RectangleShape point(sf::Vector2f(UNIT_POINT_RAD, UNIT_POINT_RAD));

        point.setPosition(sf::Vector2f(i - UNIT_POINT_RAD / 2, yOrigin - UNIT_POINT_RAD / 2));
        point.setFillColor(DEFAULT_UNIT_POINT_COL);

        texture.draw(point);
    }

    // unit points on y axis
    for (int i = yOrigin; i >= 0; i -= yUnit) {
        sf::RectangleShape point(sf::Vector2f(UNIT_POINT_RAD, UNIT_POINT_RAD));

        point.setPosition(sf::Vector2f(xOrigin - UNIT_POINT_RAD / 2, i - UNIT_POINT_RAD / 2));
        point.setFillColor(DEFAULT_UNIT_POINT_COL);

        texture.draw(point);
    }
    for (int i = yOrigin; i < texture.getSize().y + coordStart.y; i += yUnit) {
        sf::RectangleShape point(sf::Vector2f(UNIT_POINT_RAD, UNIT_POINT_RAD));

        point.setPosition(sf::Vector2f(xOrigin - UNIT_POINT_RAD / 2, i - UNIT_POINT_RAD / 2));
        point.setFillColor(DEFAULT_UNIT_POINT_COL);

        texture.draw(point);
    }
}

void CoordinatePlane::drawFrame (sf::RenderTexture& texture) {
    sf::RectangleShape frame(sf::Vector2f(texture.getSize().x - 2 * FRAME_WIDTH, texture.getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    texture.draw(frame);
}

void CoordinatePlane::draw(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    sf::Vector2f oxStart = vecGraphToCoord(texture, *this, coordStart.x, this->yOrigin + coordStart.y, coordStart);
    sf::Vector2f oxEnd   = vecGraphToCoord(texture, *this, coordStart.x + texture.getSize().x, this->yOrigin + coordStart.y, coordStart);
    Vector       ox      = Vector(oxEnd.x, oxEnd.y, sf::Color::White);

    sf::Vector2f oyStart = vecGraphToCoord(texture, *this, this->xOrigin + coordStart.x, coordStart.y + texture.getSize().y, coordStart);
    sf::Vector2f oyEnd   = vecGraphToCoord(texture, *this, this->xOrigin + coordStart.x, coordStart.y, coordStart);
    Vector       oy      = Vector(oyEnd.x, oyEnd.y, sf::Color::White);

    ox.draw(texture, *this, oxStart.x, oxStart.y);
    oy.draw(texture, *this, oyStart.x, oyStart.y);

    drawPoints(texture, coordStart);
    drawFrame (texture);
}

Vector::Vector() :
    x    (0),
    y    (0),
    color(DEFAULT_LINE_COLOR) {}

Vector::Vector(double x, double y) :
    x    (x),
    y    (y),
    color(DEFAULT_LINE_COLOR) {}

Vector::Vector(double x, double y, sf::Color color) :
    x    (x),
    y    (y),
    color(color)              {}

Vector::~Vector() {
    this->x = NAN;
    this->y = NAN;
    this->color = DEFAULT_LINE_COLOR;
}

double Vector::getX() {
    return this->x;
}

double Vector::getY() {
    return this->y;
}

sf::Color Vector::getColor() {
    return this->color;
}

inline double Vector::len() const {
    return sqrt(this->x * this->x + this->y * this->y);
}

sf::Vector2f vecCoordToGraph(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double x, double y) {
    return sf::Vector2f(coordPlane.getXOrigin() + x * coordPlane.getXUnit(),
                        coordPlane.getYOrigin() - y * coordPlane.getYUnit());
}

sf::Vector2f vecGraphToCoord(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double x, double y, const sf::Vector2f coordStart) {
    return sf::Vector2f((x - coordPlane.getXOrigin() - coordStart.x) / coordPlane.getXUnit(), 
                        (coordPlane.getYOrigin() - y + coordStart.y) / coordPlane.getYUnit());
}

void Vector::setX(double x) {
    if (x != NAN) this->x = x;
    else std::cerr << "Value was NAN\n";
}

void Vector::setY(double y) {
    if (y != NAN) this->y = y;
    else std::cerr << "Value was NAN\n";
}

void Vector::setColor(sf::Color color) {
    this->color = color;
}

void Vector::drawArrowheads(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double xStart, double yStart) {
    sf::VertexArray vecHead = sf::VertexArray(sf::Triangles, 3);
    Vector startVec = Vector(xStart, yStart, this->color);

    Vector leftArrow = (*this) - startVec;
    leftArrow = !(leftArrow) * ARROW_SIZE_COEF - (leftArrow) * ARROW_SIZE_COEF;
    leftArrow = leftArrow + Vector(this->x, this->y); // parallel move of vector

    Vector rightArrow = (*this) - startVec;
    rightArrow = !(rightArrow) * -ARROW_SIZE_COEF - (rightArrow) * ARROW_SIZE_COEF;
    rightArrow = rightArrow + Vector(this->x, this->y);

    vecHead[0].position = vecCoordToGraph(texture, coordPlane, leftArrow.x, leftArrow.y);
    vecHead[1].position = vecCoordToGraph(texture, coordPlane, rightArrow.x, rightArrow.y);
    vecHead[2].position = vecCoordToGraph(texture, coordPlane, this->x, this->y);

    vecHead[0].color    = this->color;
    vecHead[1].color    = this->color;
    vecHead[2].color    = this->color;

    texture.draw(vecHead);
}

void Vector::drawLine(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double xStart, double yStart) {
    sf::VertexArray lineObject(sf::LinesStrip, 2);

    lineObject[0].position = vecCoordToGraph(texture, coordPlane, xStart, yStart);
    lineObject[1].position = vecCoordToGraph(texture, coordPlane, this->x, this->y);

    lineObject[0].color = this->color;
    lineObject[1].color = this->color;

    texture.draw(lineObject);
}

void Vector::draw(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double xStart, double yStart) {
    drawLine(texture, coordPlane, xStart, yStart);
    drawArrowheads(texture, coordPlane, xStart, yStart);
}

Vector operator+(const Vector& a, const Vector& b) {
    sf::Color resColor = (a.color == b.color) ? a.color : DEFAULT_LINE_COLOR;
    return Vector(a.x + b.x, a.y + b.y, resColor);
}

Vector operator-(const Vector& a, const Vector& b) {
    sf::Color resColor = (a.color == b.color) ? a.color : DEFAULT_LINE_COLOR;
    return Vector(a.x - b.x, a.y - b.y, resColor);
}

Vector operator*(const Vector& a, double scalar) {
    return Vector(scalar * a.x, scalar * a.y, a.color);
}

Vector operator!(const Vector& a) {
    return Vector((-1 * a.y) / a.len(), a.x / a.len(), a.color);
}

Plot::Plot() :
    plane (nullptr),
    points(nullptr),
    size  (0)
    {}

Plot::Plot(CoordinatePlane* plane, unsigned int capacity) :
    plane   (plane),
    capacity(capacity),
    size    (0)
{
    ON_ERROR(!this, "Object pointer was null!",);

    this->points = new sf::Vector2f[capacity];
    ON_ERROR(!this->points, "Unable to allocate memory!",);
}

Plot::~Plot() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->plane    = nullptr;
    this->capacity = 0;
    this->size     = 0;

    delete[] this->points;
    this->points   = nullptr;
}

void Plot::draw(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->plane, "Plane object is null!",);
    ON_ERROR(!this->points, "Points are null!",);

    this->plane->draw(texture, coordStart);

    sf::VertexArray drawPoints(sf::Points, this->size % this->capacity);
    for (unsigned int i = 0; i < this->size % this->capacity; i++) {
        drawPoints[i].position = points[i];
        drawPoints[i].color    = sf::Color::Blue;
    }

    texture.draw(drawPoints);
}

void Plot::addPoint(sf::Vector2f& point) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->plane, "Plane object is null!",);
    ON_ERROR(!this->points, "Points are null!",);

    this->points[(size++) % this->capacity] = point; 
}