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
    if (analogValue < 350) return WHITE; else return BLACK;
  }
};

class Bot{
  public:
  int xOrient, yOrient;
  Vertex lastVertex;
  float lastError;
  float speedFactor;
  int forwardStopDelay;
  int rotateStopDelay;
  

  float RPM;
  float rightRPM;
  float leftRPM;
  float Kp;
  float Kd;
  float Ki;
  
  float error;            // current error  -- P term 
  float previousError;
  float difference;       //difference between current error and previousError (error - previousError) -- D term
  float totalError;       //sum of errors -- I term
  float change;           //amount of change in RPM

  bot(){
    RPM = 100;
    Kp = 15;
    Kd = 0;
    Ki = 0.05;
    totalError = 0;
    previousError = 0;
  }

  void moveStraight(){
    previousError = error;
    totalError += error;
    error = getErr();
    difference = error - previousError;
    change = Kp * error + Kd*difference + Ki*totalError;
    leftRPM = RPM + change;
    rightRPM = RPM -change;
      if (error > 0) rightRPM = 0;
      if (error < 0) leftRPM = 0;
      moveForward(leftRPM, rightRPM);
  }
  
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
    leftMotor(FORWARD, speedFactor*leftRPM);
    rightMotor(FORWARD, speedFactor*rightRPM);
  };

  void moveBackward(int leftRPM, int rightRPM){
    leftMotor(BACKWARD, speedFactor*leftRPM);
    rightMotor(BACKWARD, speedFactor*rightRPM);
  };

  void stopMoving(){
    leftMotor(STOP, 0);
    rightMotor(STOP, 0);
  };
  
  void leftMotor(MotorDirection motorDirection, int RPM){
    if (RPM > 200) RPM = 200;
    if (RPM < 25) motorDirection = STOP;
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
    if (RPM > 200) RPM = 200;
    if (RPM < 25) motorDirection = STOP;
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

  float getErr(){       // For 11 Linear Sensors
    float error = lastError, sensorPosition = 0;
    float sensorTarget = 0;     //For straight line movement, the middle sensor must be at middle of the line, i.e. at position = 0
    if (Sensor::color(-1) == WHITE && Sensor::color(0) == WHITE && Sensor::color(1) == WHITE) {sensorPosition = 0;}
    
    //Positive Position When Bot Deviates Right
    
    if (Sensor::color(2) == BLACK && Sensor::color(1) == BLACK && Sensor::color(0) == WHITE) {sensorPosition = 1;}    
    if (Sensor::color(0) == WHITE && Sensor::color(-2) == WHITE && Sensor::color(-3) == WHITE) {sensorPosition = 1.5;}
    if (Sensor::color(1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == WHITE && Sensor::color(-3) == WHITE) {sensorPosition = 3;}
    if (Sensor::color(1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == BLACK && Sensor::color(-3) == WHITE) {sensorPosition = 4;}
    if (Sensor::color(3) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-3) == BLACK && Sensor::color(-4) == WHITE) {sensorPosition = 5;}
    

    //Negative Position When Bot Deviates Left
    if (Sensor::color(-2) == BLACK && Sensor::color(-1) == BLACK && Sensor::color(0) == WHITE) {sensorPosition = -1;}    
    if (Sensor::color(0) == WHITE && Sensor::color(2) == WHITE && Sensor::color(3) == WHITE) {sensorPosition = -1.5;}
    if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(1) == WHITE && Sensor::color(3) == WHITE) {sensorPosition = -3;}
    if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(1) == BLACK && Sensor::color(3) == WHITE) {sensorPosition = -4;}
    if (Sensor::color(-3) == BLACK && Sensor::color(0) == BLACK && Sensor::color(3) == BLACK && Sensor::color(4) == WHITE) {sensorPosition = -5;}
    

    //sensorPosition For Blockbase's Path
    
    //if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == BLACK) {sensorPosition = 0;}
    //if (Sensor::color(-1) == BLACK && Sensor::color(0) == BLACK && Sensor::color(-1) == WHITE) {sensorPosition = 1;}
    //if (Sensor::color(-1) == WHITE && Sensor::color(0) == BLACK && Sensor::color(-1) == BLACK) {sensorPosition = -1;}

    //TODO: Work on Higher errors if bot deviates

    error = sensorTarget-sensorPosition;
    lastError = error;
    return error; // error = target position - current position (of central sensor)
  }

  void gripBlock(){
    Serial.println("GRP");    //I am gripping
  };

  void fillTransferZone(){
    
  };

  void moveUntil(VertexType vertexType, MotorDirection motorDirection = FORWARD){
    float RPM = 90, rightRPM, leftRPM;
    float error = getError(), previousError = 0, difference = 0, totalError = 0, change;
    float Kp = 10, Kd = 0, Ki = 0.05;
    while(1){   
    previousError = error;
    totalError += error;
    error = getErr();
    difference = error - previousError;
    change = Kp * error + Kd*difference + Ki*totalError;
    leftRPM = RPM + change;
    rightRPM = RPM -change;
      if (error > 0) rightRPM = 0; if (error < 0) leftRPM = 0;
      if (motorDirection == FORWARD) moveForward(leftRPM, rightRPM); else moveBackward(leftRPM, rightRPM);
      if (digitalRead(A11) == LOW){     //Single Sensor Module Sends HIGH when Black and LOW when White
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
    digitalWrite(12, HIGH);
    delay(interval);
    digitalWrite(12, LOW);
    delay(interval);
    digitalWrite(12, HIGH);
    delay(interval);
    digitalWrite(12, LOW);
  }
  
  void beep(int interval=100){
    digitalWrite(12, HIGH);
    delay(100);
    digitalWrite(12, LOW);
  }

  void moveBackward(){
    
  };

  void moveLeft(int dir=1){
    int error = 3;
    int rotateRPM = 90, RPM;
    //int Kp = 25;
    int flag[5] = {0, 0, 0, 0, 0};
    RPM = rotateRPM;
    
    leftMotor(dir*BACKWARD, RPM);
    rightMotor(dir*FORWARD, RPM);  
    delay(400);
    do{
      
    }while(Sensor::color(-1*dir*4) == BLACK);


    do{
      
    }while(Sensor::color(-1*dir*0) == BLACK);   //-1*dir*-1
    stopMoving();
    delay(80);
    leftMotor(-dir*BACKWARD, RPM);
    rightMotor(-dir*FORWARD, RPM);  
    delay(50);
    beepbeep();
    RPM = 60;
    while(0){
      leftMotor(FORWARD, RPM);
      rightMotor(BACKWARD, RPM);
      if (digitalRead(A2) == WHITE) {break;}
    }
  };
  
  void moveRight(){
    moveLeft(-1);
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
        return BLOCKBASE;  //It's path before arriving blockbase!!!
    }

    if (Sensor::color(-5) == BLACK && Sensor::color(-4) ==BLACK  && Sensor::color(0) == WHITE && Sensor::color(4) == BLACK && Sensor::color(5) == BLACK) {
        //return BLOCKBASE OR PATH;
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
