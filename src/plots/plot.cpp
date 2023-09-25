#include "plot.h"

CoordinatePlane::CoordinatePlane(double xUnit,  double yUnit,
                                 double xStart, double yStart,  
                                 double width,  double height,
                                 sf::Text* yName, sf::Font* font) :
    xUnit (xUnit),
    yUnit (yUnit),
    xStart(xStart),
    yStart(yStart),
    width (width),
    height(height),
    yName (yName),
    font  (font)
    {}

CoordinatePlane::~CoordinatePlane() {
    this->xUnit    = NAN;
    this->yUnit    = NAN;
    this->xStart   = NAN;
    this->yStart   = NAN;
    this->width    = NAN;
    this->height   = NAN;
    this->yName    = nullptr;
    this->font     = nullptr;
}

double CoordinatePlane::getXUnit() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->xUnit;
}
double CoordinatePlane::getYUnit() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->yUnit;
}
double CoordinatePlane::getXStart() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->xStart;
}
double CoordinatePlane::getYStart() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->yStart;
}

double CoordinatePlane::getWidth() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->width;
}

double CoordinatePlane::getHeight() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->height;
}

void CoordinatePlane::drawUnits(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    char intToStr[MAX_UNIT_LEN];
    sprintf(intToStr, "%.1lf", 0);

    sf::Text unitTxt(intToStr, *font, 20);
    unitTxt.setColor(AXIS_COLOR);
    unitTxt.setPosition(this->xStart + FRAME_WIDTH, this->yStart + this->height - 4 * FRAME_WIDTH);

    texture.draw(unitTxt);

    sprintf(intToStr, "%.1lf", this->yUnit);
    unitTxt.setString(intToStr);
    unitTxt.setPosition(this->xStart + FRAME_WIDTH, this->yStart + 2 * this->height / 3 - 4 * FRAME_WIDTH);
    texture.draw(unitTxt);

    sprintf(intToStr, "%.1lf", 2 * this->yUnit);
    unitTxt.setString(intToStr);
    unitTxt.setPosition(this->xStart + FRAME_WIDTH, this->yStart + this->height / 3 - 4 * FRAME_WIDTH);
    texture.draw(unitTxt);

    texture.display();
}

void CoordinatePlane::drawFrame (sf::RenderTexture& texture) {
    sf::RectangleShape frame(sf::Vector2f(this->width - 2 * FRAME_WIDTH, this->height - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(this->xStart + FRAME_WIDTH, this->yStart + FRAME_WIDTH);
    texture.draw(frame);
}

void CoordinatePlane::draw(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    sf::Vector2f namePos = sf::Vector2f(this->xStart, yStart);
    if (this->yName) { 
        this->yName->setPosition(sf::Vector2f(FRAME_WIDTH, FRAME_WIDTH) + namePos + sf::Vector2f(2, 15));
        texture.draw(*this->yName);
    }

    drawFrame(texture);
    drawUnits(texture, coordStart);
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
    return sf::Vector2f(x * coordPlane.getXUnit(),
                        y * coordPlane.getYUnit());
}

sf::Vector2f vecGraphToCoord(sf::RenderTexture& texture, CoordinatePlane& coordPlane, double x, double y, const sf::Vector2f coordStart) {
    return sf::Vector2f((x - coordStart.x) / coordPlane.getXUnit(), 
                        (y + coordStart.y) / coordPlane.getYUnit());
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

CoordinatePlane* Plot::getPlane() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->plane;
}

void Plot::draw(sf::RenderTexture& texture, const sf::Vector2f coordStart) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->plane, "Plane object is null!",);
    ON_ERROR(!this->points, "Points are null!",);

    this->plane->draw(texture, coordStart);

    sf::VertexArray drawPoints(sf::Points, this->size % this->capacity);
    for (unsigned int i = 0; i < this->size % this->capacity; i++) {
        drawPoints[i].position = points[i];
        drawPoints[i].color    = sf::Color::Yellow;
    }

    texture.draw(drawPoints);
}

void Plot::addPoint(sf::Vector2f& point) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->plane, "Plane object is null!",);
    ON_ERROR(!this->points, "Points are null!",);

    if (this->size >= this->capacity - 1) {
        for (size_t i = 0; i < this->capacity - 1; i++) {
            this->points[i] = this->points[i + 1];
            this->points[i].x--;
        }
        point.x = 960;
        this->points[this->capacity - 1] = point;
    }
    else this->points[size++] = point; 
}