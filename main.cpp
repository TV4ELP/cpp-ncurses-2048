#include<ncursesw/cursesw.h>
#include<iostream>
#include "header/map.hpp"


int main(int argc, char const *argv[])
{
   int a = 0;
   Map* playMap = new Map(10,10);
   std::cin >> a;
   playMap->init();//wir muessen irgendwo starten

   initscr();//Initialisisre ncurses
   cbreak();//Output Buffer und kei Line Break
   keypad(stdscr, TRUE);

   int key;
   while(1){//Game Loop, ich weiss nicht toll aber ey


      int height = playMap->getHeight();
      std::string buffer = "";
      for(int ih = 0; ih < height; ih++){
         buffer.append(playMap->playRow(ih));
         mvaddstr(ih,0, buffer.c_str()); //Buffer neu Bauen
         buffer.clear(); //muessen dann ja wieder frei machen
      }
      refresh(); //Buffer tauschen
      
      bool possibleToMove = playMap->calculateValidMoves();
      if(possibleToMove == false){
         break;
      }

      key = getch(); //key einlesen
      if(key == KEY_UP){
         playMap->shiftUp();
      }
      if(key == KEY_LEFT){
         playMap->shiftLeft();
      }
      if(key == KEY_DOWN){
         playMap->shiftDown();
      }
      if(key == KEY_RIGHT){
         playMap->shiftRight();
      }
   }

   endwin(); //ncurses kann sich wieder verpissen
   std::cout << "Du hast verkackt du Bonobo" << std::endl;
   return 0;
}
