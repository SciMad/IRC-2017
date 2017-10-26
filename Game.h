#include "Grid.h"
#include "EEPROMAnything.h"

class Vertex{
public:
  int x,y;
  int index;                //int index(){return 10*y+x; };
  VertexType type;
  Link links[4];            //The elements of this array are the index of other linked vertices stored in order as LRUD (Left, Right, Up, Down)
  boolean visited;
  
  Vertex(){
  };  
  static int getIndex(int m,int n){
    return m+10*n;
  }

  static Orientation dx(int index1,int index2){
    return Math::sgn((index1%10)-(index2%10));
  }
  static Orientation dy(int index1,int index2){    
    return  Math::sgn((index1/10)-(index2/10));
  }
  
};

class Game{  
public:
  RunMode mode;
  Vertex vertex[50];
  Checkpoint lastCheckpoint;
  int segmentStart;
  int completedSegments;
  int xOrient, yOrient;
  Vertex lastVertex;
  static const int dryPath[18];                 //Might be good to make this array not static variable and initialize later, to provide flexibility.
  void initializeDryRun(){
    initializeVertex();
    mode = DRY;
    lastCheckpoint = STARTZONE;
    segmentStart = Game::dryPath[0];
    completedSegments = 0;
    lastVertex.x = -1;
    lastVertex.y = 0;
    xOrient = EAST;
    yOrient = NOCHANGE;
  };
  void simulateDryCompletion(){
    int t;
    t = Vertex::getIndex(2,0);
    vertex[t].type = NODE;
    t = Vertex::getIndex(7,0);
    vertex[t].type = NODE;
    t = Vertex::getIndex(2,1);
    vertex[t].type = BLOCK;
    t = Vertex::getIndex(3,1);
    vertex[t].type = NODE;
    t = Vertex::getIndex(8,1);
    vertex[t].type = NODE;
    t = Vertex::getIndex(1,2);
    vertex[t].type = NODE;
    t = Vertex::getIndex(3,2);
    vertex[t].type = BLOCK;
    t = Vertex::getIndex(9,2);
    vertex[t].type = NODE; 
  };
  void initializeVertex(){
    //vertex[0] = new Vertex
       int t;
       for (int i=0; i<10; i++){
          for (int j=0; j<5; j++){
            t = Vertex::getIndex(i,j);
            vertex[t].x=i;
            vertex[t].y=j;
            
            (i!=0 && i!=6 && i!=5 && i!= 4) ? vertex[t].links[LEFT]=Vertex::getIndex(i-1,j) : vertex[t].links[LEFT] = NOPATH;      //to set the leftmost and central gap path to nopath
            (i!=9 && i!=3 && i!=5 && i!= 4) ? vertex[t].links[RIGHT]=Vertex::getIndex(i+1,j) : vertex[t].links[RIGHT] = NOPATH;      //to set the rightmost and central gap path to nopath
            
            (j!=4 && i!=5 && i!= 4) ? vertex[t].links[UP] = Vertex::getIndex(i,j+1) : vertex[t].links[UP] = NOPATH;    //to set the uppermost path to nopath
            (j!=0 && i!=5 && i!= 4) ? vertex[t].links[DOWN] = Vertex::getIndex(i,j-1) : vertex[t].links[DOWN] = NOPATH;    //to set the lowermost path to nopath
            
            vertex[t].type=VERTEX;
            vertex[t].visited=UNVISITED;  
            vertex[t].index=t;
          }
       }
       //======================================
       t = Vertex::getIndex(1,4);
       vertex[t].type = REDPIT;

       t = Vertex::getIndex(5,4);            //between 4 and 5 so selecting  the value of coordinate to 5 arbitaryly ... use 4 if u want 
       vertex[t].type = BLUEPIT;             
       
       t = Vertex::getIndex(8,4);
       vertex[t].type = PIT3;
        //======================================== above this are co ordinate for pits

        //========================================below this are coordinate for PHOTOPOINT
        t = Vertex::getIndex(6,0);
        vertex[t].type = PHOTOPOINT;
        
        t = Vertex::getIndex(6,1);
        vertex[t].type = PHOTOPOINT;
        
        t = Vertex::getIndex(6,2);
        vertex[t].type = PHOTOPOINT;
        
        //=====================FOR SPECIAL POINTS 
        
        t = Vertex::getIndex(2,3);         
        vertex[t].links[UP] = NOPATH;

        t = Vertex::getIndex(2,4);         
        vertex[t].links[DOWN] = NOPATH;

        t = Vertex::getIndex(3,4);         
        vertex[t].links[DOWN] = NOPATH;

        t = Vertex::getIndex(4,4);         
        vertex[t].links[LEFT] = Vertex::getIndex(3,4);
        vertex[t].links[RIGHT] = Vertex::getIndex(5,4);

        t = Vertex::getIndex(5,4);
        vertex[t].links[LEFT] = Vertex::getIndex(4,4);
        vertex[t].links[RIGHT] = Vertex::getIndex(6,4);
        
        t = Vertex::getIndex(3,3);
        vertex[t].links[UP] = NOPATH; 
        
        t = Vertex::getIndex(6,3);
        vertex[t].links[UP] = NOPATH; 
        
        t = Vertex::getIndex(7,3);
        vertex[t].links[RIGHT] = NOPATH; 
        vertex[t].links[UP] = NOPATH; 
        
        t = Vertex::getIndex(3,4);
        vertex[t].links[RIGHT] = Vertex::getIndex(4,4);
        
        t = Vertex::getIndex(8,3);
        vertex[t].links[LEFT] = NOPATH;
        vertex[t].links[RIGHT] = NOPATH;
        
        t = Vertex::getIndex(9,3);
        vertex[t].links[LEFT] = NOPATH;  
        
        t = Vertex::getIndex(6,4);
        vertex[t].links[LEFT] = Vertex::getIndex(5,4);
        vertex[t].links[RIGHT] = Vertex::getIndex(7,4);
        vertex[t].links[DOWN] = NOPATH;

        t = Vertex::getIndex(7,4);
        vertex[t].links[DOWN] = NOPATH;
        

        //TODO --> The Links of following vertices depends upon direction of inclination of See-Saw
        //#Review
        t = Vertex::getIndex(8,2); vertex[t].links[UP] = Vertex::getIndex(8,3);
        t = Vertex::getIndex(8,4); vertex[t].links[DOWN] = Vertex::getIndex(8,3);
        
        

  }; //end of initialize
};
const int Game::dryPath[18]= {0,3,13,10,20,23,33,30,40,49,9,8,28,27,7,6,36,37};

