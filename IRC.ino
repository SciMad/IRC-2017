#include "Grid.h"
#include "Bot.h"
#include "Game.h"


Game game;// end of Game class;

void setup(){  
  Serial.begin(9600);
  while(!Serial){
  }
  //clearEEPROM();
  //exit(0);

  EEPROM_readAnything(0, game);
  
  delay(2000);
  if (game.mode != WET){
    Serial.println("I am initializing dry run");
    game.initializeDryRun();
    Serial.println("Completed Initialization");
  }
}

void loop(){
  Bot bot;
  if (game.mode != WET){
    Serial.println("I am still dry");
    while(1){
      bot.moveForward();
      if(true || bot.nodeDetect()>PATH){
        game.lastVertex.x += game.xOrient;
        game.lastVertex.y += game.yOrient;
        Serial.println("\n\nLastVertex:");
        Serial.println(game.lastVertex.x);
        Serial.println(game.lastVertex.y);
        
        if (game.lastVertex.x == 7 && game.lastVertex.y == 3){
          Serial.println("Saved Wet");
          game.simulateDryCompletion();
          game.mode = WET;
          EEPROM_writeAnything(0, game);
          bot.ReadyForWet();
        }
      }
      
      if(Game::dryPath[game.completedSegments+1]==Vertex::getIndex(game.lastVertex.x, game.lastVertex.y)){
        Serial.println("arrived");
        game.completedSegments++;
        game.xOrient = Vertex::dx(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);  
        game.yOrient = Vertex::dy(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);
      }
    }
  }else {
    while(1){
      Serial.println("I am wet");
      EEPROM_readAnything(0, game);
      game.findShortest(39,36);
      delay(1000);
      exit(0);
    }
  }

  
  
}
