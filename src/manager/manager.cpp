#include "manager.h"

BaseManager::BaseManager() :
    texture(nullptr),
    sprite (nullptr)
    {}

BaseManager::BaseManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, sf::Sprite* backgroundImg) :
    texture      (_texture),
    sprite       (_sprite),
    backgroundImg(backgroundImg)
    {}

BaseManager::~BaseManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->sprite        = nullptr;
    this->texture       = nullptr;
    this->backgroundImg = nullptr;
}

sf::RenderTexture* BaseManager::getTexture() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->texture;
}

sf::Sprite* BaseManager::getSprite () {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->sprite;
}

UIManager::UIManager() :
    BaseManager(),
    buttons    (nullptr),
    btnCnt     (0)
    {}

UIManager::UIManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, Button** buttons, unsigned int btnCnt, sf::Sprite* backgroundImg) :
    BaseManager(_texture, _sprite, backgroundImg),
    buttons    (buttons),
    btnCnt     (btnCnt)
    {}

UIManager::~UIManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->buttons = nullptr;
    this->btnCnt  = 0;
}

Button** UIManager::getButtons() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->buttons;
}

unsigned int UIManager::getBtnCnt() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->btnCnt;
}

void UIManager::draw() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->buttons, "Button array is null!",);
    ON_ERROR(!this->texture || !this->sprite, "Drawable area was null",);

    this->texture->clear();

    if (this->backgroundImg) this->texture->draw(*this->backgroundImg);

    for (unsigned int i = 0; i < this->btnCnt; i++) {
        Button* btn = this->buttons[i];
        if (btn) btn->draw(*(this->texture));
    }

    //draw frame
    sf::RectangleShape frame = sf::RectangleShape(sf::Vector2f(this->texture->getSize().x - 2 * FRAME_WIDTH, this->texture->getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::Red);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    this->texture->draw(frame);

    this->texture->display();
}

MolManager::MolManager() :
    BaseManager  (),
    molecules    (nullptr),
    pressY       (FRAME_WIDTH),
    temperature  (273.15)
    {}

MolManager::MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, double _pressY, double _temperature, sf::Sprite* backgroundImg) : 
    BaseManager  (_texture, _sprite, backgroundImg),
    pressY       (_pressY),
    temperature  (_temperature)
{
    ON_ERROR(!this, "Object pointer was null!",);

    this->molecules = new List<BaseMolecule*>();
}

MolManager::MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, List<BaseMolecule*>* _molecules, double _pressY, double _temperature, sf::Sprite* backgroundImg) :
    BaseManager  (_texture, _sprite, backgroundImg),
    molecules    (_molecules),
    pressY       (_pressY),
    temperature  (_temperature)
{
    ON_ERROR(!this, "Object pointer was null!",);

    this->molecules = new List<BaseMolecule*>();
}

MolManager::~MolManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    if (!(this->molecules)) return;

    for (size_t i = 0; i < this->molecules->getSize(); i++) {
        BaseMolecule* value = (*this->molecules)[i];
        if (value) delete value;
    }

    delete this->molecules;

    this->molecules     = nullptr;
    this->pressY        = 0;
}

double MolManager::getTemperature() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    this->temperature = getEnergy() * 2 / 3 * MY_BOLTZMANN;

    return this->temperature;
}

double MolManager::getEnergy() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    double energy = 0;

    size_t listLen = this->molecules->getSize();
    for (size_t i = 0; i < listLen; i++) {
        BaseMolecule* mol = (*this->molecules)[i];
        if (mol) {
            energy += mol->getWeight() * mol->getSpeed2() / 2;
        }
    }

    return energy;
}

uint MolManager::getMolTypeCount(MoleculeType type) {
    ON_ERROR(!this, "Object pointer was null!", 0);
    ON_ERROR(!this->molecules, "Array pointer was null!", 0);

    uint count = 0;

    size_t moleculeCount = this->molecules->getSize();
    for (size_t i = 0; i < moleculeCount; i++) {
        BaseMolecule* molecule = (*this->molecules)[i];
        if (molecule) {
            if (molecule->getType() == type) count++;
        }
    }

    return count;
}

double MolManager::getPressure() {
    ON_ERROR(!this, "Object pointer was null!", 0);
    ON_ERROR(!this->molecules, "List pointer was null!", 0);
    ON_ERROR(!this->texture, "Texture pointer was null!", 0);

    // p = nkT
    double N = this->molecules->getSize();
    double V = (this->texture->getSize().y - this->pressY) * (this->texture->getSize().x);
    double n = N / V;

    return n * MY_BOLTZMANN * getTemperature();
}

double MolManager::getPressY() {
    ON_ERROR(!this, "Object pointer was null!", 0);

    return this->pressY;
}

void MolManager::moveAll() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);
    ON_ERROR(!(this->texture), "Pointer to texture was null!",);

    for (size_t i = 0; i < this->molecules->getSize(); i++) {
        BaseMolecule* molecule = (*this->molecules)[i];
        if (molecule) molecule->move(*(this->texture), this->pressY, this->temperature);
    }
}

void MolManager::resizeAll() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);
    ON_ERROR(!(this->texture), "Pointer to texture was null!",);

    long moleculeCount = this->molecules->getSize();

    for (size_t i = 0; i < moleculeCount; i++) {
        BaseMolecule* molecule = (*this->molecules)[i];
        if (molecule) molecule->resize(*(this->texture));
    }
}

void MolManager::draw() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);
    ON_ERROR(!this->texture || !this->sprite, "Drawable area was null",);

    texture->clear();

    if (this->backgroundImg) texture->draw(*this->backgroundImg);

    // draw frame
    sf::RectangleShape frame = sf::RectangleShape(sf::Vector2f(texture->getSize().x - 2 * FRAME_WIDTH, texture->getSize().y - 2 * FRAME_WIDTH));
    frame.setFillColor   (sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(FRAME_WIDTH);
    frame.setPosition(FRAME_WIDTH, FRAME_WIDTH);
    texture->draw(frame);

    // draw press
    sf::RectangleShape press = sf::RectangleShape(sf::Vector2f(texture->getSize().x - 2 * FRAME_WIDTH, FRAME_WIDTH));
    press.setFillColor   (sf::Color::Transparent);
    press.setOutlineColor(sf::Color::Red);
    press.setOutlineThickness(FRAME_WIDTH);
    press.setPosition(FRAME_WIDTH, this->pressY);
    texture->draw(press);

    size_t moleculeCount = this->molecules->getSize();

    // draw molecules
    for (size_t i = 0; i < this->molecules->getSize(); i++) {
        BaseMolecule* molecule = (*this->molecules)[i];
        if (molecule) molecule->draw(*texture);
    }

    texture->display();
}

void MolManager::checkCollisions() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    size_t moleculeCount = this->molecules->getSize();

    for (size_t i = 0; i < moleculeCount; i++) {
        for (size_t j = i + 1; j < moleculeCount; j++) {
            tryCollide(i, j);
        }
    }

    for (size_t i = 0; i < moleculeCount; i++) {
        BaseMolecule* molecule = (*this->molecules)[i];
        if (!molecule) this->molecules->remove(i);
    }
}

void MolManager::tryCollide(long ind1, long ind2) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    List<BaseMolecule*>* listPointer = this->molecules;
    ON_ERROR(ind1 > listPointer->getSize() || ind2 > listPointer->getSize(), "Overflow attempt",);

    BaseMolecule* molecule1 = (*listPointer)[ind1];
    BaseMolecule* molecule2 = (*listPointer)[ind2];

    if (!molecule1 || !molecule2) return;

    static const CollideFunc checkCollisionFuncs[2][2] = {
        {collideSquares, collideSquareCircle},
        {collideSquareCircle, collideCircles}
    };

    static const ProceedFunc proceedCollideFuncs[2][2] = {
        {proceedSquares, proceedSquareCircle},
        {proceedCircleSquare, proceedCircles}
    };

    if (checkCollisionFuncs[molecule1->getType()][molecule2->getType()](molecule1, molecule2)) {
        proceedCollideFuncs[molecule1->getType()][molecule2->getType()](*this, listPointer, ind1, ind2);
    }
}

void MolManager::addMolecule(MoleculeType type, double x, double y, double vx, double vy) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    BaseMolecule* molecule = nullptr;

    switch (type)
    {
    case SQUARE:
        molecule = new SquareMolecule(x, y, DEFAULT_WEIGHT);
        break;
    case CIRCLE:
        molecule = new CircleMolecule(x, y, DEFAULT_WEIGHT);
        break;
    default:
        return;
    }

    molecule->setSpeed(vx, vy);
    this->molecules->pushBack(molecule);
}

void MolManager::movePress(MOVE_DIR dir, double shift) {
    ON_ERROR(!this, "Object pointer was null!",);

    switch (dir)
    {
    case UP:
        shift = -1 * fabs(shift);
        break;
    case DOWN:
        shift = fabs(shift);
        break;
    default:
        return;
    }

    this->pressY += shift;

    if (this->pressY <= FRAME_WIDTH) 
        this->pressY = FRAME_WIDTH;

    if (this->pressY >= this->getTexture()->getSize().y - PRS_MAX_SHIFT)
        this->pressY = this->getTexture()->getSize().y - PRS_MAX_SHIFT;
}

void MolManager::changeTemp(MOVE_DIR dir, double shift) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->molecules, "List pointer was null!",);

    switch (dir)
    {
    case DOWN:
        shift = -1 * fabs(shift);
        break;
    case UP:
        shift = fabs(shift);
        break;
    default:
        return;
    }

    double coeff = sqrt((this->temperature + shift) / this->temperature);

    size_t listSize = this->molecules->getSize();
    for (size_t i = 0; i < listSize; i++) {
        BaseMolecule* mol = (*this->molecules)[i];
        if (mol) {
            mol->setSpeed(coeff, coeff);
        }
    }
}

PlotManager::PlotManager() :
    BaseManager(),
    tempPlot  (nullptr),
    circlePlot(nullptr),
    squarePlot(nullptr),
    pressPlot (nullptr)
    {}

PlotManager::PlotManager(sf::RenderTexture* _texture, 
                         sf::Sprite* _sprite, 
                         Plot* _tempPlot, 
                         Plot* _circlePlot,
                         Plot* _squarePlot,
                         Plot* _pressPlot,
                         sf::Sprite* backgroundImg) :
    BaseManager  (_texture, _sprite, backgroundImg),
    tempPlot     (_tempPlot),
    circlePlot   (_circlePlot),
    squarePlot   (_squarePlot),
    pressPlot    (_pressPlot)
    {}

PlotManager::~PlotManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->tempPlot   = nullptr;
    this->circlePlot = nullptr;
    this->squarePlot = nullptr;
    this->pressPlot  = nullptr;

    this->backgroundImg = nullptr;
}

Plot* PlotManager::getTempPlot() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->tempPlot;
}

Plot* PlotManager::getCirclePlot() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->circlePlot;
}

Plot* PlotManager::getSquarePlot() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->squarePlot;
}

Plot* PlotManager::getPressPlot() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->pressPlot;
}

void PlotManager::drawBackground() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->texture->clear();
    if (this->texture) this->texture->draw(*(this->backgroundImg));
}

void PlotManager::draw() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->texture || !this->sprite, "Drawable area was null!",);
    
    drawBackground();

    if (tempPlot)   tempPlot  ->draw(*(this->texture));
    if (circlePlot) circlePlot->draw(*(this->texture));
    if (squarePlot) squarePlot->draw(*(this->texture));
    if (pressPlot)  pressPlot ->draw(*(this->texture));    

    this->texture->display();
}

void PlotManager::setXAxis(double duration) {
    ON_ERROR(!this, "Object pointer was null!",);

    if (tempPlot)   this->tempPlot  ->setDuration(duration);
    if (circlePlot) this->circlePlot->setDuration(duration);
    if (squarePlot) this->squarePlot->setDuration(duration);
    if (pressPlot)  this->pressPlot ->setDuration(duration);
}

void PlotManager::addPoints(sf::Vector2f tempPoint, sf::Vector2f circlePoint, sf::Vector2f squarePoint, sf::Vector2f pressPoint) {
    ON_ERROR(!this, "Object pointer was null!",);

    if (tempPlot)   this->tempPlot  ->addPoint(tempPoint);
    if (circlePlot) this->circlePlot->addPoint(circlePoint);
    if (squarePlot) this->squarePlot->addPoint(squarePoint);
    if (pressPlot)  this->pressPlot ->addPoint(pressPoint);
}

Controller::Controller() :
    btnManager(nullptr),
    molManager(nullptr),
    pltManager(nullptr)
    {}

Controller::Controller(UIManager* _btnManager, MolManager* _molManager, PlotManager* _pltManager) :
    btnManager(_btnManager),
    molManager(_molManager),
    pltManager(_pltManager)
    {}

Controller::~Controller() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->btnManager = nullptr;
    this->molManager = nullptr;
    this->pltManager = nullptr;
}

UIManager* Controller::getBtnManager() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->btnManager;
}

MolManager* Controller::getMolManager() {
    ON_ERROR(!this, "Object pointer was null!", nullptr);

    return this->molManager;
}

void Controller::registerClick() {
    ON_ERROR(!this, "Object pointer was null!",);

    unsigned int btnCnt  = this->btnManager->getBtnCnt();
    Button**     buttons = this->btnManager->getButtons();
    sf::Sprite*  btnSpr  = this->btnManager->getSprite();

    if (!buttons || !btnSpr) return;

    for (unsigned int i = 0; i < btnCnt; i++) {
        Button* btn = buttons[i];
        if (btn) {
            sf::Vector2f btnSpritePos = btnSpr->getPosition();

            sf::Vector2i mousePositionTexture = 
                sf::Mouse::getPosition() - sf::Vector2i(btnSpritePos) - sf::Vector2i(0, 2 * FRAME_WIDTH);

            if (btn->isInside(mousePositionTexture)) {
                (btn->getFunc())(*this);
            }
        }
    }    
}

void Controller::update() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->molManager || !this->btnManager || !this->pltManager, "Manager pointer was null!",);

    MolManager* molManager  = this->molManager;
    UIManager*  btnManager  = this->btnManager;
    PlotManager* pltManager = this->pltManager;

    if (!btnManager || !molManager) return;

    molManager->checkCollisions();
    molManager->moveAll        ();
    molManager->resizeAll      ();

    molManager->draw();
    btnManager->draw();
}

void Controller::updatePlot(size_t frameNum, double duration) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!this->molManager || !this->pltManager, "Manager pointer was null!",);

    sf::Vector2f tempPoint    (FRAME_WIDTH + frameNum, pltManager->getTempPlot  ()->getPlane()->getYStart() + pltManager->getTempPlot  ()->getPlane()->getHeight() - molManager->getTemperature()        * TEMP_COEFF   - 2 * FRAME_WIDTH);
    sf::Vector2f circlePoint  (FRAME_WIDTH + frameNum, pltManager->getCirclePlot()->getPlane()->getYStart() + pltManager->getCirclePlot()->getPlane()->getHeight() - molManager->getMolTypeCount(CIRCLE) * CIRCLE_COEFF - 2 * FRAME_WIDTH);
    sf::Vector2f squarePoint  (FRAME_WIDTH + frameNum, pltManager->getSquarePlot()->getPlane()->getYStart() + pltManager->getSquarePlot()->getPlane()->getHeight() - molManager->getMolTypeCount(SQUARE) * SQUARE_COEFF - 2 * FRAME_WIDTH);
    sf::Vector2f pressurePoint(FRAME_WIDTH + frameNum, pltManager->getPressPlot ()->getPlane()->getYStart() + pltManager->getPressPlot ()->getPlane()->getHeight() - molManager->getPressure()           * PRESS_COEFF  - 2 * FRAME_WIDTH);
    
    pltManager->setXAxis(duration);
    pltManager->addPoints(tempPoint, circlePoint, squarePoint, pressurePoint);
    pltManager->draw();
}

bool collideCircles(BaseMolecule* mol1, BaseMolecule* mol2) {
    ON_ERROR(!mol1 || !mol2, "Object pointer was null!", false);

    double centre1x = mol1->getX() + mol1->getSize();
    double centre1y = mol1->getY() + mol1->getSize();
    double centre2x = mol2->getX() + mol2->getSize();
    double centre2y = mol2->getY() + mol2->getSize();

    double radSum = mol1->getSize() + mol2->getSize();

    return (centre1x - centre2x) * (centre1x - centre2x) 
         + (centre1y - centre2y) * (centre1y - centre2y) <= radSum * radSum;
}

bool collideSquareCircle(BaseMolecule* square, BaseMolecule* circle) {
    ON_ERROR(!square || !circle, "Object pointer was null!", false);

    double rectCentreX = square->getX() + square->getSize() / 2;
    double rectCentreY = square->getY() + square->getSize() / 2;
    double circCentreX = circle->getX() + circle->getSize();
    double circCentreY = circle->getY() + circle->getSize();

    double distanceX = abs(rectCentreX - circCentreX);
    double distanceY = abs(rectCentreY - circCentreY);

    if ((distanceX > square->getSize() / 2 + circle->getSize()) ||
        (distanceY > square->getSize() / 2 + circle->getSize())) return false;

    if ((distanceX <= square->getSize() / 2) ||
        (distanceY <= square->getSize() / 2)) return true;

    double distToCorner = 
        2 * (circCentreX - square->getSize() / 2) * (circCentreX - square->getSize() / 2); 

    return distToCorner <= circle->getSize() * circle->getSize();
        
}

void proceedCircles(MolManager& manager, List<BaseMolecule*>* list, long ind1, long ind2) {
    ON_ERROR(!list, "List pointer was null!",);

    CircleMolecule* molecule1 = (CircleMolecule*) (*list)[ind1];
    CircleMolecule* molecule2 = (CircleMolecule*) (*list)[ind2];

    double createX = (molecule1->getX() + molecule2->getX()) / 2;
    double createY = (molecule1->getY() + molecule2->getY()) / 2;

    list->pushBack(new SquareMolecule(createX, createY, molecule1->getWeight() + molecule2->getWeight()));
    
    list->updateElem(NULL, ind1);
    list->updateElem(NULL, ind2);

    delete molecule1;
    delete molecule2;
}

void proceedSquares(MolManager& manager, List<BaseMolecule*>* list, long ind1, long ind2) {
    ON_ERROR(!list, "List pointer was null!",);

    BaseMolecule* molecule1 = (*list)[ind1];
    BaseMolecule* molecule2 = (*list)[ind2];

    unsigned int circleAmount = molecule1->getWeight() + molecule2->getWeight();
    double createCrlcPointX = (molecule1->getX() + molecule2->getX()) / 2;
    double createCrlcPointY = (molecule1->getY() + molecule2->getY()) / 2;
    double diameter = molecule1->getSize() * 2;

    list->updateElem(NULL, ind2);
    list->updateElem(NULL, ind1);

    delete molecule1;
    delete molecule2;

    double angle = 2 * M_PI / circleAmount;
    for (unsigned int i = 0; i < circleAmount; i++) {
        double speedY = cos(angle * i);
        double speedX = sin(angle * i);

        double circleX = createCrlcPointX + diameter * speedX;
        double circleY = createCrlcPointY + diameter * speedY;

        if (circleX <= FRAME_WIDTH) 
            circleX = FRAME_WIDTH;
        if (circleX >= manager.getTexture()->getSize().x - DEFAULT_SIZE)
            circleX  = manager.getTexture()->getSize().x - DEFAULT_SIZE;

        if (circleY <= 2 * FRAME_WIDTH + manager.getPressY()) 
            circleY  = 2 * FRAME_WIDTH + manager.getPressY();
        if (circleY >= manager.getTexture()->getSize().y - DEFAULT_SIZE)
            circleY  = manager.getTexture()->getSize().y - DEFAULT_SIZE;

        manager.addMolecule(CIRCLE, circleX, circleY, speedX, speedY);
    }
}

void proceedCircleSquare(MolManager& manager, List<BaseMolecule*>* list, long circle, long square) {
    BaseMolecule* molecule1 = (*list)[circle];

    list->updateElem(NULL, circle);
    (*list)[square]->addWeight(molecule1->getWeight());

    delete molecule1;
}

void proceedSquareCircle(MolManager& manager, List<BaseMolecule*>* list, long square, long circle) {
    ON_ERROR(!list, "List pointer was null!",);

    BaseMolecule* molecule2 = (*list)[circle];

    list->updateElem(NULL, circle);
    (*list)[square]->addWeight(molecule2->getWeight());

    delete molecule2;
}

bool collideSquares(BaseMolecule* mol1, BaseMolecule* mol2) {
    ON_ERROR(!mol1 || !mol2, "Object pointer was null!", false);

    double mol1CentreX = mol1->getX() + mol1->getSize();
    double mol1CentreY = mol1->getY() + mol1->getSize();
    double mol2CentreX = mol2->getX() + mol2->getSize();
    double mol2CentreY = mol2->getY() + mol2->getSize();

    return (abs(mol1CentreX - mol2CentreX) < mol1->getSize() / 2 + mol2->getSize() / 2) &&
           (abs(mol1CentreY - mol2CentreY) < mol1->getSize() / 2 + mol2->getSize() / 2);
}

void addCircle(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;

    molManager->addMolecule(CIRCLE,
                            2 * DEFAULT_SIZE, 
                            molText->getSize().y - int(DEFAULT_SIZE * 2));
}

void addSquare(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;

    molManager->addMolecule(SQUARE,
                            2 * DEFAULT_SIZE, 
                            molText->getSize().y - int(DEFAULT_SIZE * 2));
}

void pressUp(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    molManager->movePress(UP, PRESS_SHIFT);
}

void pressDown(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    molManager->movePress(DOWN, PRESS_SHIFT);
}

void tempUp(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;      

    molManager->changeTemp(UP, TEMP_SHIFT);
}

void tempDown(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;          
     
    molManager->changeTemp(DOWN, TEMP_SHIFT);
}