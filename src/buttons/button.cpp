#include "button.h"

Button::Button() :
    x       (0),
    y       (0),
    size    (0),
    text    (nullptr),
    onClick (nullptr)
    {}

Button::Button(double x, double y, double size, ButtonFunc onClick) :
    x       (x),
    y       (y),
    size    (size),
    text    (nullptr),
    onClick (onClick)
    {}

Button::Button(double x, double y, double size, sf::Text* text, ButtonFunc onClick) :
    x       (x),
    y       (y),
    size    (size),
    text    (text),
    onClick (onClick)
    {}

CircleButton::CircleButton(double x, double y, double size, ButtonFunc onClick) :
      Button(x, y, size, onClick) {}

CircleButton::CircleButton(double x, double y, double size, sf::Text* text, ButtonFunc onClick) :
      Button(x, y, size, text, onClick) {}

void CircleButton::draw(sf::RenderTexture& drawTexture) {
    ON_ERROR(!this, "Object pointer was null!",);

    sf::CircleShape circle(this->size);
    circle.setFillColor(sf::Color::Black);
    circle.setPosition(this->x, this->y);

    if (this->text) {
        this->text->setOrigin(this->text->getGlobalBounds().getSize() / 2.f + this->text->getLocalBounds().getPosition());
        this->text->setPosition(circle.getPosition() + sf::Vector2f(circle.getRadius() / 2.f, circle.getRadius() / 2.f));
    }

    drawTexture.draw(*this->text);
    drawTexture.draw(circle);
}

bool CircleButton::isInside(sf::Vector2f mousePosition) {
    ON_ERROR(!this, "Object pointer was null!", false);

    double centreX = this->x + this->size;
    double centreY = this->y + this->size;

    double shiftX2 = (mousePosition.x - centreX) * (mousePosition.x - centreX);
    double shiftY2 = (mousePosition.y - centreY) * (mousePosition.y - centreY);

    return shiftX2 + shiftY2 <= this->size * this->size;
}

SquareButton::SquareButton(double x, double y, double width, double height, ButtonFunc onClick) :
    Button(x, y, width, onClick),
    height(height)
    {}

SquareButton::SquareButton(double x, double y, double width, double height, sf::Text* text, ButtonFunc onClick) :
    Button(x, y, width, text, onClick),
    height(height)
    {}

void SquareButton::draw(sf::RenderTexture& drawTexture) {
    ON_ERROR(!this, "Object pointer was null!",);

    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(this->size, this->height));
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(10);
    rect.setPosition(this->x, this->y);

    if (this->text) {
        this->text->setOrigin(this->text->getGlobalBounds().getSize() / 2.f + this->text->getLocalBounds().getPosition());
        this->text->setPosition(rect.getPosition() + (rect.getSize() / 2.f));
    }

    drawTexture.draw(*this->text);
    drawTexture.draw(rect);
}
bool SquareButton::isInside(sf::Vector2f mousePosition) {
    ON_ERROR(!this, "Object pointer was null!", false);

    double centreX = this->x + this->size   / 2;
    double centreY = this->y + this->height / 2;

    double shiftX = abs(mousePosition.x - centreX);
    double shiftY = abs(mousePosition.y - centreY);

    return (shiftX <= this->size / 2) && (shiftY <= this->height / 2);
}