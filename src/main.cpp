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
    buttonTexture.create(960, 240);
    buttonTexture.setSmooth(true);
    sf::Sprite buttonSprite(buttonTexture.getTexture());
    buttonSprite.setPosition(0, 800);

    sf::RenderTexture plotTexture;
    plotTexture.create(960, 1080);
    plotTexture.setSmooth(true);
    sf::Sprite plotSprite(plotTexture.getTexture());
    plotSprite.setPosition(960, 0);

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

    sf::Text testPlaneName = sf::Text(L" Темп.", font, 20);
    sf::Text testPlaneUnit = sf::Text(" 540K", font, 15);
    CoordinatePlane testPlane = CoordinatePlane(240, 240, 50, 50, 0, 0, 480, 540, &testPlaneName, &testPlaneUnit);
    Plot testPlt = Plot(&testPlane, 480);

    sf::Text crclPlaneName = sf::Text(L" ○", font, 20);
    sf::Text crclPlaneUnit = sf::Text(L" 5 ○", font, 15);
    CoordinatePlane crclPlane = CoordinatePlane(620, 240, 50, 50, 480, 0, 480, 540, &crclPlaneName, &crclPlaneUnit);
    Plot crclPlot = Plot(&crclPlane, 480);

    sf::Text sqrPlaneName = sf::Text(L" □", font, 20);
    sf::Text sqrPlaneUnit = sf::Text(L" 5 □", font, 15);
    CoordinatePlane sqrPlane = CoordinatePlane(240, 240, 50, 50, 0, 480, 480, 540, &sqrPlaneName, &sqrPlaneUnit);
    Plot sqrtPlt = Plot(&sqrPlane, 480);

    sf::Text pressPlaneName = sf::Text(L" Давление", font, 20);
    sf::Text pressPlaneUnit = sf::Text(L" 1000 у.е.", font, 15);
    CoordinatePlane pressPlane = CoordinatePlane(620, 240, 50, 50, 480, 480, 480, 540, &pressPlaneName, &pressPlaneUnit);
    Plot pressPlot = Plot(&pressPlane, 480);

    UIManager   btnManager = UIManager  (&buttonTexture, &buttonSprite, buttons, BUTTON_CNT);
    MolManager  molManager = MolManager (&moleculeTexture, &moleculeSprite, 0, 273.15);
    PlotManager pltManager = PlotManager(&plotTexture, &plotSprite, &testPlt, &crclPlot, &sqrtPlt, &pressPlot);
    Controller  bossOfGym  = Controller (&btnManager, &molManager, &pltManager);

    for (unsigned int i = 0; i < START_MOL_CNT; i++)
        bossOfGym.getMolManager()->addMolecule(CIRCLE,
                            rand() % (moleculeTexture.getSize().x - int(DEFAULT_SIZE * 2)), 
                            rand() % (moleculeTexture.getSize().y - int(DEFAULT_SIZE * 2)));

    size_t frameCnt = 0;
    size_t delay    = 100;

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
                    bossOfGym.registerClick();
                break;
            }
        }

        window.clear();

        bossOfGym.update();
        if (++delay > FRAME_DELAY) {
            delay = 0;
            bossOfGym.updatePlot((frameCnt++) % 480);
        }

        window.draw(moleculeSprite);
        window.draw(buttonSprite);
        window.draw(plotSprite);

        window.display();
    }

    delete[] buttons;

    return 0;
}