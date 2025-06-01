#include "Water.h"

Water::Water(int x, int y, int width, int height, int frame, Adafruit_ILI9341* screen)
    : x(x), y(y), width(width), height(height), frame(frame), screen(screen) {}

void Water::draw() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            screen->drawRGBBitmap(x + i*16, y + j*16, fondo[frame], 16, 16);
        }
    }
}

bool Water::checkCollision(int playerX, int playerY, int playerWidth, int playerHeight) const {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int blockX = x + i * 16;
            int blockY = y + j * 16;
            
            if (playerX < blockX + 16 &&
                playerX + playerWidth > blockX &&
                playerY < blockY + 16 &&
                playerY + playerHeight > blockY) {
                return true;
            }
        }
    }
    return false;
}