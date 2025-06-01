#ifndef Player_h
#define Player_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Sprites/Sprite.h"
#include "Sprites/Heart.h"
#include <Adafruit_ILI9341.h>
#include "Config.h"


class Player1 {
public:
    Player1(int x, int y, Adafruit_ILI9341* screen);
    void move(uint8_t direction);
    void draw(uint8_t frame);
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    uint8_t getDirection() const;
    bool isInvulnerable() const;
    void takeDamage();
    void updateInvulnerability();
    int getLives() const;
    void drawHearts() const;
    
private:
    int x, y;
    const int width = 32;
    const int height = 32;
    uint8_t currentDirection;
    int lives = 3;
    bool invulnerable = false;
    unsigned long invulnerabilityTime = 0;
    const unsigned long INVULNERABILITY_DURATION = 3000;
    Adafruit_ILI9341* screen;
    GFXcanvas16 canvas;
};

#endif