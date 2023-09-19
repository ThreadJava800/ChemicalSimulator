#include "manager/manager.h"

void printTest() {
    std::cout << "test\n";
}

int main() {
    srand((unsigned) time(NULL));

    std::cout << sizeof(sf::Font);

    sf::RenderWindow window(sf::VideoMode(), "Chemical simulator", sf::Style::Fullscreen);
    window.setPosition(sf::Vector2i(0, 0));

    sf::RenderTexture moleculeTexture;
    moleculeTexture.create(960, 800);
    moleculeTexture.setSmooth(true);

    sf::RenderTexture buttonTexture;
    buttonTexture.create(960, 160);
    buttonTexture.setSmooth(true);

    sf::Font font;
    font.loadFromFile(DEFAULT_FONT);
    sf::Text text = sf::Text("Test", font, 10);
    text.setColor(sf::Color::White);
    CircleButton addBtn = CircleButton(10, 10, 30, &text, printTest);

    Button** buttons = new Button*[1];
    buttons[0] = &addBtn;

    Manager moleculeManager = Manager(buttons, 1);
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

    sf::Sprite buttonSprite(buttonTexture.getTexture());
    buttonSprite.setPosition(0, 800);

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
            case sf::Event::MouseButtonPressed:
                // std::cout << "Test";
                if (event.mouseButton.button == sf::Mouse::Left) 
                    // std::cout << "test";
                    moleculeManager.registerClick(buttonTexture, buttonSprite.getPosition());
                break;
            }
        }

        moleculeTexture.clear();
        buttonTexture  .clear();
        window.clear();

        moleculeManager.moveAllObjects(moleculeTexture);
        moleculeManager.drawAll(moleculeTexture, buttonTexture);
        moleculeTexture.display();
        buttonTexture  .display();

        window.draw(moleculeSprite);
        window.draw(buttonSprite);
        window.display();
    }

    delete[] buttons;
    

    return 0;
}