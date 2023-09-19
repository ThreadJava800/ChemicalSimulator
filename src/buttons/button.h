#ifndef _BUTTON_h_
#define _BUTTON_h_

#include "../constants.h"
#include "../manager/manager.h"

typedef void (*ButtonFunc)(Manager& manager, sf::RenderTexture& moleculeTexture);

class Button {
protected:
    double     x        = 0;
    double     y        = 0;
    double     size     = 0;
    sf::Text*  text     = nullptr;
    ButtonFunc onClick  = nullptr;

public:
    Button();
    Button(double x, double y, double size, ButtonFunc onClick);
    Button(double x, double y, double size, sf::Text* text, ButtonFunc onClick);

    ButtonFunc getFunc();

    virtual void draw(sf::RenderTexture& drawTexture) = 0;
    virtual bool isInside(sf::Vector2i mousePosition) = 0;
};

class CircleButton : public Button {
public:
    CircleButton(double x, double y, double size, ButtonFunc onClick);
    CircleButton(double x, double y, double size, sf::Text* text, ButtonFunc onClick);

    void draw(sf::RenderTexture& drawTexture) override;
    bool isInside(sf::Vector2i mousePosition) override;
};

class SquareButton : public Button {
private:
    double height = 0;
public:
    SquareButton(double x, double y, double width, double height, ButtonFunc onClick);
    SquareButton(double x, double y, double width, double height, sf::Text* text, ButtonFunc onClick);

    void draw(sf::RenderTexture& drawTexture) override;
    bool isInside(sf::Vector2i mousePosition) override;
};

#endif