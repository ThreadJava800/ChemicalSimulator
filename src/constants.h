#ifndef _CONST_h_
#define _CONST_h_

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

const double       DEFAULT_SIZE   = 10;
const int          FRAME_WIDTH    = 10;
const unsigned int DEFAULT_WEIGHT = 1;
const sf::Color    CIRCLE_COL     = sf::Color::Blue;
const sf::Color    SQUARE_COL     = sf::Color::Red;
const static char*        DEFAULT_FONT   = "assets/Arial.ttf";

#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif