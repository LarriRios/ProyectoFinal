#ifndef Gem_h
#define Gem_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Sprites/Gemas.h"
#include <Adafruit_ILI9341.h>

class Gem {
public:
    Gem() {}
    Gem(int x, int y, int type, Adafruit_ILI9341* screen);
    void draw();
    bool isCollected() const;
    bool checkCollision(int playerX, int playerY, int playerWidth, int playerHeight);
    
private:
    int x, y;
    int type; // 0-5 para diferentes tipos de gemas
    bool collected = false;
    Adafruit_ILI9341* screen;
};

#endif