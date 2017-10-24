typedef int VertexType;
typedef int Link;
typedef int Direction;
typedef boolean RunMode;
typedef boolean Visit;
typedef int Checkpoint;
typedef int Orientation;

const Orientation NOCHANGE = 0, EAST = 1, WEST = -1, NORTH = 1, SOUTH = -1; 
const Checkpoint STARTZONE = 0, CHECKPOINT1 = 1, CHECKPOINT2 = 2, CHECKPOINT3 = 3;
const RunMode DRY = 0, WET = 1;
const Direction LEFT=0, RIGHT=1, UP=2, DOWN =3, NOPATH = -1;
const VertexType PATH = 0 ,VERTEX = 1, NODE = 2, BLOCKBASE = 3, REDPIT = 4, BLUEPIT = 5, PIT3 = 6, PHOTOPOINT = 7;
const Visit VISITED=1,UNVISITED=0;

class Math{
  public:
  static int sgn(int v) {
  return (v > 0) - (v < 0);
}
};

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
  static const int dryPath[18];             //={0,3,13,10,20,23,33,30,40,49,9,8,28,27,7,6,36,37};
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
} game; // end of Game class;

const int Game::dryPath[18]= {0,3,13,10,20,23,33,30,40,49,9,8,28,27,7,6,36,37};


Game LoadFromEEPROM(){
  Game tempGame;
  //ReadEEPROM;
  //if (first byte on EPPROM is 0 (DRY) else load all the details of the game
  tempGame.mode = DRY;  //WritesegmentStart a code to read from game mode from EEPROM   
  tempGame.lastCheckpoint = STARTZONE;
  tempGame.segmentStart = Game::dryPath[0];   //Replace 0 by completedSegments read from EEPROM
  tempGame.completedSegments = 0;
  tempGame.lastVertex.x = -1;
  tempGame.lastVertex.y = 0;
  tempGame.xOrient = EAST;
  tempGame.yOrient = NOCHANGE;
  return tempGame;
};

void setup(){
  Serial.begin(9600);
  game = LoadFromEEPROM();
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
