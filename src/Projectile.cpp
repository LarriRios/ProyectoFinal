#include "Projectile.h"

Projectile::Projectile(Adafruit_ILI9341* screen) : screen(screen) {}

void Projectile::shoot(int x, int y, uint8_t direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    active = true;
}

void Projectile::update() {
    if (!active) return;
    
    if (millis() - lastUpdate >= interval) {
        lastUpdate = millis();
        
        // Borrar proyectil anterior
        screen->fillCircle(x, y, radius, ILI9341_WHITE);
        
        if (direction == RIGHT) {
            x += 8;
            if (x > 240) active = false;
        } else if (direction == LEFT) {
            x -= 8;
            if (x < 0) active = false;
        }
    }
}

void Projectile::draw() {
    if (active) {
        screen->fillCircle(x, y, radius, ILI9341_RED);
    }
}

bool Projectile::checkCollision(int objX, int objY, int objWidth, int objHeight) const {
    if (!active) return false;
    
    // Verificar colisión con un rectángulo
    int closestX = constrain(x, objX, objX + objWidth);
    int closestY = constrain(y, objY, objY + objHeight);
    
    int distanceX = x - closestX;
    int distanceY = y - closestY;
    
    return (distanceX * distanceX + distanceY * distanceY) <= (radius * radius);
}

bool Projectile::isActive() const {
    return active;
}

void Projectile::setActive(bool state) {
    active = state;
}

int Projectile::getX() const {
    return x;
}

int Projectile::getY() const {
    return y;
}
