#ifndef _BUTTON_h_
#define _BUTTON_h_

#include "../constants.h"

class Controller;
typedef void (*ButtonFunc)(Controller& manager);

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
protected:
    double height = 0;
public:
    SquareButton(double x, double y, double width, double height, ButtonFunc onClick);
    SquareButton(double x, double y, double width, double height, sf::Text* text, ButtonFunc onClick);

    void draw(sf::RenderTexture& drawTexture) override;
    bool isInside(sf::Vector2i mousePosition) override;
};

class CircleImageButton : public CircleButton {
private:
    sf::Texture* texture;

public:
    CircleImageButton(double x, double y, double radius, ButtonFunc onClick);
    CircleImageButton(double x, double y, double radius, sf::Texture* texture, ButtonFunc onClick);

    void draw(sf::RenderTexture& drawTexture) override;
};

class SquareImageButton : public SquareButton {
private:
    sf::Texture* texture;

public:
    SquareImageButton(double x, double y, double width, double height, ButtonFunc onClick);
    SquareImageButton(double x, double y, double width, double height, sf::Texture* texture, ButtonFunc onClick);

    void draw(sf::RenderTexture& drawTexture) override;
};

#endif