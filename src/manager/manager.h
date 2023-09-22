#ifndef _GRAPH_h_
#define _GRAPH_h_

#include "../constants.h"
#include "../objects/objects.h"
#include "../../LinkedList/list.h"
#include "../buttons/button.h"
#include "../plots/plot.h"

class BaseManager {
protected:
    sf::RenderTexture* texture = nullptr;
    sf::Sprite*        sprite  = nullptr;

public:
    BaseManager();
    BaseManager(sf::RenderTexture* _texture, sf::Sprite* _sprite);

    ~BaseManager();

    sf::RenderTexture* getTexture();
    sf::Sprite*        getSprite ();

    virtual void draw() = 0;
};

class UIManager : public BaseManager {
private:
    Button**     buttons = nullptr;
    unsigned int btnCnt  = 0;

public:
    UIManager();
    UIManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, Button** buttons, unsigned int btnCnt);

    ~UIManager();

    Button**     getButtons();
    unsigned int getBtnCnt();

    void draw() override;

};

enum MOVE_DIR {
    UP,
    DOWN
};

class MolManager : public BaseManager {
private:
    List_t* molecules   = nullptr;
    double  pressY      = 0;
    double  temperature = 273.15;   // kelvins (0 degrees celsium)

public:
    MolManager();
    MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, double _pressY, double _temperature);
    MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, List_t* _molecules, double _pressY, double _temperature);

    ~MolManager();

    void draw() override;

    void moveAll        ();
    void resizeAll      ();
    void checkCollisions();
    void tryCollide     (long ind1, long ind2);
    void addMolecule    (MoleculeType type, double x, double y, double vx = 1, double vy = 1);
    void movePress      (MOVE_DIR dir, double shift);
    void changeTemp     (MOVE_DIR dir, double shift);
};

// class PlotManager : BaseManager {
// private:
    
// };

class Controller {
private:
    UIManager*  btnManager = nullptr;
    MolManager* molManager = nullptr;

public:
    Controller();
    Controller(UIManager* _btnManager, MolManager* _molManager);

    ~Controller();

    UIManager*  getBtnManager();
    MolManager* getMolManager();

    void registerClick();
    void update();
};

void addCircle(Controller& manager);
void addSquare(Controller& manager);
void pressUp  (Controller& manager);
void pressDown(Controller& manager);
void tempUp   (Controller& manager);
void tempDown (Controller& manager);

bool collideCircles     (BaseMolecule* mol1,   BaseMolecule* mol2);
bool collideSquareCircle(BaseMolecule* square, BaseMolecule* circle);
bool collideSquares     (BaseMolecule* mol1,   BaseMolecule* mol2);

#endif