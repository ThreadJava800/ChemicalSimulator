#ifndef _CONST_h_
#define _CONST_h_

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>

// molecules constants
const double       DEFAULT_SIZE   = 10;
const int          FRAME_WIDTH    = 10;
const unsigned int DEFAULT_WEIGHT = 1;
const sf::Color    CIRCLE_COL     = sf::Color::Blue;
const sf::Color    SQUARE_COL     = sf::Color::Red;
const unsigned int START_MOL_CNT  = 25;
const double       TEMP_SHIFT     = 10;
const double       PRESS_SHIFT    = 10;
const double       START_IMPULSE  = 0.00001;
const double       START_VELOCITY = sqrt(START_IMPULSE);
const double       PRS_MAX_SHIFT  = 250;
const double       MY_BOLTZMANN   = 1e6;

// button constants
const unsigned int BUTTON_CNT     = 6;

//plot
const float     UNIT_POINT_RAD         = 8;
const double    ARROW_SIZE_COEF        = 0.1;
const sf::Color DEFAULT_LINE_COLOR     = sf::Color::White;
const sf::Color DEFAULT_UNIT_POINT_COL = sf::Color::Red;
const sf::Color AXIS_COLOR             = sf::Color::Red;
const size_t    FRAME_DELAY            = 10;
const float     TEMP_COEFF             = 0.3;
const float     CIRCLE_COEFF           = 4;
const float     SQUARE_COEFF           = 20;
const float     PRESS_COEFF            = 0.02;
const int       MAX_UNIT_LEN           = 15;

// files
const static char* DEFAULT_FONT = "assets/Arial.ttf";
const static char* PLOT_FONT    = "assets/Simple Story.ttf";
const static char* PLOT_BACK    = "assets/plot.jpg";
const static char* BTN_BACK     = "assets/btnBack.jpg";
const static char* MOL_BACK     = "assets/molBack.jpg";
const static char* ADD_CRCL     = "assets/addCircle.png";
const static char* ADD_SQUARE   = "assets/addSquare.png";
const static char* PRESS_DOWN   = "assets/pressDown.png";
const static char* PRESS_UP     = "assets/pressUp.png";
const static char* TEMP_DOWN    = "assets/tempDown.png";
const static char* TEMP_UP      = "assets/tempUp.png";

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif