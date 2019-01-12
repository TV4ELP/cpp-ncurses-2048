#include <iostream>
#include <string>
#include <random>
#include "../../header/map.hpp"

#define KEY_DOWN	0402		/* down-arrow key */
#define KEY_UP		0403		/* up-arrow key */
#define KEY_LEFT	0404		/* left-arrow key */
#define KEY_RIGHT	0405		/* right-arrow key */

Map::Map(int width, int height){
   this->height = height;
   this->width = width;
   
   Tile* tempTile = NULL;
   
   for(int ih = 0; ih < height; ih++){
      std::vector<Tile*> vecc; 
      vMap.push_back(vecc);
      for (int iw = 0; iw < width; iw++){
         Tile* tempTile = new Tile(0);
         vMap[ih].push_back(tempTile);
         std::cout << "Tile Created[" << ih << "][" << iw << "]" << "R:::" << tempTile->number <<std::endl;
      }
   }
}

void Map::init(){
   this->placeAtRandom();
   this->placeAtRandom();
}

void Map::shiftUp(){\
   this->placeAtRandom();
}

void Map::shiftLeft(){
   for(int row = 0; row < this->height; row++){
      this->shiftLeftInternal(row);
   }
   this->resetAddedTileFlag();//naechste Runde = wieder addierbar
   this->placeAtRandom(); //wenn move cool dann mach mir nen neues ding
}

bool Map::shiftLeftInternal(int row){

   for(int w = 0; w < this->width; w++){
      Tile* thisTile = this->getTile(w, row);
      if(thisTile->validL == false){
         continue;
      }else{
         Tile* leftTile = this->getTile(w -1, row);
         if(leftTile->number == 0){//nicht null, dann swappen
            this->shiftLeftFromPositon(row, w);
         }else{
            if(leftTile->alreadyAdded == false){
               this->addToLeft(row, w);
            }
         }
      }
      this->calculateValidMoves(); //recalculate
   }
   this->calculateValidMoves(); //recalculate
   int numMovesLeft = 0;
   for(int w = 0; w < this->width; w++){
      if(this->vMap[row][w]->validL){
         numMovesLeft ++;
      }
   }
   if(numMovesLeft != 0){//again and again
      this->shiftLeftInternal(row);
   }
   
}

void Map::shiftLeftFromPositon(int h, int w){
   std::vector<Tile*> row = this->vMap[h];
   for (int i = w; w < this->width; w++){
      Tile* tempTile = this->vMap[h][w -1];
      this->vMap[h][w -1] = this->vMap[h][w];
      this->vMap[h][w] = tempTile; 
   }
}

void Map::addToLeft(int h, int w){
   Tile* currentTile = this->vMap[h][w];
   Tile* leftTile = this->vMap[h][w -1];
   leftTile->number = leftTile->number + currentTile->number;
   leftTile->alreadyAdded = true;
   currentTile->number = 0; 
   this->shiftLeftFromPositon(h, w+1);//Jetzt das ding an das Ende schieben
}


void Map::shiftRight(){
   this->placeAtRandom();
}

void Map::shiftDown(){
   this->placeAtRandom();
}

void Map::placeAtRandom(){
   bool placed = 0;
   while(!placed){
      int max = this->height * this->width -1;
      std::random_device rd;//random from hardware
      std::mt19937 eng(rd());//seed generator mt....
      std::uniform_int_distribution<> distr(0,max);

      int rand = distr(eng);//neue nummer mit engine
      bool isFree = this->isFree(rand);

      std::uniform_int_distribution<> distF(0,2);//wieviele Nummer zum verteilen der zweien und vieren (0,1) = 50%
      int randNumber = distF(eng);
      if(isFree == 1){
         Tile* tile = this->getTile(rand);
         if(randNumber > 0){ //mehr Nummern, mehr zweien und weniger vieren
            tile->number = tile->number + 2;
         }else{
            tile->number = tile->number + 4;
         }
         
         placed = true;
      }
   }
}

void Map::resetAddedTileFlag(){
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         this->vMap[h][w]->alreadyAdded = false;
      }
   }
}

bool Map::isFree(int rand){
   Tile* tile = this->getTile(rand);
   int number = tile->number;

   if(number == 0){
      return 1;
   }else{
      return 0;
   }
}

bool Map::calculateValidMoves(){
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         Tile* currentTile = this->getTile(w, h);
         if(h == 0){
            currentTile->validU = false;
         }else{
            Tile* nextTileUp = this->getTile(w, h -1);//Tile ueber dem aktuellen
            if(nextTileUp->number == 0 || currentTile->number == nextTileUp->number){
               currentTile->validU = true;//TODO ALREADY ADDED
            }else{
               currentTile->validU = false;
            }
         }

         if(w == 0){
            currentTile->validL = false;
         }else{
            Tile* nextTileLeft = this->getTile(w -1, h);//Tile links dem aktuellen
            if((nextTileLeft->number == 0 && currentTile->number != 0) || (currentTile->number == nextTileLeft->number && currentTile->number != 0)){
               if(nextTileLeft->alreadyAdded){
                  currentTile->validL = false;
               }else{
                  currentTile->validL = true;
               }
            }else{
               currentTile->validL = false;
            }
         }

         if(h == this->height -1){
            currentTile->validD = false;
         }else{
            Tile* nextTileDown = this->getTile(w, h +1);//Tile unter dem aktuellen
            if(nextTileDown->number == 0 || currentTile->number == nextTileDown->number){
               currentTile->validD = true;//TODO ALREADY ADDED
            }else{
               currentTile->validD = false;
            }
         }

         if(w == this->width -1){
            currentTile->validR = false;
         }else{
            Tile* nextTileRight = this->getTile(w +1, h);//Tile rechts dem aktuellen
            if(nextTileRight->number == 0 || currentTile->number == nextTileRight->number){
               currentTile->validR = true;//TODO ALREADY ADDED
            }else{
               currentTile->validR = false;
            }
         }
      }
   }
   int numMoves = 0;
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         Tile* tile = this->getTile(w, h);
         numMoves += tile->validD;
         numMoves += tile->validL;
         numMoves += tile->validR;
         numMoves += tile->validU;
      }
   }

   if(numMoves == 0){
      return false;
   }else{
      return true;
   }
}

Tile* Map::getTile(int width, int height){
   return vMap[height][width];
}

Tile* Map::getTile(int tileNumber){
   if(tileNumber == 0){
      return this->getTile(0,0);
   }
   int row = tileNumber / this->width;
   int collumn;
   collumn = tileNumber % this->width;
   Tile* tile = this->getTile(collumn, row);
   return tile;
}

int Map::getHeight(){
   return this->height;
}

int Map::getWidth(){
   return this->width;
}

std::string Map::playRow(int row){
   std::string buffer = "";
   for (int iw = 0; iw < this->height; iw++){
      buffer += std::to_string(this->vMap[row][iw]->number);
   }
   return buffer;
}