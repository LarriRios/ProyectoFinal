#ifndef Enemy_h
#define Enemy_h

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include <Adafruit_ILI9341.h>

class Enemy {
public:
    enum EnemyType { SKELETON, BAMBOO, REPTIL, LLAMA };
    Enemy(EnemyType type, int x, int y, Adafruit_ILI9341* screen);
    void update();
    void draw(uint8_t frame);
    void setVisible(bool visible);
    bool checkCollision(int objX, int objY, int objWidth, int objHeight) const;
    int getX() const { return x; }
    int getY() const { return y; }
private:
    EnemyType type;
    int x, y;
    int direction;
    unsigned long speed;
    int topLimit, bottomLimit, leftLimit, rightLimit;
    bool visible = true;
    Adafruit_ILI9341* screen;
    unsigned long lastMoveTime = 0;
    
    void moveSkeleton();
    void moveBamboo();
    void moveReptil();
    void moveLlama();
    void clearPreviousPosition();
    int prevX, prevY;
};

#endif