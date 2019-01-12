
#ifndef _CLASS_TILE
#define _CLASS_TILE

class Tile{
   public:
      Tile(int n);

      int number;
      bool validL;
      bool validR;
      bool validU;
      bool validD;
      bool alreadyAdded = false;
};

#endif