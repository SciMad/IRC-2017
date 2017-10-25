#ifndef GRID
#define GRID

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

#endif
