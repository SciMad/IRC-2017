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
  
  delay(1000);
  if (game.mode != WET){
    Serial.println("DrI");     //I am initializing dry run
    game.initializeDryRun();
    Serial.println("CDrI");   //Completed Initialization of Dry Run
  }
}

void loop(){
  Bot bot;
  if (game.mode != WET){
    Serial.println("IDr");  //In Dry RUN
    while(1){
      bot.moveForward();
      if(true || bot.nodeDetect()>PATH){
        game.lastVertex.x += game.xOrient;
        game.lastVertex.y += game.yOrient;
        //Serial.println("\n\nLastVertex:");
        //Serial.println(game.lastVertex.x);
        //Serial.println(game.lastVertex.y);
        
        if (game.lastVertex.x == 7 && game.lastVertex.y == 3){
          //Serial.println("Saved Wet");
          game.simulateDryCompletion();
          game.mode = WET;
          EEPROM_writeAnything(0, game);
          bot.ReadyForWet();
        }
      }
      
      if(Game::dryPath[game.completedSegments+1] == Vertex::getIndex(game.lastVertex.x, game.lastVertex.y)){
        Serial.println("ARV");  //Arrived
        game.completedSegments++;
        game.xOrient = Vertex::dx(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);  
        game.yOrient = Vertex::dy(Game::dryPath[game.completedSegments + 1],Game::dryPath[game.completedSegments]);
      }
    }
  }else {   //game.mode = WET
      Serial.println("WR");   //I am in Wet Run --> Actual Run
      bot.initializeWetRun();
      Color color;
      int blockCount = 0, block1, block2, d1,d2, tempBlock, pathLength;
      int from, to;
      for (int i=0; i<50; i++){
        if (game.vertex[i].type == BLOCK){
          blockCount++;
          if (blockCount == 1) block1 = i;  
          if (blockCount == 2) block2 = i;
        }
      }
            
      pathLength = game.findShortest(0,block1);
      if (game.findShortest(0,block1) > game.findShortest(0,block2)){
        tempBlock = block1;
        block1 = block2;
        block2 = block1;
      }
      
      pathLength = game.findShortest(0,block1); 
      
      if (pathLength == 0){   //Just In Case Blockbase appears at (0,0)
        color = bot.readBlockColor();
        bot.gripBlock();
        game.vertex[Vertex::getIndex(0,0)].type = BLOCKBASE;
        do{
          bot.moveForward();
        }while(bot.nodeDetect() != BLOCKBASE);
        bot.lastVertex.x = 0;
        bot.lastVertex.y = 0;
      }else{    //if pathLength != 0
        do{
          Serial.println("-1~0"); //I have to go (0, 0) from (-1,0)
          bot.moveForward();
        }while(bot.nodeDetect() != VERTEX);   //Reach (0,0)
        bot.lastVertex.x = 0;
        bot.lastVertex.y = 0;
        
        bot.traverse(game.wetPath, pathLength, game.vertex[block1].type);   //Bot traverses from (0,0) to block1 //This should also orient the bot in the direction of the destination
        Serial.println("LVAT"); //Last Vertex After Traversing
        Serial.println(bot.lastVertex.x);
        Serial.println(bot.lastVertex.y);
        Serial.println("LOrB"); //Last Orient Of Bot
        Serial.println(bot.xOrient);
        Serial.println(bot.yOrient);
                
        color = bot.readBlockColor();
        if (color == BLUE){
          if (game.arrow == NORTH){
            to = Vertex::getIndex(0, 4);
          }else{
            to = Vertex::getIndex(0, 1);
          }
        }else{  // color == RED
          to = Vertex::getIndex(1,4);
        }
        //TODO  : It might be necessary to make the VertexType of vertex[block1] to BLOCKBASE (currently BLOCK) otherwise the bot maybot travel from that point
        d1 = game.findShortest(block1, to);
        d2 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);
        //TODO : Resume work from here (Wed Nov 1 2017 01:30:16)
        if (d1 == INF && d2 == INF){// not possible to carry this block, so move set target to another block
          tempBlock = block1;
          block1 = block2;
          block2 = block1;
          Serial.println("NxtB");   // I am moving towards next Block
          bot.traverse(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), block1, game.vertex[block1].type);
          color = bot.readBlockColor();
          if (color == BLUE){
            if (game.arrow == NORTH){
              to = Vertex::getIndex(0, 4);
            }else{
              to = Vertex::getIndex(0, 1);
            }
          }else{  // color == RED
            to = Vertex::getIndex(1,4);
          }
        }// Now it is 100% possible to move the block1
        bot.gripBlock();  //First Block is Gripped
        d1 = game.findShortest(block1, to); d2 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);   //At least one is not infinite
        if (d1<d2){
          Serial.println("ViaBB");    //It's shorter to go via BlockBase
          do{
            Serial.println("Adj2BB"); //I have adjusteed to block base
            bot.moveForward();
          }while(bot.nodeDetect() != BLOCKBASE);
          bot.lastVertex.x = Vertex::getX(block1);
          bot.lastVertex.y = Vertex::getY(block1);
          d1 = game.findShortest(block1, to);
          bot.traverse(game.wetPath, d1, game.vertex[to].type);
        }else{
          Serial.println("ViaLV\nRot-180");    //It's shorter to go via Last Vertex
          bot.rotate180();
          do{
            Serial.println("Adj2LV"); //I have adjusteed to block base
            bot.moveForward();
          }while(bot.nodeDetect() != VERTEX);
          d2 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);
          bot.traverse(game.wetPath, d2, game.vertex[to].type);
        }

        if (game.vertex[to].type == REDPIT){
          bot.fillPit();
          
        }
       
      }
      Serial.println("LV:");
      Serial.println(bot.lastVertex.x);
      Serial.println(bot.lastVertex.y);
      if(game.vertex[to].type == TRANSFERZONE){
        //go Tannai agadi and put slowly
        bot.rotate180();
        //Adjust back to last vertex
        
        to = block2;
        d1 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);
        bot.traverse(game.wetPath, d1, game.vertex[to].type);
        bot.gripBlock();  //Second Block is Gripped
        to = Vertex::getIndex(1, 4); 
        d1 = game.findShortest(block2, to); d2 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);   //At least one is not infinite
        if (d1<d2){
          Serial.println("ViaBB");    //It's shorter to go via BlockBase
          do{
            Serial.println("Adj2BB"); //I have adjusteed to block base
            bot.moveForward();
          }while(bot.nodeDetect() != BLOCKBASE);
          bot.lastVertex.x = Vertex::getX(block2);
          bot.lastVertex.y = Vertex::getY(block2);
          d1 = game.findShortest(block2, to);
          bot.traverse(game.wetPath, d1, game.vertex[to].type);
        }else{
          Serial.println("ViaLV\nRot-180");    //It's shorter to go via Last Vertex
          bot.rotate180();
          do{
            Serial.println("Adj2LV"); //I have adjusteed to block base
            bot.moveForward();
          }while(bot.nodeDetect() != VERTEX && bot.nodeDetect() != BLOCKBASE);
          d2 = game.findShortest(Vertex::getIndex(bot.lastVertex.x, bot.lastVertex.y), to);
          bot.traverse(game.wetPath, d2, game.vertex[to].type);
        }
        bot.fillPit();
        Serial.println("PitFilled");
      }else{
        
      }
      

      
      delay(2000);
      exit(0);

      
  }

  
  
}
