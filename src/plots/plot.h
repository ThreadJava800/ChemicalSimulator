#ifndef _PLOT_h_
#define _PLOT_h_

#include "../constants.h"

class CoordinatePlane {
private:
    double    xOrigin, yOrigin;
    double    xUnit  , yUnit;

    void drawPoints(sf::RenderTexture& texture, const sf::Vector2f coordStart);
    void drawFrame (sf::RenderTexture& texture);

public:
    CoordinatePlane(double xOrigin, double yOrigin, double xUnit, double yUnit);
    ~CoordinatePlane();

    double getXOrigin();
    double getYOrigin();
    double getXUnit  ();
    double getYUnit  ();

    void draw(sf::RenderTexture& texture, const sf::Vector2f coordStart);
};

class Vector {
private:
    double    x, y;
    sf::Color color;

    void drawArrowheads(sf::RenderTexture& texture, CoordinatePlane& coordPlane,
                        double xStart, double yStart);
    void drawLine      (sf::RenderTexture& texture, CoordinatePlane& coordPlane,
                        double xStart, double yStart);

public:
    Vector();
    Vector(double x, double y);
    Vector(double x, double y, sf::Color color);

    ~Vector();

    double    getX();
    double    getY();
    sf::Color getColor();

    void setX(double x);
    void setY(double y);
    void setColor(sf::Color color);

    inline double len() const;

    void draw(sf::RenderTexture& texture, CoordinatePlane& coordPlane, 
              double xStart = 0, double yStart = 0);

    friend Vector operator+(const Vector& a, const Vector& b);
    friend Vector operator-(const Vector& a, const Vector& b);
    friend Vector operator*(const Vector& a, double scalar);
    friend Vector operator!(const Vector& a);
};

class Plot {
private:
    CoordinatePlane* plane    = nullptr;
    sf::Vector2f*    points   = nullptr;
    unsigned int     capacity = 0;
    unsigned int     size     = 0;

public:
    Plot();
    Plot(CoordinatePlane* plane, unsigned int capacity);

    ~Plot();

    void draw(sf::RenderTexture& texture, const sf::Vector2f coordStart);
    void addPoint(sf::Vector2f& point);
};

sf::Vector2f vecGraphToCoord(sf::RenderTexture& texture, 
                             CoordinatePlane& coordPlane, 
                             double x, 
                             double y,
                             const sf::Vector2f coordStart);

sf::Vector2f vecCoordToGraph(sf::RenderTexture& texture, 
                             CoordinatePlane& coordPlane, 
                             double x, 
                             double y);

#endif