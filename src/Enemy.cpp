#include "Enemy.h"
#include "Sprites/Skeleton.h"
#include "Sprites/Bamboo.h"
#include "Sprites/Reptil.h"
#include "Sprites/Llama.h"

Enemy::Enemy(EnemyType type, int x, int y, Adafruit_ILI9341* screen) 
    : type(type), x(x), y(y), screen(screen) {
    switch(type) {
        case SKELETON:
            direction = -1;
            speed = 20;
            topLimit = 240; // YMAX - 80
            bottomLimit = 288; // YMAX - 32
            break;
        case BAMBOO:
            direction = 1;
            speed = 20;
            topLimit = 0;
            bottomLimit = 80;
            break;
        case REPTIL:
            direction = -1;
            speed = 5;
            topLimit = 240; // YMAX - 80
            bottomLimit = 304; // YMAX - 16
            break;
        case LLAMA:
            direction = 1;
            speed = 20;
            leftLimit = 0;
            rightLimit = 120; // XMAX / 2
            break;
    }
}

void Enemy::update() {
    if (!visible) return;
    
    unsigned long currentTime = millis();
    if (currentTime - lastMoveTime >= speed) {
        lastMoveTime = currentTime;
        
        switch(type) {
            case SKELETON: moveSkeleton(); break;
            case BAMBOO: moveBamboo(); break;
            case REPTIL: moveReptil(); break;
            case LLAMA: moveLlama(); break;
        }
    }
}
void Enemy::clearPreviousPosition() {
    if (!visible) return;
    
    int clearWidth = (type == SKELETON) ? 32 : 16;
    int clearHeight = (type == SKELETON) ? 32 : 16;
    
    // Limpiar solo si la posición cambió
    if (x != prevX || y != prevY) {
        screen->fillRect(prevX, prevY, clearWidth, clearHeight, ILI9341_WHITE);
    }
}
void Enemy::draw(uint8_t frame) {
    if (!visible) return;
        clearPreviousPosition();
    switch(type) {
        case SKELETON: {
            GFXcanvas16 canvas(32, 32);
            canvas.fillScreen(0);
            canvas.drawRGBBitmap(0, 0, Skeleton[frame], 32, 32);
            screen->drawRGBBitmap(x, y, (uint16_t*)canvas.getBuffer(), 32, 32);
            break;
        }
        case BAMBOO: {
            GFXcanvas16 canvas1(16, 16);
            canvas1.fillScreen(0);
            canvas1.drawRGBBitmap(0, 0, Bamboo[frame], 16, 16);
            screen->drawRGBBitmap(x, y, (uint16_t*)canvas1.getBuffer(), 16, 16);
            break;
        }
        case REPTIL: {
            GFXcanvas16 canvas2(16, 16);
            canvas2.fillScreen(0);
            canvas2.drawRGBBitmap(0, 0, Reptil[frame], 16, 16);
            screen->drawRGBBitmap(x, y, (uint16_t*)canvas2.getBuffer(), 16, 16);
            break;
        }
        case LLAMA: {
            GFXcanvas16 canvas3(16, 16);
            canvas3.fillScreen(0);
            if (direction == -1) {
                canvas3.drawRGBBitmap(0, 0, Llama[0], 16, 16);
            } else {
                canvas3.drawRGBBitmap(0, 0, Llama[2], 16, 16);
            }
            screen->drawRGBBitmap(x, y, (uint16_t*)canvas3.getBuffer(), 16, 16);
            break;
        }
    }
    prevX = x;
    prevY = y;
}

bool Enemy::checkCollision(int objX, int objY, int objWidth, int objHeight) const {
    if (!visible) return false;
    
    int enemyWidth = (type == SKELETON) ? 32 : 16;
    int enemyHeight = (type == SKELETON) ? 32 : 16;
    
    return (x < objX + objWidth &&
            x + enemyWidth > objX &&
            y < objY + objHeight &&
            y + enemyHeight > objY);
}

// Movement methods for each enemy type
void Enemy::moveSkeleton() {
    y += direction * 2;
    if (y <= topLimit) {
        y = topLimit;
        direction = 1;
    } else if (y >= bottomLimit) {
        y = bottomLimit;
        direction = -1;
    }
}

void Enemy::moveBamboo() {
    y += direction * 2;
    if (y <= topLimit) {
        y = topLimit;
        direction = 1;
    } else if (y >= bottomLimit) {
        y = bottomLimit;
        direction = -1;
    }
}

void Enemy::moveReptil() {
    y += direction * 2;
    if (y <= topLimit) {
        y = topLimit;
        direction = 1;
    } else if (y >= bottomLimit) {
        y = bottomLimit;
        direction = -1;
    }
}

void Enemy::moveLlama() {
    x += direction * 4;
    if (x >= rightLimit - 32) {
        direction = -1;
    } else if (x <= leftLimit) {
        direction = 1;
    }
}

void Enemy::setVisible(bool visible) {
    this->visible = visible;
}
