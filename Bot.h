#include "Grid.h"
#include "Game.h"

typedef int MotorDirection;
const MotorDirection FORWARD = 1, BACKWARD = -1, STOP = 0;
int maxRPM = 200;
class Sensor{
  public:
  static int color(int sensorNum){
    int analogValue = 800;
    switch (sensorNum){
      case -5:
        analogValue = analogRead(A2);
        break;
      case -4:
        analogValue = analogRead(A1);
        break;
      case -3:
        analogValue = analogRead(A0);
        break;
      case -2:
        analogValue = analogRead(A3);
        break;
      case -1:
        analogValue = analogRead(A4);
        break;
      case 0:
        analogValue = analogRead(A5);
        break;
      case 1:
        analogValue = analogRead(A6);
        break;
      case 2:
        analogValue = analogRead(A7);
        break;
      case 3:
        analogValue = analogRead(A8);
        break;
      case 4:
        analogValue = analogRead(A9);
        break;
      case 5:
        analogValue = analogRead(A10);
        break;
      default:
        break;
    }
    if (analogValue < 100) return WHITE; else return BLACK;
  }
};

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
    leftMotor(FORWARD, leftRPM);
    rightMotor(FORWARD, rightRPM);
  };

  void moveBackward(int leftRPM, int rightRPM){
    leftMotor(BACKWARD, leftRPM);
    rightMotor(BACKWARD, rightRPM);
  };

  void stopMoving(){
    leftMotor(STOP, 0);
    rightMotor(STOP, 0);
  };
  
  void leftMotor(MotorDirection motorDirection, int RPM){
    float PWM = (RPM*255/maxRPM);
    analogWrite(3, PWM);
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
  void rightMotor(MotorDirection motorDirection, int RPM){
    float PWM = (RPM*255/maxRPM);
    
    analogWrite(9, PWM);
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
  int getError(){       // For 5 Linear Sensors
    int error = 0;
    if (digitalRead(A1) == WHITE) {error = 1;}
    if (digitalRead(A1) == WHITE && digitalRead(A2) == BLACK) error = 2;
    if (digitalRead(A0) == WHITE && digitalRead(A2) == BLACK) error = 3;
    
    if (digitalRead(A3) == WHITE) error = -1;
    if (digitalRead(A3) == WHITE && digitalRead(A2) == BLACK) error = -2;
    if (digitalRead(A4) == WHITE && digitalRead(A2) == BLACK) error = -3;
    return error;
  }

  int getErr(){       // For 11 Linear Sensors
    int error = 0;
    //Positive Errors When Bot Deviates Right
    if (Sensor::color(-1) == WHITE) {error = 0;}
    if (Sensor::color(1) == BLACK) {error = 1;}    
    if (Sensor::color(-2) == WHITE) {error = 2;}
    if (Sensor::color(-3) == WHITE ||  Sensor::color(0)== BLACK) {error = 3;}
    if (Sensor::color(-4) == WHITE &&  Sensor::color(0)== BLACK) {error = 4;}

    //Negative Errors When Bot Deviates Left
    if (Sensor::color(1) == WHITE) {error = 0;}
    if (Sensor::color(-1) == BLACK) {error = -1;}    
    if (Sensor::color(2) == WHITE) {error = -2;}
    if (Sensor::color(3) == WHITE ||  Sensor::color(0)== BLACK) {error = -3;}
    if (Sensor::color(4) == WHITE &&  Sensor::color(0)== BLACK) {error = -4;}

    //Error For Blockbase's Path
    
    if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == BLACK) {error = 0;}
    
    if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == WHITE) {error = 1;}
    if (Sensor::color(-1) == WHITE && Sensor::color(0) == BLACK && Sensor::color(-1) == BLACK) {error = -1;}

    //TODO: Work on Higher errors if bot deviates

    
    return error;
  }

  void gripBlock(){
    Serial.println("GRP");    //I am gripping
  };

  void fillTransferZone(){
    
  };

  void moveUntil(VertexType vertexType, MotorDirection motorDirection = FORWARD){
    float RPM = 100, rightRPM, leftRPM;
    int error = getError(), previousError = 0, difference = 0;
    int Kp = 16, Kd = 0;
    while(1){   
      previousError = error;
      error = getError();
      difference = previousError - error;
      rightRPM = (RPM + Kp * error - Kd * difference); leftRPM = (RPM - Kp * error + Kd*difference);
      if (motorDirection == FORWARD) moveForward(leftRPM, rightRPM); else moveBackward(leftRPM, rightRPM);
      if (error > 0) leftRPM /= 2;
      if (error < 0) rightRPM /= 2;
      if (digitalRead(A5) == LOW){
        break;
      }
    }
    //do{
    //  moveForward();
    //}while(nodeDetect() != vertexType);   //Reach (0,0)
  };

  void rotate180(){
    xOrient = -1 * xOrient;
    yOrient = -1 * yOrient;
    Serial.println("R180");
    //Serial.println("New Orientation is:");
    //Serial.println(xOrient);
    //Serial.println(yOrient);
  };
  void beepbeep(int interval=100){
    digitalWrite(13, HIGH);
    delay(interval);
    digitalWrite(13, LOW);
    delay(interval);
    digitalWrite(13, HIGH);
    delay(interval);
    digitalWrite(13, LOW);
  }
  
  void beep(int interval=100){
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
  }

  void moveBackward(){
    
  };

  void moveLeft(){
    int error = 3;
    int rotateRPM = 60, RPM;
    //int Kp = 25;
    int flag[5] = {0, 0, 0, 0, 0};
    RPM = rotateRPM;
    
    leftMotor(BACKWARD, RPM);
    rightMotor(FORWARD, RPM);  
    delay(300);
    while(error){  
      if (digitalRead(A0) == WHITE && flag[0] == 0) {error--; flag[0] = 1; }
      if (digitalRead(A1) == WHITE && flag[1] == 0) {error--; flag[1] = 1; }
      if (digitalRead(A2) == WHITE && flag[2] == 0) {error--; flag[2] = 1; }
    }
    stopMoving();
    delay(100);
    RPM = 60;
    while(0){
      leftMotor(FORWARD, RPM);
      rightMotor(BACKWARD, RPM);
      if (digitalRead(A2) == WHITE) {break;}
    }
  };
  
  void moveRight(){
    int error = 3;
    int rotateRPM = 60, RPM;
    //int Kp = 25;
    int flag[5] = {0, 0, 0, 0, 0};
    leftMotor(FORWARD, RPM);
    rightMotor(BACKWARD, RPM);  
    delay(300);
    while(error){
      RPM = rotateRPM ;
      leftMotor(FORWARD, RPM);
      rightMotor(BACKWARD, RPM);
      if (digitalRead(A4) == WHITE && flag[0] == 0) {error--; flag[0] = 1;}
      if (digitalRead(A3) == WHITE && flag[1] == 0) {error--; flag[1] = 1;}
      if (digitalRead(A2) == WHITE && flag[2] == 0) {error--; flag[2] = 1;}
    }
    stopMoving();
    delay(50);
    RPM = 60;
    while(0){
      leftMotor(BACKWARD, RPM);
      rightMotor(FORWARD, RPM);
      if (digitalRead(A2) == WHITE) {break;}
    }
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
    if (Sensor::color(-4) == WHITE && Sensor::color(0) == WHITE && Sensor::color(4) == WHITE ) { return VERTEX;}
    if (Sensor::color(-5) == WHITE && Sensor::color(-4) == WHITE && Sensor::color(0) == BLACK && Sensor::color(4) == WHITE && Sensor::color(5) == WHITE) {
        return NODE;
    }
    if (Sensor::color(-5) == BLACK && Sensor::color(-4) == WHITE && Sensor::color(0) == BLACK && Sensor::color(4) == WHITE && Sensor::color(5) == BLACK) {
        //return BLOCKBASE;
        //TODO: Might wanna change the vertexType to "PATH2BB"
        return PATH;  //It's path before arriving blockbase!!!
    }

    if (Sensor::color(-5) == WHITE && Sensor::color(-4) == BLACK && Sensor::color(0) == WHITE && Sensor::color(4) == BLACK && Sensor::color(5) == WHITE) {
        //return BLOCKBASE;
        return PATH;  //It's path before arriving blockbase!!!
    }
    
    return PATH;
  };
 
  void rotateTo(Orientation finalX, Orientation finalY){
    //read bot.xorient, bot.yOirent and use a logic to turn the bot tothe destination
  }

  void fillPit(){
    
  }

  void ReadyForWet(){
    beep();
    beep();
    beep();
      
  };
  
  int ReadSensor(){
    
  };
};
