#ifndef WATER_H
#define WATER_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Sprites/fondo.h"
#include <Adafruit_ILI9341.h>

class Water {
public:
    Water() {}
    Water(int x, int y, int width, int height, int frame, Adafruit_ILI9341* screen);
    void draw();
    bool checkCollision(int playerX, int playerY, int playerWidth, int playerHeight) const;
    
private:
    int x, y;
    int width, height; // En bloques de 16x16
    int frame;
    Adafruit_ILI9341* screen;
};

#endif