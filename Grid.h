#ifndef GRID
#define GRID

typedef int VertexType;
typedef int Link;
typedef int Direction;
typedef int RunMode;
typedef boolean Visit;
typedef int Checkpoint;
typedef int Orientation;

const Orientation NOCHANGE = 0, EAST = 1, WEST = -1, NORTH = 1, SOUTH = -1; 
const Checkpoint STARTZONE = 0, CHECKPOINT1 = 1, CHECKPOINT2 = 2, CHECKPOINT3 = 3;
const RunMode DRY = 310, WET = 321;
const Direction LEFT=0, RIGHT=1, UP=2, DOWN =3, NOPATH = -1;
const VertexType PATH = 0 ,VERTEX = 1, TRANSFERZONE = 2, PHOTOPOINT = 3, BLOCKBASE = 4, DEPOSITED = 5, REDPIT = 6, BLUEPIT = 7, PIT3 = 8, BLOCK = 9, NODE = 10;
const Visit VISITED=1, UNVISITED=0;
const int INF = 999;
enum Color {RED, BLUE};
const int BLACK = 0, WHITE = 1;
class Math{
  public:
  static int sgn(int v) {
  return (v > 0) - (v < 0);
}
};

#endif
