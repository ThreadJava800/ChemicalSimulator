#ifndef _GRAPH_h_
#define _GRAPH_h_

#include "../constants.h"
#include "../objects/objects.h"
#include "../../libs/list.h"
#include "../buttons/button.h"
#include "../plots/plot.h"

class MolManager;

typedef bool (*CollideFunc)(BaseMolecule* mol1, BaseMolecule* mol2);
typedef void (*ProceedFunc)(MolManager& manager, List<BaseMolecule*>* list, long ind1, long ind2);

class BaseManager {
protected:
    sf::RenderTexture* texture       = nullptr;
    sf::Sprite*        sprite        = nullptr;
    sf::Sprite*        backgroundImg = nullptr;

public:
    BaseManager();
    BaseManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, sf::Sprite* backgroundImg);

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
    UIManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, Button** buttons, unsigned int btnCnt, sf::Sprite* backgroundImg);

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
    List<BaseMolecule*>* molecules = nullptr;
    double  pressY                 = 0;
    double  temperature            = 273.15;   // kelvins (0 degrees celsium)

public:
    MolManager();
    MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, double _pressY, double _temperature, sf::Sprite* backgroundImg);
    MolManager(sf::RenderTexture* _texture, sf::Sprite* _sprite, List<BaseMolecule*>* _molecules, double _pressY, double _temperature, sf::Sprite* backgroundImg);

    ~MolManager();

    double getTemperature ();
    uint   getMolTypeCount(MoleculeType type);
    double getPressure    ();
    double getPressY      ();

    void draw() override;

    void moveAll        ();
    void resizeAll      ();
    void checkCollisions();
    void tryCollide     (long ind1, long ind2);
    void addMolecule    (MoleculeType type, double x, double y, double vx = 1, double vy = 1);
    void movePress      (MOVE_DIR dir, double shift);
    void changeTemp     (MOVE_DIR dir, double shift);
};

class PlotManager : public BaseManager {
private:
    Plot* tempPlot   = nullptr;
    Plot* circlePlot = nullptr;
    Plot* squarePlot = nullptr;
    Plot* pressPlot  = nullptr;

public:
    PlotManager();
    PlotManager(sf::RenderTexture* _texture, 
                sf::Sprite* _sprite,
                Plot* _tempPlot,
                Plot* _circlePlot,
                Plot* _squarePlot,
                Plot* _pressPlot,
                sf::Sprite* backgroundImg);

    ~PlotManager();

    Plot* getTempPlot();
    Plot* getCirclePlot();
    Plot* getSquarePlot();
    Plot* getPressPlot();

    void draw() override;

    void drawBackground();
    void addPoints(sf::Vector2f tempPoint, 
                   sf::Vector2f circlePoint, 
                   sf::Vector2f squarePoint, 
                   sf::Vector2f pressPoint);
};

class Controller {
private:
    UIManager*   btnManager = nullptr;
    MolManager*  molManager = nullptr;
    PlotManager* pltManager = nullptr; 

public:
    Controller();
    Controller(UIManager* _btnManager, MolManager* _molManager, PlotManager* _pltManager);

    ~Controller();

    UIManager*  getBtnManager();
    MolManager* getMolManager();

    void registerClick();
    void update();
    void updatePlot(size_t frameNum);
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

void proceedCircles     (MolManager& manager, List<BaseMolecule*>* list, long ind1, long ind2);
void proceedSquares     (MolManager& manager, List<BaseMolecule*>* list, long ind1, long ind2);
void proceedCircleSquare(MolManager& manager, List<BaseMolecule*>* list, long circle, long square);
void proceedSquareCircle(MolManager& manager, List<BaseMolecule*>* list, long square, long circle);

#endif