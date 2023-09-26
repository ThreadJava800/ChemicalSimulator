#include "manager/manager.h"

int main() {
    srand((unsigned) time(NULL));

    sf::Font font;
    font.loadFromFile(PLOT_FONT);

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

    sf::Text testPlaneName = sf::Text(L" Temp.", font, 20);
    sf::Text testPlaneUnit = sf::Text(" 540K", font, 15);
    CoordinatePlane testPlane = CoordinatePlane(50, 323, 0, 0, 960, 270, &testPlaneName, &font);
    Plot testPlt = Plot(&testPlane, 960);

    sf::Text crclPlaneName = sf::Text(L" Circles", font, 20);
    CoordinatePlane crclPlane = CoordinatePlane(50, 25, 0, 250, 960, 270, &crclPlaneName, &font);
    Plot crclPlot = Plot(&crclPlane, 960);

    sf::Text sqrPlaneName = sf::Text(L" Squares", font, 20);
    CoordinatePlane sqrPlane = CoordinatePlane(50, 5, 0, 500, 960, 270, &sqrPlaneName, &font);
    Plot sqrtPlt = Plot(&sqrPlane, 960);

    sf::Text pressPlaneName = sf::Text(L" Pressure", font, 20);
    sf::Text pressPlaneUnit = sf::Text(L" 1000 c.u.", font, 15);
    CoordinatePlane pressPlane = CoordinatePlane(50, 12.3, 0, 770, 960, 270, &pressPlaneName, &font);
    Plot pressPlot = Plot(&pressPlane, 960);

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

    std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();

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
            std::chrono::_V2::system_clock::time_point end = std::chrono::high_resolution_clock::now();
            bossOfGym.updatePlot((frameCnt++) % 960, std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
        }

        window.draw(moleculeSprite);
        window.draw(buttonSprite);
        window.draw(plotSprite);

        window.display();
    }

    delete[] buttons;

    return 0;
}