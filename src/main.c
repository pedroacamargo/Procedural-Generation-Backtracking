#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "imports.h"

void windowSetUp() {
  noecho();
  refresh();
}

NormalRoom * create(int col, int row) {

  // int randomY = rand() % col;
  // int randomX = rand() % row;

  NormalRoom * room = createNormalRoom(&col,&row);
  drawRoom(room);

  return room;
}







NormalRoom * createNormalRoom(int *rows, int *cols) {
  NormalRoom * newRoom;
  newRoom = malloc(sizeof(NormalRoom));

  // Room width/height randomization (width 10 - 20/ height 4 - 14)
  newRoom->width = rand() % 10 + 10;
  newRoom->height = rand() % 6 + 4;

  // position YX axis ( - height for the room doesn't overflow the screen)
  newRoom->pos.y = (rand() % (*rows - newRoom->height) + 1); 
  newRoom->pos.x = (rand() % (*cols - newRoom->width) + 1);
  // newRoom->pos.x = 10;
  // newRoom->pos.y = *rows - newRoom->height - 1; 

  return newRoom;
}



void drawRoom(NormalRoom * room) {
  // draw top and bottom
  for (int x = room->pos.x + 1; x < room->pos.x + room->width - 1; x++) {
    if (mvinch(room->pos.y,x) == '.') {
      mvprintw(room->pos.y,x,"."); // overwrite rooms
    } else {
      mvprintw(room->pos.y,x, "#");
    }
     
    if (mvinch(room->pos.y + room->height,x) == '.') {
      mvprintw(room->pos.y + room->height,x,"."); // overwrite rooms
    } else {
      mvprintw(room->pos.y + room->height, x, "#");
    }
  }

  // draw side walls / floor
  for (int y = room->pos.y; y <= room->pos.y + room->height; y++) {
    // draw side walls
    if (mvinch(y,room->pos.x) == '.') {
      mvprintw(y,room->pos.x,"."); // overwrite rooms
    } else {
      mvprintw(y,room->pos.x,"#");
    }

    if (mvinch(y,room->pos.x + room->width) == '.') {
      mvprintw(y,room->pos.x + room->width, ".");
    } else {
      mvprintw(y,room->pos.x + room->width - 1, "#");
    }

    // draw floors
    for (int x = room->pos.x + 1; x < room->pos.x + room->width - 1; x++) {
      if (y >= room->pos.y + room->height - 1) break; 
      mvprintw(y + 1,x,".");
    }    
  }
}

/*
  1   2   3   4
  
  12          5

  11          6

  10  9   8   7

*/


NormalRoom * randomizePosition(WINDOW * wnd,NormalRoom * room, int col, int row, int first, int iterations) {
  if (iterations == 12) return room;
  iterations++;
  NormalRoom * newRoom = createNormalRoom(&row,&col);
  Position p = room->pos;
  int top = 10, right = 12, bottom = 3, left = 6; // doors positioning
  int x = 0, y = 0; // room positioning
  int distanceX = 13; // gap X axis
  int distance = 5; // gap Y axis
  switch (first) {
    case 1:
      x = p.x - newRoom->width - distanceX;
      y = p.y - newRoom->height - distance;
      makeDoor(top,room);
      first++;
      break;
    case 2:
      x = p.x - (newRoom->width / 4);
      y = p.y - newRoom->height - distance;
      makeDoor(top,room);
      first++;
      break;
    case 3:
      x = p.x + (newRoom->width + (distanceX/2));
      y = p.y - newRoom->height - distance;
      makeDoor(top,room);
      first++;
      break;
    case 4:
      x = p.x + (newRoom->width + distanceX);
      y = p.y - (newRoom->height + distance);
      makeDoor(top,room);
      first++;
      break;
    case 5:
      x = p.x + (newRoom->width + distanceX);
      y = p.y - (newRoom->height / 2);
      makeDoor(right,room);
      first++;
      break;
    case 6:
      x = p.x + (newRoom->width + distanceX);
      y = p.y + (newRoom->height / 2);
      makeDoor(right,room);
      first++;
      break;
    case 7:
      x = p.x + (newRoom->width + distanceX);
      y = p.y + (newRoom->height + distance);
      makeDoor(bottom,room);
      first++;
      break;
    case 8:
      x = p.x + (newRoom->width * 3/4);
      y = p.y + newRoom->height + distance;
      makeDoor(bottom,room);
      first++;
      break;
    case 9:
      x = p.x - newRoom->width;
      y = p.y + newRoom->height + distance;
      makeDoor(bottom,room);
      first++;
      break;
    case 10:
      x = p.x - newRoom->width - distanceX;
      y = p.y + newRoom->height + distance;
      makeDoor(left,room);
      first++;
      break;
    case 11:
      x = p.x - newRoom->width - distanceX;
      y = p.y + (newRoom->height / 2);
      makeDoor(left,room);
      first++;
      break;
    case 12:
      x = p.x - newRoom->width - distanceX;
      y = p.y;
      makeDoor(left,room);
      break;
  }
  
  int has = checkPos(room,y,x,wnd,col,row);
  if (has == 1) {
    free(newRoom);
    if (first == 12) first = 1;
    return randomizePosition(wnd,room,col,row,first,iterations);
  } else {
    newRoom->pos.x = x;
    newRoom->pos.y = y;
    if (newRoom->pos.x == room->pos.x && newRoom->pos.y == room->pos.y) return room;
    newRoom = makeDoor(first, newRoom);
    drawDoor(room);
    return newRoom;
  }
}

int checkPos(NormalRoom * room, int randomY, int randomX, WINDOW * wnd, int cols, int rows) {
  int areaXstart = randomX - 3;
  int areaYstart = randomY - 3;
  int areaXend = randomX + room->width + 3;
  int areaYend = randomY + room->height + 3;

  for (int x = areaXstart;x < areaXend; x++) {
    for (int y = areaYstart; y < areaYend; y++) {
      refresh(); 
      if (mvwinch(wnd,y,x) == '.' || mvwinch(wnd,y,x) == '#' || y > cols - room->height || y < 0 || x > rows - room->width || x < 0) return 1;
    }
  }

  return 0;
}


int main()
{
  WINDOW * wnd = initscr();
  srand(time(NULL));
  windowSetUp();
  int col, row;
  getmaxyx(wnd,col,row);

  printw("x: %d | y: %d\n",row,col);
  // printw("randomX: %d | randomY: %d",randomX,randomY);
  srand(time(NULL));
  int firstPosition = rand() % 12 + 1;
  int maxRooms = 30, iterations = 0, rooms = 0;
  NormalRoom * room = create(col,row);
  while (1) {
    if(rooms == maxRooms) break;
    int key = getch();
    if (key == 'p' || key == 'P') {
      rooms++;
      room = randomizePosition(wnd,room,col,row,firstPosition,iterations);
      if (!(mvinch(room->pos.y,room->pos.x) == '#' || mvinch(room->pos.y,room->pos.x) == '.')) {
        drawRoom(room);
        drawDoor(room);
      }
    }
  }

  

  getch();

  endwin();

  return 0;
}