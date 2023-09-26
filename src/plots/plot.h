#ifndef _PLOT_h_
#define _PLOT_h_

#include "../constants.h"
#include "../../libs/list.h"

class CoordinatePlane {
private:
    double xUnit,  yUnit;
    double xStart, yStart;
    double width,  height;
        
    size_t secondsCount;
    size_t frameCount;
    bool   hasOverflowedX;

    sf::Text* yName;
    sf::Font* font;

    struct XAxisTxt {
        double xCoord = 0;
        size_t xText  = 0;
    };

    List<XAxisTxt>* xAxisUnits;

    void drawYUnits(sf::RenderTexture& texture);
    void drawXUnits(sf::RenderTexture& texture, const double duration);
    void drawFrame (sf::RenderTexture& texture);

public:
    CoordinatePlane(double xUnit,     double yUnit,
                    double xStart,    double yStart,  
                    double width,     double height,
                    sf::Text*  yName, sf::Font* font);
    ~CoordinatePlane();

    double getXUnit ();
    double getYUnit ();
    double getXStart();
    double getYStart();
    double getWidth ();
    double getHeight();

    void draw(sf::RenderTexture& texture, const double duration);
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

    double duration     = NAN;

public:
    Plot();
    Plot(CoordinatePlane* plane, unsigned int capacity);

    ~Plot();

    CoordinatePlane* getPlane();

    void setDuration(const double _duration);
    void draw(sf::RenderTexture& texture);
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