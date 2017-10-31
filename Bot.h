#include "Grid.h"

class Bot{

  
  public:
  Color readBlockColor(){
  };
  
  void moveForward(){
  
  };

  void moveBackward(){
    
  };

  void moveLeft(){
    
  };
  
  void moveRight(){
    
  };

  void traverse(int*, int){
    
  }
  

  VertexType nodeDetect(){    
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
