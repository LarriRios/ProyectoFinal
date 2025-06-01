#include "Game.h"
#include "Config.h"


Game game; 

void setup() {
  game.init();
}

void loop() {
  
  game.update();
  
  game.draw();
  
}