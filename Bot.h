#include "Grid.h"
#include "Game.h"

typedef int MotorDirection;
const MotorDirection FORWARD = 1, BACKWARD = -1, STOP = 0;
int maxRPM = 200;

class Bot{
  public:
  int xOrient, yOrient;
  Vertex lastVertex;
  Color readBlockColor(){ 
    
    // Communicate with RPI and get the color from Serial
    Serial.println("I am Detecting Color");
    return RED;
  };
  void initializeWetRun(){
    VertexType detect;
    xOrient = EAST;
    yOrient = NOCHANGE;
    lastVertex.x = -1;
    lastVertex.y = 0;
  }
  void moveForward(){
    
  }
  void moveForward(int leftRPM, int rightRPM){
    int rightPWM, leftPWM;
    rightPWM = (rightRPM*255/maxRPM);
    leftPWM = (leftRPM*255/maxRPM);
    analogWrite(3, leftPWM);
    analogWrite(9, rightPWM);
    leftMotor(FORWARD);
    rightMotor(FORWARD);
  };

  void stopMoving(){
    leftMotor(STOP);
    rightMotor(STOP);
     
  };
  
  void leftMotor(MotorDirection motorDirection){
    switch (motorDirection){
      case FORWARD:
        digitalWrite(5,HIGH);
        digitalWrite(6,LOW);
      break;
      
      case BACKWARD:
        digitalWrite(6,HIGH);
        digitalWrite(5,LOW);
      break;
      
      case STOP:
        digitalWrite(5,LOW);
        digitalWrite(6,LOW);
      break;
      
      default:
      break;
    }
  };
  void rightMotor(MotorDirection motorDirection){
    switch (motorDirection){
      case FORWARD:
        digitalWrite(7,HIGH);
        digitalWrite(8,LOW);
      break;
      
      case BACKWARD:
        digitalWrite(7,LOW);
        digitalWrite(8, HIGH);
      break;
                                                                    
      case STOP:
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
      break;
      
      default:
      break;
    }
  };
  int getError(){
  int error = 0;
    if (digitalRead(A1) == WHITE) {error = 1;}
    if (digitalRead(A1) == WHITE && digitalRead(A2) == BLACK) error = 2;
    if (digitalRead(A3) == WHITE) error = -1;
    if (digitalRead(A3) == WHITE && digitalRead(A2) == BLACK) error = -2;
    return error;
  }
  void gripBlock(){
    Serial.println("GRP");    //I am gripping
  };

  void fillTransferZone(){
    
  };

  void moveUntil(VertexType vertexType){
    do{
      moveForward();
    }while(nodeDetect() != vertexType);   //Reach (0,0)
  };

  void rotate180(){
    xOrient = -1 * xOrient;
    yOrient = -1 * yOrient;
    Serial.println("R180");
    //Serial.println("New Orientation is:");
    //Serial.println(xOrient);
    //Serial.println(yOrient);
  };

  void moveBackward(){
    
  };

  void moveLeft(){
    
  };
  
  void moveRight(){
    
  };

  void traverse(int* p, int l, VertexType type){
    Serial.println("OrBT" ); //My orientation before TRAVERSE is
    Serial.println(xOrient);
    Serial.println(yOrient);
    Serial.println("PosBT");  //And my position before Traverse is
    Serial.println(lastVertex.x);
    Serial.println(lastVertex.y);
    Serial.println("PII");  //Position Interms of Index:
    Serial.println(p[0]);
    Serial.println("----");   
    
    int prevXOrient = xOrient, prevYOrient  = yOrient;
    
    Serial.println("Len");     //Length of path:
    Serial.println(l);
    int flag = 0;
    if(type >= REDPIT){
      flag = 1;
    }
    for(int i=0; i<l-flag; i++){       //Length of Path is one less than the length of array of vertex
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
    if(type == TRANSFERZONE || type == PHOTOPOINT){
      Serial.println("Rot2Wst");
      rotateTo(WEST, NOCHANGE);
      xOrient = WEST;
      yOrient = NOCHANGE;
    }
  }
  
  VertexType nodeDetect(){
    //if (digitalRead(2) == HIGH) {Serial.println("TZ Detected"); return TRANSFERZONE; }
    //if (digitalRead(3) == HIGH) {Serial.println("Vertex Detected"); return VERTEX; }
    //if (digitalRead(4) == HIGH) {Serial.println("BLOCKBASE Detected"); return BLOCKBASE; }
    if (digitalRead(A1) == WHITE && digitalRead(A2) == WHITE && digitalRead(A3) == WHITE ) { return VERTEX;}
    return PATH;
  };

  void rotateTo(Orientation finalX, Orientation finalY){
    //read bot.xorient, bot.yOirent and use a logic to turn the bot tothe destination
  }

  void fillPit(){
    
  }

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
