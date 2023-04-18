#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "imports.h"

NormalRoom * makeDoor(int first,NormalRoom * room) {
  first--;
  if (first == 0) first = 1;
  mvprintw(2,4,"Direction:%d  ",first);
  int randomX = rand() % (room->width - 2) + 1; // 1 - width
  int randomY = rand() % (room->height - 1) + 1; // 1 - height
  if (first == 10 || first == 9 || first == 8 || first == 7) {
    room->door.x = randomX + room->pos.x; // -1 is to forbid spawn doors in the corner
    room->door.y = room->pos.y;
  } else if (first == 12 || first == 11) {
    room->door.x = room->pos.x + room->width - 1; // stay in the wall and don't overflow
    room->door.y = randomY + room->pos.y;
  } else if (first == 1 || first == 2 || first == 3) {
    room->door.x = randomX + room->pos.x;
    room->door.y = room->pos.y + room->height;
  } else {
    room->door.x = room->pos.x;
    room->door.y = randomY + room->pos.y;
  }


  return room;
}

void drawDoor(NormalRoom * room) {
  mvprintw(room->door.y,room->door.x,"+");
}