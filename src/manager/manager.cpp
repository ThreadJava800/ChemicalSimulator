#include "manager.h"

BaseManager::BaseManager() :
    texture(nullptr),
    sprite (nullptr)
    {}

BaseManager::BaseManager(sf::RenderTexture* _texture, sf::Sprite* _sprite) :
    texture(_texture),
    sprite (_sprite)
    {}

BaseManager::~BaseManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->sprite  = nullptr;
    this->texture = nullptr;
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

UIManager::UIManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, Button** buttons, unsigned int btnCnt) :
    BaseManager(_texture, _sprite),
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
    BaseManager(),
    molecules  (nullptr),
    pressY     (0),
    temperature(273.15)
    {}

MolManager::MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, double _pressY, double _temperature) : 
    BaseManager(_texture, _sprite),
    pressY     (_pressY),
    temperature(_temperature) 
{
    ON_ERROR(!this, "Object pointer was null!",);

    this->molecules = new List_t;
    listCtor(this->molecules, 1, 1);
}

MolManager::MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, List_t* _molecules, double _pressY, double _temperature) :
    BaseManager(_texture, _sprite),
    molecules  (_molecules),
    pressY     (_pressY),
    temperature(_temperature)
    {}

MolManager::~MolManager() {
    ON_ERROR(!this, "Object pointer was null!",);

    if (!(this->molecules)) return;

    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* value = this->molecules->values[i].value;
        if (value) delete value;
    }

    listDtor(this->molecules);
    delete this->molecules;

    this->molecules = nullptr;
    this->pressY    = 0;
}

void MolManager::moveAll() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* molecule = this->molecules->values[i].value;
        if (molecule) molecule->move(*(this->texture), this->pressY, this->temperature);
    }
}

void MolManager::draw() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);
    ON_ERROR(!this->texture || !this->sprite, "Drawable area was null",);

    texture->clear();

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

    long moleculeCount = this->molecules->size;

    // draw molecules
    for (long i = 0; i <= this->molecules->size; i++) {
        BaseMolecule* molecule = this->molecules->values[i].value;
        if (molecule) molecule->draw(*texture);
    }

    texture->display();
}

void MolManager::checkCollisions() {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    long moleculeCount = this->molecules->size;

    for (long i = 0; i <= moleculeCount; i++) {
        for (long j = i + 1; j <= moleculeCount; j++) {
            tryCollide(i, j);
        }
    }
}

void MolManager::tryCollide(long ind1, long ind2) {
    ON_ERROR(!this, "Object pointer was null!",);
    ON_ERROR(!(this->molecules), "Pointer to list was null!",);

    List_t* listPointer = this->molecules;
    ON_ERROR(ind1 > listPointer->size || ind2 > listPointer->size, "Overflow attempt",);

    BaseMolecule* molecule1 = listPointer->values[ind1].value;
    BaseMolecule* molecule2 = listPointer->values[ind2].value;

    if (!molecule1 || !molecule2) return;

    if (molecule1->getType() == CIRCLE && molecule2->getType() == CIRCLE) {
        if (collideCircles(molecule1, molecule2)) {
            double createX = (molecule1->getX() + molecule2->getX()) / 2;
            double createY = (molecule1->getY() + molecule2->getY()) / 2;

            listPushBack(listPointer, new SquareMolecule(createX, createY, molecule1->getWeight() + molecule2->getWeight()));
            listPointer->values[ind1].value = nullptr;
            listPointer->values[ind2].value = nullptr;

            delete molecule1;
            delete molecule2;
        }
        return;
    }
    if (molecule1->getType() == CIRCLE && molecule2->getType() == SQUARE) {
        if (collideSquareCircle(molecule2, molecule1)) {
                listPointer->values[ind1].value = nullptr;
                listPointer->values[ind2].value->addWeight(molecule1->getWeight());

                delete molecule1;
            }
        return;
    }
    if (molecule1->getType() == SQUARE && molecule2->getType() == CIRCLE) {
        if (collideSquareCircle(molecule1, molecule2)) {
                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value->addWeight(molecule2->getWeight());

                delete molecule2;
            }
        return;
    }
    if (molecule1->getType() == SQUARE && molecule2->getType() == SQUARE) {
        if (collideSquares(molecule1, molecule2)) {
                unsigned int circleAmount = molecule1->getWeight() + molecule2->getWeight();
                double createCrlcPointX = (molecule1->getX() + molecule2->getX()) / 2;
                double createCrlcPointY = (molecule1->getY() + molecule2->getY()) / 2;
                double diameter = molecule1->getSize() * 2;

                listPointer->values[ind2].value = nullptr;
                listPointer->values[ind1].value = nullptr;

                delete molecule1;
                delete molecule2;

                double angle = 2 * M_PI / circleAmount;
                for (unsigned int i = 0; i < circleAmount; i++) {
                    double speedY = cos(angle * i);
                    double speedX = sin(angle * i);

                    double circleX = createCrlcPointX + diameter * speedX;
                    double circleY = createCrlcPointY + diameter * speedY;

                    addMolecule(CIRCLE, circleX, circleY, speedX, speedY);
                }
            }
        return;
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

    listPushBack(this->molecules, molecule);
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
}

void MolManager::changeTemp(MOVE_DIR dir, double shift) {
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

    this->temperature += shift;
}

Controller::Controller() :
    btnManager(nullptr),
    molManager(nullptr)
    {}

Controller::Controller(UIManager* _btnManager, MolManager* _molManager) :
    btnManager(_btnManager),
    molManager(_molManager)
    {}

Controller::~Controller() {
    ON_ERROR(!this, "Object pointer was null!",);

    this->btnManager = nullptr;
    this->molManager = nullptr;
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

    MolManager* molManager = this->molManager;
    UIManager*  btnManager = this->btnManager;

    if (!btnManager || !molManager) return;

    molManager->checkCollisions();
    molManager->moveAll        ();

    molManager->draw();
    btnManager->draw();
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
                            rand() % (molText->getSize().x - int(DEFAULT_SIZE * 2)), 
                            rand() % (molText->getSize().y - int(DEFAULT_SIZE * 2)));
}

void addSquare(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;

    molManager->addMolecule(SQUARE,
                            rand() % (molText->getSize().x - int(DEFAULT_SIZE * 2)), 
                            rand() % (molText->getSize().y - int(DEFAULT_SIZE * 2)));
}

void pressUp(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;

    molManager->movePress(UP, PRESS_SHIFT);
}

void pressDown(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;

    molManager->movePress(DOWN, PRESS_SHIFT);
}

void tempUp(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;     

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;      

    molManager->changeTemp(UP, TEMP_SHIFT);
}

void tempDown(Controller& manager) {
    MolManager* molManager = manager.getMolManager();
    if (!molManager) return;     

    sf::RenderTexture* molText = molManager->getTexture();
    if (!molText) return;      
     
    molManager->changeTemp(DOWN, TEMP_SHIFT);
}