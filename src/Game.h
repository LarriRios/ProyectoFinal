#ifndef Game_h
#define Game_h

#include "Player.h"
#include "Enemy.h"
#include "Gem.h"
#include "Water.h"
#include "Projectile.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

class Game {
public:
    Game();
    void init();
    void update();
    void draw();
    
private:

    Adafruit_ILI9341 screen;
    Player1 player;
    Enemy skeleton;
    Enemy bamboo;
    Enemy reptil;
    Enemy llama;
    Projectile projectile;
    Gem gems[6];
    Water waterBlocks[5];
    
    void showTitle();
    void transitionEffect();
    void fillGreenBackground();
    void checkCollisions();
    void playMelody();
    void playWinMelody();
    void playLoseMelody();
    void checkPlayerCollisions();
    void checkProjectileCollisions();
    void checkGemCollisions();
    void handleGameOver();
    void handleGameWin();

};

#endif

