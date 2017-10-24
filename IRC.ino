typedef int VertexType;
typedef int Link;
typedef int Direction;
typedef boolean RunMode;
typedef boolean Visit;

const RunMode DRY = 0, WET = 1;
const Direction LEFT=0, RIGHT=1, UP=2, DOWN =3, NOPATH = -1;
const VertexType VERTEX = 0, NODE = 1, BLOCKBASE = 2, REDPIT = 3, BLUEPIT = 4, PIT3 = 5, PHOTOPOINT = 6;
const Visit VISITED=1,UNVISITED=0;


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
};

class IRC_GamePlay{  
public:
  RunMode mode;
  Vertex vertex[50];
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
} game; // end of IRC_GamePlay class;

IRC_GamePlay LoadFromEEPROM(){
  IRC_GamePlay tempGame;
  //ReadEEPROM;
  //if (first byte on EPPROM is 0 (DRY) else load all the details of the game
  tempGame.mode = DRY;  //Write a code to read from EEPROM and load the whole game if not in dry run
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
    int i=49;
    Serial.println("\n\nIndex=");
    Serial.println(i);
    Serial.println("\nLEFT =");
    Serial.println(game.vertex[i].links[LEFT]);
    Serial.println("\nRIGHT= ");
    Serial.println(game.vertex[i].links[RIGHT]);
    Serial.println("\nUP = ");
    Serial.println(game.vertex[i].links[UP]);
    Serial.println("\n DOWN = ");
    Serial.println(game.vertex[i].links[DOWN]);

  exit(0);
    
}
