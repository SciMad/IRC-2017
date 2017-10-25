#include "Grid.h"
class Bot{

  
  public:
  
  void moveForward(){
  
  };

  void moveBackward(){
    
  };

  void moveLeft(){
    
  };
   void moveRight(){
    
  };

  VertexType nodeDetect(){    
    if (digitalRead(A0==HIGH)){
      return VERTEX;
    }else{
      return PATH;
    }
  };

  void ReadyForWet(){
    
  };
  
  int ReadSensor(){
    
  };
};