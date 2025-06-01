#include "Gem.h"

Gem::Gem(int x, int y, int type, Adafruit_ILI9341* screen) 
    : x(x), y(y), type(type), screen(screen) {}

void Gem::draw() {
    if (!collected) {
        screen->drawRGBBitmap(x, y, Gemas[type % 3], 16, 16);
    }
}

bool Gem::checkCollision(int playerX, int playerY, int playerWidth, int playerHeight) {
    if (collected) return false;
    
    if (playerX < x + 16 && 
        playerX + playerWidth > x && 
        playerY < y + 16 && 
        playerY + playerHeight > y) {
        collected = true;
        screen->fillRect(x, y, 16, 16, ILI9341_WHITE);
        return true;
    }
    return false;
}

bool Gem::isCollected() const {
    return collected;
}
