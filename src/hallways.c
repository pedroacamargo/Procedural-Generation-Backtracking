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
  // the below function takes the position of the door from the new room (the recently created) and pushes it +1 square, for hallways don't be created 
  // in the direction of the wall.
  // isInsideAWall(newRoom,1,'x');

  // debug door axis:
  // mvprintw(20,20,"%c",newRoom->doorAxis);


  int distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance

  // This is an important variable to say if the direction changed, to help in the corridors' walls construction.
  // Needs to be reseted every iteration to axisSwap = 0; 
  int axisSwap = 0; // if = 0, axis didn't changed, if = 1, axis've just changed 


  char axis = 'x';
  while (distance != 0) {
    getch();
    int xDist = calculateDistanceXAxis(&newRoom->door,&room->door,0);
    int yDist = calculateDistanceYAxis(&newRoom->door,&room->door,0);

    if (newRoom->doorAxis == 'x') {
      yDist--;
    } else {
      xDist--;
    }

    if((xDist + yDist) == 0) break;

    // THIS CODE NEEDS TO BE OPTIMIZED !!!!
    // little code just to change the axis of the hallway creation if theres a wall in the way or dist in x/y axis is 0
    if (mvinch(room->door.y,room->door.x + 1) == '#' || mvinch(room->door.y,room->door.x - 1) == '#' || xDist == 0) {
      axis = 'y';
      axisSwap = 1;
    } else if (mvinch(room->door.y + 1,room->door.x) == '#' || mvinch(room->door.y - 1,room->door.x) == '#' || yDist == 0) {
      axis = 'x';
      axisSwap = 1;
    }




    int newDistance = calculateNewDistance(&newRoom->door,&room->door,1,axis); // this will calculate the distance between the 2 doors in manhattan distance
    // 7 > 6

    // If the distance calculating the new position decreased, print the hallway pixel in this position
    if (distance < newDistance) {
      if (axis == 'x') {
        // go left

        // if there is a floor, don't create corridors
        if (isThereAFloor(room) == 0) {
          room->door.x -= 1;
          distance--;
          continue;
        } 

        // if there isn't a wall, continue the way
        if(isInsideAWall(room,-1,axis) == 1) {
          room->door.x -= 1; // go to left
        }

        // mvprintw(room->door.y-1,room->door.x, "#");
        mvprintw(room->door.y, room->door.x, "+");
        // mvprintw(room->door.y+1,room->door.x, "#");
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
  
   
    distance--;
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