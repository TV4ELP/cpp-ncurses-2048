#include <vector>
#include <string>

#include "tile.hpp"

#ifndef _CLASS_MAP
#define _CLASS_MAP

class Map{
   private:
      int width;
      int height;
      
      std::vector<std::vector<Tile*> > vMap; //2 Dimensionaler Vector

      bool isFree(int rand);
      bool shiftLeftInternal(int row);

      void resetAddedTileFlag();
   protected:

   public:
      Map(int width, int height);

      int getHeight();
      int getWidth();

      bool calculateValidMoves();

      //Shiften wie alles und nichts
      void init();
      void shiftUp();
      void shiftLeft();
      void shiftRight();
      void shiftDown();
      void shiftLeftFromPositon(int h, int w);
      void addToLeft(int h, int w);
      void placeAtRandom();
      void shiftZeroLeft(int row);

      Tile* getTile(int tileNumber);
      Tile* getTile(int width, int height);

      std::string playRow(int row);
};

#endif