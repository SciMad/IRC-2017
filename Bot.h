#include "Grid.h"
#include "Game.h"

class Bot{
  public:
  int xOrient, yOrient;
  Vertex lastVertex;
  Color readBlockColor(){
    // Communicate with RPI and get the color from Serial
    Serial.println("I am Detecting Madhav Humagain २२. आफूले लेखेका स्टाटस् जति चैँ सबै सेयर गर्नुहुन अनुरोध नगर्ने।Color");
    return BLUE;
  };
  void initializeWetRun(){
    VertexType detect;
    xOrient = EAST;
    yOrient = NOCHANGE;
    lastVertex.x = -1;
    lastVertex.y = 0;
  }
  
  void moveForward(){
    Serial.println("MOVF");   //I am moving Forward
  };

  void gripBlock(){
    Serial.println("GRP");    //I am gripping
  }

  void rotate180(){
    xOrient = -1 * xOrient;
    yOrient = -1 * yOrient;
    //Serial.println("Rotated 180");
    //Serial.println("New Orientation is:");
    //Serial.println(xOrient);
    //Serial.println(yOrient);
  }

  void moveBackward(){
    
  };

  void moveLeft(){
    
  };
  
  void moveRight(){
    
  };

  void traverse(int* p, int l){
    Serial.println("OrBT" ); //My orientation before TRAVERSE is
    Serial.println(xOrient);
    Serial.println(yOrient);
    Serial.println("PosBT");  //And my position before Traverse is
    Serial.println(lastVertex.x);
    Serial.println(lastVertex.y);
    Serial.println("PII");  //Position Interms of Index:
    Serial.println(p[0]);
    Serial.println("----");   
    
    int prevXOrient = xOrient, prevYOrient = yOrient;
    
    Serial.println("Len");     //Length of path:
    Serial.println(l);
    for(int i=0; i<l-1; i++){       //Length of Path is one less than the length of array of vertex
      Serial.println("I am at ");
      Serial.println(p[i]);
      Serial.println("Ort" );       //My orientation is
      Serial.println(xOrient);
      Serial.println(yOrient);
      
      prevXOrient = xOrient;
      prevYOrient = yOrient;
      
      xOrient = Vertex::dx(p[i+1],p[i]);  
      yOrient = Vertex::dy(p[i+1],p[i]);
      if(xOrient != prevXOrient || yOrient != prevYOrient){
        Serial.println("COR2");   //Now I need to change my orientation to:
        Serial.println(xOrient);
        Serial.println(yOrient);
        Serial.println("ROT");    //I am rotating
        //Write Function to rotate the bot to required orientation
        delay(1000);
        Serial.println("ROTC"); //Rotation Completed:
      }
      
      do{
        moveForward();
      }while(nodeDetect()==PATH);
      
      lastVertex.x += xOrient;
      lastVertex.y += yOrient;
    }
    Serial.println("TRVC");   //Traverse completed

    //Now to direct the bot towards Destination vertex
    prevXOrient = xOrient;
    prevYOrient = yOrient;
    Serial.println("Dr2D");   //Direction to destination
    xOrient = Vertex::dx(p[l], p[l-1]);
    yOrient = Vertex::dy(p[l],p[l-1]);
    if(xOrient != prevXOrient || yOrient != prevYOrient){
      Serial.println("COR2");   //Now I need to change my orientation to:
      Serial.println(xOrient);
      Serial.println(yOrient);
      Serial.println("ROT");    //I am rotating
      //Write Function to rotate the bot to required orientation
      Serial.println("ROTC & Dr2D"); //Rotation Completed:
    }    
  }
  

  VertexType nodeDetect(){
    return VERTEX; 
    if (digitalRead(A0==HIGH)){
      return VERTEX;
    }else{
      return PATH;
    }
  };

  void ReadyForWet(){
    while(1){
      digitalWrite(13,HIGH);
      delay(300);
      digitalWrite(13,LOW);
      delay(300);
    }
      
  };
  
  int ReadSensor(){
    
  };
};
