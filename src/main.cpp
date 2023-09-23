#include "manager/manager.h"

int main() {
    srand((unsigned) time(NULL));

    sf::Font font;
    font.loadFromFile(DEFAULT_FONT);

    sf::RenderWindow window(sf::VideoMode(), "Chemical simulator", sf::Style::Fullscreen);
    window.setPosition(sf::Vector2i(0, 0));

    sf::RenderTexture moleculeTexture;
    moleculeTexture.create(960, 880);
    moleculeTexture.setSmooth(true);
    sf::Sprite moleculeSprite(moleculeTexture.getTexture());
    moleculeSprite.setPosition(0, 0);

    sf::RenderTexture buttonTexture;
    buttonTexture.create(960, 160);
    buttonTexture.setSmooth(true);
    sf::Sprite buttonSprite(buttonTexture.getTexture());
    buttonSprite.setPosition(0, 880);

    sf::RenderTexture plotTexture;
    plotTexture.create(960, 1040);
    plotTexture.setSmooth(true);
    sf::Sprite plotSprite(plotTexture.getTexture());
    plotSprite.setPosition(960, 0);

    sf::Texture addCircleTexture;
    addCircleTexture.loadFromFile(ADD_CRCL);
    CircleImageButton addCrclBtn = CircleImageButton(20, 20, 60, &addCircleTexture, addCircle);

    sf::Texture addSquareTexture;
    addSquareTexture.loadFromFile(ADD_SQUARE);
    SquareImageButton addSquaBtn = SquareImageButton(180, 20, 120, 120, &addSquareTexture, addSquare);

    sf::Texture addPrsUpTexture;
    addPrsUpTexture.loadFromFile(PRESS_UP);
    SquareImageButton prsUpBtn = SquareImageButton(350, 20, 120, 120, &addPrsUpTexture, pressUp);

    sf::Texture addPrsDwnTexture;
    addPrsDwnTexture.loadFromFile(PRESS_DOWN);
    SquareImageButton prsDwnBtn = SquareImageButton(510, 20, 120, 120, &addPrsDwnTexture, pressDown);

    sf::Texture addTempUpTexture;
    addTempUpTexture.loadFromFile(TEMP_UP);
    CircleImageButton prstmpUpBtn = CircleImageButton(670, 20, 60, &addTempUpTexture, tempUp);

    sf::Texture addTempDwnTexture;
    addTempDwnTexture.loadFromFile(TEMP_DOWN);
    CircleImageButton prstmpDwnBtn = CircleImageButton(820, 20, 60, &addTempDwnTexture, tempDown);

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

    sf::Texture plotBackground;
    ON_ERROR(!plotBackground.loadFromFile(PLOT_BACK), "Can't open file", -1);
    sf::Sprite plotBackgrSprite;
    plotBackgrSprite.setTexture(plotBackground);
    plotBackgrSprite.setPosition(-300, 0);

    sf::Texture btnBackground;
    ON_ERROR(!btnBackground.loadFromFile(BTN_BACK), "Can't open file", -1);
    sf::Sprite btnBackgrSprite;
    btnBackgrSprite.setTexture(btnBackground);
    btnBackgrSprite.setPosition(-50, -10);

    sf::Texture molBackground;
    ON_ERROR(!molBackground.loadFromFile(MOL_BACK), "Can't open file", -1);
    sf::Sprite molBackgrSprite;
    molBackgrSprite.setTexture(molBackground);
    molBackgrSprite.setPosition(-50, -10);

    UIManager   btnManager = UIManager  (&buttonTexture, &buttonSprite, buttons, BUTTON_CNT, &btnBackgrSprite);
    MolManager  molManager = MolManager (&moleculeTexture, &moleculeSprite, FRAME_WIDTH, 273.15, &molBackgrSprite);
    PlotManager pltManager = PlotManager(&plotTexture, &plotSprite, &testPlt, &crclPlot, &sqrtPlt, &pressPlot, &plotBackgrSprite);
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