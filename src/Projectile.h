#ifndef Projectile_h
#define Projectile_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include <Adafruit_ILI9341.h>

class Projectile {
public:
    Projectile(Adafruit_ILI9341* screen);
    void shoot(int x, int y, uint8_t direction);
    void update();
    void draw();
    bool isActive() const;
    bool checkCollision(int x, int y, int width, int height) const;
    void setActive(bool active);
    int getX() const;
    int getY() const;
private:
    Adafruit_ILI9341* screen;
    bool active = false;
    int x, y;
    uint8_t direction;
    const int radius = 4;
    unsigned long lastUpdate = 0;
    const unsigned long interval = 50;
    const uint8_t UP = 0;
    const uint8_t DOWN = 1;
    const uint8_t RIGHT = 2;
    const uint8_t LEFT = 3;
};

#endif