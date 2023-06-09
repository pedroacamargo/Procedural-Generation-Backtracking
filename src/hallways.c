#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>
#include "imports.h"

// door.x + 1 -> direita
// door.x - 1 -> esquerda
// door.y + 1 -> baixo
// door.y - 1 -> cima

// Main function
void drawHallway(NormalRoom * newRoom, NormalRoom * room,WINDOW * wnd) {
  int distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance

  // This is an important variable to say if the direction changed, to help in the corridors' walls construction.
  // Needs to be reseted every iteration to axisSwap = 0; 
  int axisSwap = 0; // if = 0, axis didn't changed, if = 1, axis've just changed 

  int isFirst = 1; // if is the first iteration in the hallway construction
  char axis = 'x';
  while (distance != 0) {
    int xDist = calculateDistanceXAxis(&newRoom->door,&room->door,0);
    int yDist = calculateDistanceYAxis(&newRoom->door,&room->door,0);

    if (newRoom->doorAxis == 'x') {
      yDist--;
    } else {
      xDist--;
    }

    if((xDist + yDist) == 0) break;

    int sizeForbiddenToPass = 2;
    unsigned int blocks[] = {'#', '.'};

    // this code is remediating in the case the hallway has a wall in front of it
    for (int i = 0; i < sizeForbiddenToPass; i++) {
      
      // fix a bug -> the first iteration the hallway will be inside a wall, then the code needs to jump 1 iteration
      if (isFirst == 1) {
        buildWalls(room,axis,isFirst,axisSwap);
        isFirst = 0;
        break;
      }


      if ((mvinch(room->door.y,room->door.x + 1) == blocks[i] || mvinch(room->door.y,room->door.x - 1) == blocks[i] || xDist == 0) && axis == 'x') {
        // function to check which way is faster
        int distanceY = calculateDistanceYAxis(&newRoom->door,&room->door,0);
        int newDistanceY = calculateDistanceYAxis(&newRoom->door,&room->door,1);
        axisSwap = 1;

        // if distance going down increased (the hallway needs to go bottom)
        if (distanceY > newDistanceY) {
          int y = room->door.y;
          axis = 'y';
          while (mvinch(y,room->door.x + 1) == blocks[i] || mvinch(y,room->door.x - 1) == blocks[i]) {
            room->door.y++;
            y++;
            distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance
            mvprintw(room->door.y,room->door.x,"+");
          }
        // if distance goind down decreased
        } else if (distanceY <= newDistanceY) {
          int y = room->door.y;
          axis = 'y';
          while (mvinch(y,room->door.x + 1) == blocks[i] || mvinch(y,room->door.x - 1) == blocks[i]) {
            room->door.y--;
            y--;
            distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance
            mvprintw(room->door.y,room->door.x,"+");
          }
        }
      } else if ((mvinch(room->door.y + 1,room->door.x) == blocks[i] || mvinch(room->door.y - 1,room->door.x) == blocks[i] || yDist == 0) && axis == 'y') {
        // function to check which way is faster
        int distanceX = calculateDistanceXAxis(&newRoom->door,&room->door,0);
        int newDistanceX = calculateDistanceXAxis(&newRoom->door,&room->door,1);

        // if distance going right increased (the hallway needs to go left)
        if (distanceX > newDistanceX) {
          int x = room->door.x;
          axis = 'x';
          while (mvinch(room->door.y + 1,x) == blocks[i] || mvinch(room->door.y - 1,x) == blocks[i]) {
            room->door.x++;
            x++;
            distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance
            mvprintw(room->door.y,room->door.x,"+");
          }
        // if distance goind down decreased
        } else if (distanceX <= newDistanceX) {
          axis = 'x';
          int x = room->door.x;
          while (mvinch(room->door.y + 1,x) == blocks[i] || mvinch(room->door.y - 1,x) == blocks[i]) {
            room->door.x--;
            x--;
            distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance
            mvprintw(room->door.y,room->door.x,"+");
          }
        }
      }
    }





    int newDistance = calculateNewDistance(&newRoom->door,&room->door,1,axis); // this will calculate the distance between the 2 doors in manhattan distance

    // If the distance calculating the new position decreased, print the hallway pixel in this position
    if (distance < newDistance) {
      if (axis == 'x') {
        // go left

        // if there is a floor, don't create corridors
        if (isThereAFloor(room) == 0) {
          room->door.x -= 1;
          continue;
        } 

        // if there isn't a wall, continue the way
        if(isInsideAWall(room,-1,axis) == 1) {
          room->door.x -= 1; // go to left
        }

        mvprintw(room->door.y, room->door.x, "+");
      } else if (axis == 'y') {
        // go up

        // if there is a floor, don't create corridors
        if (isThereAFloor(room) == 0) {
          room->door.y -= 1;
          continue;
        } 

        // if there isn't a wall, continue the way
        if (isInsideAWall(room,-1,axis) == 1) {
          room->door.y -= 1;
        }

        mvprintw(room->door.y,room->door.x,"+");
      }

    // else if the distance calculating the new position increased, print the hallway in the opposite position
    } else if (distance > newDistance) {
      if (axis == 'x') {
        // go right

        // if there is a floor, don't create corridors
        if (isThereAFloor(room) == 0) {
          room->door.x += 1;
          continue;
        } 

        // if there isn't a wall, continue the way
        if(isInsideAWall(room,1,axis) == 1) {
          room->door.x += 1;
        }

        mvprintw(room->door.y, room->door.x, "+");
      } else if (axis == 'y') {
        // go down

        // if there is a floor, don't create corridors
        if (isThereAFloor(room) == 0) {
          room->door.y += 1;
          continue;
        } 

        // if there isn't a wall, continue the way
        if (isInsideAWall(room,1,axis) == 1) {
          room->door.y += 1;
        }

        mvprintw(room->door.y,room->door.x,"+");
      }
    }
  
  buildWalls(room,axis,isFirst,axisSwap);
  axisSwap = 0;
  distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance (Update the distance)
  }
}


// This function is to test if there's a floor in the place the corridor wants to be constructed
// If there is a floor, the floor will continue in that place untouched
int isThereAFloor(NormalRoom * room) {
  if (mvinch(room->door.y,room->door.x) == '.') {
    return 0;
  }
  return 1;
}


// This function is in case the first iteration and the door is in the top/bottom of the room, if this happens, 
// the door will go a pixel above/below and undo the repositioning of the door in X axis, 
// just to don't overwrite the room walls and happens to have a hallway inside the wall.
int isInsideAWall(NormalRoom * room, int move, char axis) {
  
  if (axis == 'x') {

    if (mvinch(room->door.y,room->door.x + move) == '#') {

      // if the exit is downwards, print the hallway start going below the room
      if(mvinch(room->door.y + 1,room->door.x) != '.' && mvinch(room->door.y + 1,room->door.x) != '#') { 
        room->door.y += 1;
        return 0;
      } // else if the exit is upwards, print the hallway start goind above the room
      else if (mvinch(room->door.y - 1,room->door.x) != '.' || mvinch(room->door.y - 1,room->door.x) != '#') {
        room->door.y -= 1;
        return 0;
      }
    }


  } else if (axis == 'y') {

    if (mvinch(room->door.y + move,room->door.x) == '#') {

      // if the exit is downwards, print the hallway start going below the room
      if(mvinch(room->door.y,room->door.x + 1) != '.' && mvinch(room->door.y,room->door.x + 1) != '#') { 
        room->door.x += 1;
        return 0;
      } // else if the exit is upwards, print the hallway start goind above the room
      else if (mvinch(room->door.y,room->door.x - 1) != '.' || mvinch(room->door.y,room->door.x - 1) != '#') {
        room->door.x -= 1;
        return 0;
      }

    }
  }
  return 1;
}



// -------------------------------------
// Distances calculation functions below

int calculateDistanceRooms(NormalRoom * newRoom, NormalRoom * room) {
  int xDistance = fabs((double) newRoom->door.x - (double) room->door.x);
  int yDistance = fabs((double) newRoom->door.y - (double) room->door.y);

  return xDistance + yDistance;
}

int calculateNewDistance(Position * newDoor, Position * door, int n, char axis) {
  if (axis == 'y') {
    int xDistance = fabs((double) newDoor->x - (double) door->x);
    int yDistance = fabs((double) newDoor->y - ((double) door->y + n));

    return xDistance + yDistance;
  } else if (axis == 'x') {
    int xDistance = fabs((double) newDoor->x - ((double) door->x + n));
    int yDistance = fabs((double) newDoor->y - (double) door->y);

    return xDistance + yDistance;
  }
  return 1;
}

int calculateDistanceXAxis(Position * newDoor, Position * door, int n) {
  return fabs((double) newDoor->x - ((double) door->x + n));
}

int calculateDistanceYAxis(Position * newDoor, Position * door, int n) {
  return fabs((double) newDoor->y  - ((double) door->y + n));
}