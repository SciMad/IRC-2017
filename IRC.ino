#include "IRC.h"
#include "Bot.h"
#include "Game.h"


Game game;// end of Game class;

void setup(){
  Serial.begin(9600);
  game.loadFromEEPROM();
  if (game.mode == DRY){
    game.initializeVertex();
  }
  
}

void loop(){
  delay(2000);
  Bot bot;
  if (game.mode == DRY){
    while(1){
      bot.moveForward();
      if(true || bot.nodeDetect()>PATH){
       game.lastVertex.x += game.xOrient;
       game.lastVertex.y += game.yOrient;
       if (game.lastVertex.x == 7 && game.lastVertex.y == 3){
        game.mode = WET;
        //bot.ReadyForWet();
       }
      }
      Serial.println("LastVertex:");
      Serial.println(game.lastVertex.x);
      Serial.println(game.lastVertex.y);
      
      if(Game::dryPath[game.completedSegments+1]==Vertex::getIndex(game.lastVertex.x, game.lastVertex.y)){
        Serial.println("\n\nArrived");
        game.completedSegments++;
        game.xOrient = Vertex::dx(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);  
        game.yOrient = Vertex::dy(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);
         
         Serial.println("\n\nNew Orientation");
         Serial.println(game.xOrient);
         Serial.println(game.yOrient);
         
      }
    }
  }else{
    
  }

  delay(2000);
  exit(0);
  
}
