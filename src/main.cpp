#include "manager/manager.h"

int main() {
    srand((unsigned) time(NULL));

    sf::RenderWindow window(sf::VideoMode(), "Chemical simulator", sf::Style::Fullscreen);
    window.setPosition(sf::Vector2i(0, 0));

    sf::RenderTexture moleculeTexture;
    moleculeTexture.create(960, 1040);
    moleculeTexture.setSmooth(true);

    Manager moleculeManager = Manager();
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);
    moleculeManager.addMolecule(moleculeTexture, rand() % moleculeTexture.getSize().x, rand() % moleculeTexture.getSize().y);


    sf::Sprite moleculeSprite(moleculeTexture.getTexture());
    moleculeSprite.setPosition(0, 0);

    // sf::CircleShape shape(5);
    // shape.setPosition(0, 0);
    // shape.setFillColor(CIRCLE_COL);

    // moleculeTexture.draw(shape);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // close if escape clicked
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            }
        }

        moleculeTexture.clear();
        window.clear();

        moleculeManager.moveAll(moleculeTexture);
        moleculeManager.drawAll(moleculeTexture);
        moleculeTexture.display();

        window.draw(moleculeSprite);
        window.display();
    }
    

    return 0;
}