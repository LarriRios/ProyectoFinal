#include "Player.h"
#include "Sprites/Sprite.h"

Player1::Player1(int x, int y, Adafruit_ILI9341* screen) 
    : x(x), y(y), screen(screen), canvas(32, 32) {
    currentDirection = PLAYER_RIGHT;
    lives = 3;
    invulnerable = false;
    canvas.fillScreen(0);
    canvas.drawRGBBitmap(0, 0, Player[0], 32, 32);
    screen->drawRGBBitmap(x, y, (uint16_t*)canvas.getBuffer(), 32, 32);
}

void Player1::move(uint8_t direction) {
    const int delta = 6;
    if (direction == PLAYER_LEFT || direction == PLAYER_RIGHT) {
        currentDirection = direction;
    } 
    
    screen->fillRect(x, y, width, height, ILI9341_WHITE);

    int newX = x;
    int newY = y;
    
    switch(direction) {
        case PLAYER_UP: newY = y - delta; break;
        case PLAYER_DOWN: newY = y + delta; break;
        case PLAYER_RIGHT: newX = x + delta; break;
        case PLAYER_LEFT: newX = x - delta; break;
    }
    
    // Verificar límites de pantalla
    if (newX < 0) newX = 0;
    if (newX > screen->width() - width) newX = screen->width() - width;
    if (newY < 0) newY = 0;
    if (newY > screen->height() - height) newY = screen->height() - height;
    
    x = newX;
    y = newY;
}

void Player1::draw(uint8_t frame) {
    
    canvas.fillScreen(0);
    uint8_t animStep;
    if (currentDirection == PLAYER_RIGHT) {
        animStep = frame;           // 0 o 1
    } else if (currentDirection == PLAYER_LEFT) {
        animStep = 2 + frame;       // 2 o 3
    } 
    canvas.drawRGBBitmap(0, 0, Player[animStep], 32, 32);
    screen->drawRGBBitmap(x, y, (uint16_t*)canvas.getBuffer(), 32, 32);

}

void Player1::takeDamage() {
    if (!invulnerable && lives > 0) {
        lives--;
        invulnerable = true;
        invulnerabilityTime = millis();
        tone(BUZZER_PIN, 300, 200); // Sonido al recibir daño
    }
}

void Player1::updateInvulnerability() {
    if (invulnerable && (millis() - invulnerabilityTime >= INVULNERABILITY_DURATION)) {
        invulnerable = false;
    }
}

void Player1::drawHearts() const {
    for (int i = 0; i < 3; i++) {
        int heartType = (i < lives) ? 1 : 0; // 1 = lleno, 0 = vacío
        int heartX = screen->width() - (i + 1) * (16 + 2); // Espaciado entre corazones
        screen->drawRGBBitmap(heartX, 2, Heart[heartType], 16, 16);
    }
}

// Getters
int Player1::getX() const { return x; }
int Player1::getY() const { return y; }
int Player1::getWidth() const { return width; }
int Player1::getHeight() const { return height; }
uint8_t Player1::getDirection() const { return currentDirection; }
bool Player1::isInvulnerable() const { return invulnerable; }
int Player1::getLives() const { return lives; }