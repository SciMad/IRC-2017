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
    Serial.println("I am in Dry Run");
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
      
      if(Game::dryPath[game.completedSegments+1] == Vertex::getIndex(game.lastVertex.x, game.lastVertex.y)){
        Serial.println("arrived");
        game.completedSegments++;
        game.xOrient = Vertex::dx(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);  
        game.yOrient = Vertex::dy(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);
      }
    }
  }else {
      Serial.println("I am in Actual Run");
      EEPROM_readAnything(0, game);
      int blockCount = 0, block1, block2, d1,d2, tempBlock, pathLength;
      for (int i=0; i<50; i++){
        if (game.vertex[i].type == BLOCK){
          blockCount++;
          if (blockCount == 1) block1 = i;  
          if (blockCount == 2) block2 = i;
        }
      }
      if (game.findShortest(0,block1) > game.findShortest(0,block2)){
        tempBlock = block1;
        block1 = block2;
        block2 = block1;
      }

      pathLength = game.findShortest(0,block1);
      bot.traverse(game.wetPath, pathLength);
      
      Color color = bot.readBlockColor();
      
      if (color == BLUE){
        // go from current position to transfer zone{EITHER ((0,2) /* to confirm direction of arrow*/ AND then to transfer zone) OR directly to transfer zone }
        // go to other block
        // go to (4,1)
      }else{//color == RED
        // go from current position to (4,1)
        // return to other block
        // go to transfer zone
      }

      
      

      
      
      
      delay(1000);
      exit(0);
  }

  
  
}
