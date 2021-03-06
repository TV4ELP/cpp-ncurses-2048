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
   int moves = 0;
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         if(this->vMap[h][w]->validU){
            moves ++;
         }
      }
   }

   if(moves > 0){
      for(int collumn = 0; collumn < this->width; collumn++){
         this->shiftUpInternal(collumn);
      }
      this->resetAddedTileFlag();//naechste Runde = wieder addierbar
      this->placeAtRandom(); //wenn move cool dann mach mir nen neues ding
   }
}

void Map::shiftRight(){
   int moves = 0;
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         if(this->vMap[h][w]->validR){
            moves ++;
         }
      }
   }

   if(moves > 0){
      for(int row = 0; row < this->height; row++){
         this->shiftRightInternal(row);
      }
      this->resetAddedTileFlag();//naechste Runde = wieder addierbar
      this->placeAtRandom(); //wenn move cool dann mach mir nen neues ding
   }
}

bool Map::shiftRightInternal(int row){
   this->shiftZeroRight(row); //alles nach Links ballern
   this->calculateValidMoves(); //neu die moeglichkeiten berechnen
   for(int w = 0; w < this->width; w++){
      Tile* thisTile = this->getTile(w, row);
      if(thisTile->validR == false){
         continue;
      }else{
         Tile* rightTile = this->getTile(w +1, row);
         if(rightTile->number != 0){//nicht null, dann swappen
            if(rightTile->alreadyAdded == false){
               this->addToRight(row, w);
            }
         }
      }
      this->calculateValidMoves(); //recalculate
   }
   this->shiftZeroRight(row);
}

bool Map::shiftUpInternal(int collumn){
   //TODO
}

void Map::addToRight(int h, int w){
   Tile* currentTile = this->vMap[h][w];
   Tile* rightTile = this->vMap[h][w +1];
   rightTile->number = rightTile->number + currentTile->number;
   rightTile->alreadyAdded = true;
   currentTile->number = 0; 
   this->shiftRightFromPositon(h, w-1);//Jetzt das ding an das Ende schieben
}

void Map::shiftLeft(){
   int moves = 0;
   for(int h = 0; h < this->height; h++){
      for(int w = 0; w < this->width; w++){
         if(this->vMap[h][w]->validL){
            moves ++;
         }
      }
   }

   if(moves > 0){
      for(int row = 0; row < this->height; row++){
         this->shiftLeftInternal(row);
      }
      this->resetAddedTileFlag();//naechste Runde = wieder addierbar
      this->placeAtRandom(); //wenn move cool dann mach mir nen neues ding
   }
}

bool Map::shiftLeftInternal(int row){
   this->shiftZeroLeft(row); //alles nach Links ballern
   this->calculateValidMoves(); //neu die moeglichkeiten berechnen
   for(int w = 0; w < this->width; w++){
      Tile* thisTile = this->getTile(w, row);
      if(thisTile->validL == false){
         continue;
      }else{
         Tile* leftTile = this->getTile(w -1, row);
         if(leftTile->number != 0){//nicht null, dann swappen
            if(leftTile->alreadyAdded == false){
               this->addToLeft(row, w);
            }
         }
      }
      this->calculateValidMoves(); //recalculate
   }
   this->shiftZeroLeft(row);
}

void Map::shiftZeroLeft(int row){
   int numZeroTotal = 0;
   int numZeroLeft = 0;
   for (int w = 0; w < this->width; w++){
      if (this->vMap[row][w]->number == 0){
         numZeroTotal++;
      }
   }

   if(numZeroTotal == this->width){
      return;
   }

   if(numZeroTotal != 0){
      for (int w = 0; w < this->width - numZeroTotal; w++){//alle Nullen die Links noch sind 
         if (this->vMap[row][w]->number == 0){
            this->shiftLeftFromPositon(row, w +1);
         }
      }

      for (int w = 0; w < this->width - numZeroTotal; w++){//alle Nullen die Links noch sind 
         if (this->vMap[row][w]->number == 0){
            numZeroLeft ++;
         }
      }
   }

   if(numZeroLeft != 0){
      this->shiftZeroLeft(row);
   }

}

void Map::shiftZeroRight(int row){
   int numZeroTotal = 0;
   int numZeroLeft = 0;
   for (int w = 0; w < this->width; w++){
      if (this->vMap[row][w]->number == 0){
         numZeroTotal++;
      }
   }

   if(numZeroTotal == this->width){//alles null
      return;
   }

   if(numZeroTotal != 0){
      for (int w = this->width -1; w >= 0 + numZeroTotal; w--){//alle Nullen die rechts noch sind 
         if (this->vMap[row][w]->number == 0){
            this->shiftRightFromPositon(row, w -1);
         }
      }

      for (int w = this->width -1; w >= 0 + numZeroTotal; w--){//alle Nullen die rechts noch sind 
         if (this->vMap[row][w]->number == 0){
            numZeroLeft ++;
         }
      }
   }

   if(numZeroLeft != 0){
      this->shiftZeroRight(row);
   }

}

void Map::shiftLeftFromPositon(int h, int w){
   for (int i = w; w < this->width; w++){
      Tile* tempTile = this->vMap[h][w -1];
      this->vMap[h][w -1] = this->vMap[h][w];
      this->vMap[h][w] = tempTile; 
   }
   
}

void Map::shiftRightFromPositon(int h, int w){
   for (int i = w; w >= 0; w--){
      Tile* tempTile = this->vMap[h][w +1];
      this->vMap[h][w +1] = this->vMap[h][w];
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
      if(isFree == true){
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
            if(currentTile->number != 0){
               if(nextTileUp->number == 0 || currentTile->number == nextTileUp->number){
                  if(currentTile->alreadyAdded){
                     currentTile->validU = false;
                  }else{
                     currentTile->validU = true;
                  }
               }else{
                  currentTile->validU = false;
               }
            }else{
               currentTile->validU = false;
            }
         }

         if(w == 0){
            currentTile->validL = false;
         }else{
            Tile* nextTileLeft = this->getTile(w -1, h);//Tile links dem aktuellen
            if(currentTile->number != 0){
               if(nextTileLeft->number == 0 || currentTile->number == nextTileLeft->number){
                  if(currentTile->alreadyAdded){
                     currentTile->validL = false;
                  }else{
                     currentTile->validL = true;
                  }
               }else{
                  currentTile->validL = false;
               }
            }else{
               currentTile->validL = false;
            }
         }

         if(h == this->height -1){
            currentTile->validD = false;
         }else{
            Tile* nextTileDown = this->getTile(w, h +1);//Tile unter dem aktuellen
            if(currentTile->number != 0){
               if(nextTileDown->number == 0 || currentTile->number == nextTileDown->number){
                  if(currentTile->alreadyAdded){
                     currentTile->validD = false;
                  }else{
                     currentTile->validD = true;
                  }
               }else{
                  currentTile->validD = false;
               }
            }else{
               currentTile->validD = false;
            }
         }

         if(w == this->width -1){
            currentTile->validR = false;
         }else{
            Tile* nextTileRight = this->getTile(w +1, h);//Tile rechts dem aktuellen
            if(currentTile->number != 0){
               if(nextTileRight->number == 0 || currentTile->number == nextTileRight->number){
                  if(currentTile->alreadyAdded){
                     currentTile->validR = false;
                  }else{
                     currentTile->validR = true;
                  }
               }else{
                  currentTile->validR = false;
               }
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