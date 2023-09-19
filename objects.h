#ifndef _OBJ_h_
#define _OBJ_h_

#include <SFML/Graphics.hpp>

const double START_IMPULSE  = 0.0009;
const double START_VELOCITY = sqrt(START_IMPULSE);

enum MoleculeType {
    ERROR_TYPE,
    SQUARE,
    CIRCLE
};

class BaseMolecule {
protected:
    double       x         = 0;
    double       y         = 0;
    double       size      = 0;
    double       velocityX = 0;
    double       velocityY = 0;
    unsigned int weight    = 0;
    MoleculeType type      = ERROR_TYPE;

public:
    explicit BaseMolecule(double x, double y, double size, unsigned int weight, MoleculeType type);
    
    void move     (sf::RenderTexture& texture, double press, double temperature);
    void addWeight(unsigned int weightAdd);

    double       getX     ();
    double       getY     ();
    double       getSize  ();
    double       getWeight();
    MoleculeType getType  ();

    void setSpeed(double velX, double velY);

    virtual void draw(sf::RenderTexture& texture) = 0;
    virtual void wallCollision(sf::RenderTexture& texture, double press) = 0;
};

class CircleMolecule : public BaseMolecule {
public:
    explicit CircleMolecule();
    explicit CircleMolecule(double x, double y, unsigned int weight);

    ~CircleMolecule();

    void draw         (sf::RenderTexture& texture)               override;
    void wallCollision(sf::RenderTexture& texture, double press) override;
};

class SquareMolecule : public BaseMolecule {
public:
    explicit SquareMolecule();
    explicit SquareMolecule(double x, double y, unsigned int weight);

    ~SquareMolecule();

    void draw         (sf::RenderTexture& texture)               override;
    void wallCollision(sf::RenderTexture& texture, double press) override;
};

#endif