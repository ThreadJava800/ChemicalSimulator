#include "manager/manager.h"

int main() {
    srand((unsigned) time(NULL));

    sf::Font font;
    font.loadFromFile(DEFAULT_FONT);

    sf::RenderWindow window(sf::VideoMode(), "Chemical simulator", sf::Style::Fullscreen);
    window.setPosition(sf::Vector2i(0, 0));

    sf::RenderTexture moleculeTexture;
    moleculeTexture.create(960, 800);
    moleculeTexture.setSmooth(true);
    sf::Sprite moleculeSprite(moleculeTexture.getTexture());
    moleculeSprite.setPosition(0, 0);

    sf::RenderTexture buttonTexture;
    buttonTexture.create(960, 160);
    buttonTexture.setSmooth(true);
    sf::Sprite buttonSprite(buttonTexture.getTexture());
    buttonSprite.setPosition(0, 800);

    Manager bossOfGym = Manager();
    for (unsigned int i = 0; i < START_MOL_CNT; i++)
        bossOfGym.addCircle(rand() % (moleculeTexture.getSize().x - int(DEFAULT_SIZE * 2)), 
                            rand() % (moleculeTexture.getSize().y - int(DEFAULT_SIZE * 2)));

    sf::Text addCrcl = sf::Text(L"+○", font, 40);
    CircleButton addCrclBtn = CircleButton(20, 20, 60, &addCrcl, addCircle);
    sf::Text addSqua = sf::Text(L"+□", font, 40);
    CircleButton addSquaBtn = CircleButton(180, 20, 60, &addSqua, addSquare);
    sf::Text prsUp   = sf::Text(L"↑", font, 40);
    SquareButton prsUpBtn = SquareButton(350, 30, 100, 100, &prsUp, pressUp);
    sf::Text prsDwn   = sf::Text(L"↓", font, 40);
    SquareButton prsDwnBtn = SquareButton(510, 30, 100, 100, &prsDwn, pressDown);
    sf::Text tmpUp   = sf::Text(L"+°", font, 40);
    CircleButton prstmpUpBtn = CircleButton(670, 20, 60, &tmpUp, tempUp);
    sf::Text tmpDwn   = sf::Text(L"-°", font, 40);
    CircleButton prstmpDwnBtn = CircleButton(820, 20, 60, &tmpDwn, tempDown);

    Button** buttons = new Button*[BUTTON_CNT];
    buttons[0] = &addCrclBtn;
    buttons[1] = &addSquaBtn;
    buttons[2] = &prsUpBtn;
    buttons[3] = &prsDwnBtn;
    buttons[4] = &prstmpUpBtn;
    buttons[5] = &prstmpDwnBtn;

    bossOfGym.setButtons(buttons, BUTTON_CNT);

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
                if (event.mouseButton.button == sf::Mouse::Left) 
                    bossOfGym.registerClick(moleculeTexture, buttonTexture, buttonSprite.getPosition());
                break;
            }
        }

        moleculeTexture.clear();
        buttonTexture  .clear();
        window         .clear();

        bossOfGym.moveAllObjects(moleculeTexture);
        bossOfGym.drawAll(moleculeTexture, buttonTexture);

        moleculeTexture.display();
        buttonTexture  .display();

        window.draw(moleculeSprite);
        window.draw(buttonSprite);

        window.display();
    }

    delete[] buttons;
    

    return 0;
}