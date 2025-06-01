#include "Game.h"
#include "Config.h"
#include "Sprites/Sprite.h"

Game::Game() 
    : screen(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO),
      player(0, 288, &screen), // Posición inicial (x, y)
      skeleton(Enemy::SKELETON, 208, 288, &screen),
      bamboo(Enemy::BAMBOO, 80, 0, &screen),
      reptil(Enemy::REPTIL, 104, 288, &screen),
      llama(Enemy::LLAMA, 0, 120, &screen),
      projectile(&screen) {
    // Inicializar gemas
    gems[0] = Gem(16, 48, 0, &screen);
    gems[1] = Gem(220, 32, 1, &screen);
    gems[2] = Gem(200, 220, 2, &screen);
    gems[3] = Gem(140, 250, 0, &screen);
    gems[4] = Gem(204, 32, 1, &screen);
    gems[5] = Gem(100, 200, 0, &screen);
    
    // Inicializar bloques de agua
    waterBlocks[0] = Water(80, 100, 2, 1, 1, &screen);
    waterBlocks[1] = Water(160, 176, 1, 2, 1, &screen);
    waterBlocks[2] = Water(34, 280, 2, 2, 1, &screen);
    waterBlocks[3] = Water(0, 32, 2, 1, 1, &screen);
    waterBlocks[4] = Water(160, 160, 2, 1, 1, &screen);
}

void Game::init() {
    screen.begin();
    screen.fillScreen(ILI9341_BLACK);
    
    showTitle();
    transitionEffect();
    playMelody();
    fillGreenBackground();
    
    // Configurar pines de entrada
    pinMode(botonDisparo, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(botonUp, INPUT);
    pinMode(botonDown, INPUT);
    pinMode(botonLeft, INPUT);
    pinMode(botonRight, INPUT);

    // Dibujar bloques de agua
        for (int i = 0; i < 5; i++) {
            waterBlocks[i].draw();
        }
        
        // Dibujar gemas
        for (int i = 0; i < 6; i++) {
            gems[i].draw();
        }
}    

void Game::update() {
    // Actualizar estado del jugador
    player.updateInvulnerability();
        // Leer botones de dirección
    if (digitalRead(botonUp) == HIGH) {
        player.move(PLAYER_UP);
    } else if (digitalRead(botonDown) == HIGH) {
        player.move(PLAYER_DOWN);
    } else if (digitalRead(botonRight) == HIGH) {
        player.move(PLAYER_RIGHT);
    } else if (digitalRead(botonLeft) == HIGH) {
        player.move(PLAYER_LEFT);
    } 
    
    // Manejar disparo
    if (digitalRead(botonDisparo) == HIGH && !projectile.isActive()) {
        if (player.getDirection() == PLAYER_RIGHT) {
            projectile.shoot(player.getX() + player.getWidth(), 
                           player.getY() + player.getHeight()/2, 
                           PLAYER_RIGHT);
        } else if (player.getDirection() == PLAYER_LEFT) {
            projectile.shoot(player.getX() - 4, 
                           player.getY() + player.getHeight()/2, 
                           PLAYER_LEFT);
        }
    }
    // Verificar todas las colisiones
    checkCollisions();
    bamboo.update();
    reptil.update();
    skeleton.update();
    llama.update();
    projectile.update();
}

void Game::draw() {
    static unsigned long lastBackgroundUpdate = 0;
static unsigned long lastTopUpdate = 0;
static int count = 3;
static int count2 = 6;
uint8_t frame = (millis() / 20) % 2;
if (millis() - lastBackgroundUpdate > 10) {
    lastBackgroundUpdate = millis();
    screen.drawRGBBitmap(0, 16, fondo[count], 16, 16);
    screen.drawRGBBitmap(16, 16, fondo[count], 16, 16);
    count++;
    if (count == 5) count = 3;
}

if (millis() - lastTopUpdate > 5) {
    lastTopUpdate = millis();
    screen.drawRGBBitmap(0, 0, fondo[count2], 16, 16);
    screen.drawRGBBitmap(16, 0, fondo[count2], 16, 16);
    count2++;
    if (count2 == 10) count2 = 6;
}

    // Dibujar personajes
        player.draw(frame);
        skeleton.draw(frame);
        bamboo.draw(frame);
        reptil.draw(frame);
        llama.draw(frame);
        projectile.draw();
        
        // Dibujar HUD (vidas)
        player.drawHearts();
    
    
}

void Game::checkCollisions() {
    checkPlayerCollisions();
    checkProjectileCollisions();
    checkGemCollisions();
    
    // Verificar condición de derrota
    if (player.getLives() <= 0) {
        handleGameOver();
    }
    
    // Verificar condición de victoria
    bool allGemsCollected = true;
    for (int i = 0; i < 6; i++) {
        if (!gems[i].isCollected()) {
            allGemsCollected = false;
            break;
        }
    }
    if (allGemsCollected) {
        handleGameWin();
    }
}

void Game::checkPlayerCollisions() {
    // Verificar colisión con enemigos
    if (skeleton.checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()) ||
        bamboo.checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()) ||
        reptil.checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()) ||
        llama.checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight())) {
        player.takeDamage();
    }
    
    // Verificar colisión con agua
    for (int i = 0; i < 5; i++) {
        if (waterBlocks[i].checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight())) {
            player.move(PLAYER_UP);
            break;
        }
    }
}

void Game::checkProjectileCollisions() {
    if (!projectile.isActive()) return;
    
    // Colisión con skeleton
    if (skeleton.checkCollision(projectile.getX(), projectile.getY(), 4, 4)) {
        // Borrar skeleton (pintar su área de blanco)
        screen.fillRect(skeleton.getX(), skeleton.getY(), 
                       32,32, ILI9341_WHITE);
        skeleton.setVisible(false);
        screen.fillCircle(projectile.getX(), projectile.getY(), 4, ILI9341_WHITE);
        projectile.setActive(false);

    }
    
    // Colisión con bamboo
    if (bamboo.checkCollision(projectile.getX(), projectile.getY(), 4, 4)) {
        screen.fillRect(bamboo.getX(), bamboo.getY(), 
                       16, 16,  ILI9341_WHITE);
        bamboo.setVisible(false);
        screen.fillCircle(projectile.getX(), projectile.getY(), 4, ILI9341_WHITE);
        projectile.setActive(false);
    }
    
    // Colisión con reptil
    if (reptil.checkCollision(projectile.getX(), projectile.getY(), 4, 4)) {
        screen.fillRect(reptil.getX(), reptil.getY(), 
                       16, 16, ILI9341_WHITE);
        reptil.setVisible(false);
        screen.fillCircle(projectile.getX(), projectile.getY(), 4, ILI9341_WHITE);
        projectile.setActive(false);
    }
    
    // Colisión con llama
    if (llama.checkCollision(projectile.getX(), projectile.getY(), 4, 4)) {
        screen.fillRect(llama.getX(), llama.getY(), 
                       16, 16, ILI9341_WHITE);
        llama.setVisible(false);
        screen.fillCircle(projectile.getX(), projectile.getY(), 4, ILI9341_WHITE);
        projectile.setActive(false);
    }
}

void Game::checkGemCollisions() {
    for (int i = 0; i < 6; i++) {
        if (gems[i].checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight())) {
            // Sonido al recoger gema
            tone(BUZZER_PIN, 500, 100);
        }
    }
}

void Game::handleGameOver() {
    screen.fillScreen(ILI9341_BLACK);
    screen.drawRGBBitmap((240 - 32)/2, 80, Player3, 32, 32);
    
    screen.setTextColor(ILI9341_RED);
    screen.setTextSize(3);
    screen.setCursor(50, 150);
    screen.println("GAME OVER");
    
    playLoseMelody();
    while (true); // Detener el juego
}

void Game::handleGameWin() {
    screen.fillScreen(ILI9341_BLACK);
    
    screen.setTextSize(4);
    screen.setTextColor(ILI9341_YELLOW);
    screen.setCursor(50, 150);
    screen.print("YOU WIN");
    
    playWinMelody();
    while(true); // Detener el juego
}

void Game::showTitle() {
    screen.setTextColor(ILI9341_WHITE);

    // Mostrar "TERRA"
    screen.setTextSize(5);
    int16_t x1, y1;
    uint16_t w, h;
    screen.getTextBounds("TERRA", 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((240 - w) / 2, 90);
    screen.println("TERRA");

    // Mostrar "WORLD"
    screen.setTextSize(4);
    screen.getTextBounds("WORLD", 0, 0, &x1, &y1, &w, &h);
    screen.setCursor((240 - w) / 2, 160);
    screen.println("WORLD");

    delay(2000);
    screen.fillScreen(ILI9341_BLACK);
}

void Game::transitionEffect() {
    for (int y = 0; y < 320; y += 2) {
        screen.drawLine(0, y, 240, y, ILI9341_WHITE);
        delay(5);
    }
    delay(500);
    screen.fillScreen(ILI9341_BLACK);
}

void Game::fillGreenBackground() {
    screen.fillScreen(ILI9341_WHITE);
}

void Game::playMelody() {
    int melody[] = {262, 294, 330, 349, 392, 440, 494, 523}; 
    int duration = 300;
    
    for (int i = 0; i < 8; i++) {
        tone(BUZZER_PIN, melody[i]);
        delay(duration);
        noTone(BUZZER_PIN);
        delay(50);
    }
}

void Game::playWinMelody() {
    tone(BUZZER_PIN, 330); 
    delay(150);
    tone(BUZZER_PIN, 392); 
    delay(150);
    tone(BUZZER_PIN, 659); 
    delay(150);
    tone(BUZZER_PIN, 523); 
    delay(150);
    tone(BUZZER_PIN, 587); 
    delay(150);
    tone(BUZZER_PIN, 784); 
    delay(150);
    noTone(BUZZER_PIN);
}

void Game::playLoseMelody() {
    tone(BUZZER_PIN, 622); 
    delay(300);
    tone(BUZZER_PIN, 587); 
    delay(300);
    tone(BUZZER_PIN, 554); 
    delay(300);

    // Efecto descendente
    for (byte i = 0; i < 10; i++) {
        for (int pitch = -10; pitch <= 10; pitch++) {
            tone(BUZZER_PIN, 523 + pitch);
            delay(5);
        }
    }
    noTone(BUZZER_PIN);
}