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

// A(1,1) | B(2,2)

// distancex = 1 distancey = 1 | distance = 2
// newdistance -> P(1,2) = 2

void drawHallway(NormalRoom * newRoom, NormalRoom * room,WINDOW * wnd) {
  // the below function takes the position of the door from the new room (the recently created) and pushes it +1 square, for hallways don't be created 
  // in the direction of the wall.
  newRoom = changeDoorPosition(newRoom,wnd);


  int distance = calculateDistanceRooms(newRoom,room); // distance between the rooms doors in manhattan distance 
  char axis = 'x';
  while (distance != 0) {
    int xDist = calculateDistanceXAxis(&newRoom->door,&room->door,0);
    int yDist = calculateDistanceYAxis(&newRoom->door,&room->door,0);

    // little code just to change the axis of the hallway creation if theres a wall in the way or dist in x/y axis is 0
    if (mvinch(room->door.y,room->door.x + 1) == '#' || mvinch(room->door.y,room->door.x - 1) == '#' || xDist == 0) axis = 'y';
    else if (mvinch(room->door.y + 1,room->door.x) == '#' || mvinch(room->door.y - 1,room->door.x) == '#' || yDist == 0) axis = 'x';  

    int newDistance = calculateNewDistance(&newRoom->door,&room->door,1,axis); // this will calculate the distance between the 2 doors in manhattan distance
    // 7 > 6

    // If the distance calculating the new position decreased, print the hallway pixel in this position
    if (distance < newDistance) {
      if (axis == 'x') {
        // go left

        // if there isn't a wall, continue the way
        if(isInsideAWall(room,-1,axis) == 1) {
          room->door.x -= 1; // go to left
        }

        mvprintw(room->door.y, room->door.x, "+");
      } else if (axis == 'y') {
        // go up

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

        // if there isn't a wall, continue the way
        if(isInsideAWall(room,1,axis) == 1) {
          room->door.x += 1;
        }

        mvprintw(room->door.y, room->door.x, "+");
      } else if (axis == 'y') {
        // go down

        // if there isn't a wall, continue the way
        if (isInsideAWall(room,1,axis) == 1) {
          room->door.y += 1;
        }

        mvprintw(room->door.y,room->door.x,"+");
      }
    }
  
   
    if((xDist + yDist) == 0) break;
    distance--;
  }
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


// DO A FUNCTION TO PUSH +1 THE DOOR POSITION WHEN IN A GIVEN AXIS, THIS IS FOR THE HALLWAYS BE GENERATED PROPERLY
NormalRoom * changeDoorPosition(NormalRoom * newRoom,WINDOW * wnd) {
  
  if (mvwinch(wnd,newRoom->door.y + 1,newRoom->door.x) == '#' || mvwinch(wnd,newRoom->door.y - 1, newRoom->door.x) == '#') {
    if (mvwinch(wnd,newRoom->door.y,newRoom->door.x + 1) == '.') {
      newRoom->door.x -= 1;
    } else if (mvwinch(wnd,newRoom->door.y,newRoom->door.x - 1) == '.') {
      newRoom->door.x += 1;
    }
  } else if (mvwinch(wnd,newRoom->door.y,newRoom->door.x - 1) == '#' || mvwinch(wnd,newRoom->door.y,newRoom->door.x + 1) == '#') {
    if (mvwinch(wnd,newRoom->door.y + 1,newRoom->door.x) == '.') {
      newRoom->door.y -= 1;
    } else if (mvwinch(wnd,newRoom->door.y - 1,newRoom->door.x) == '.') {
      newRoom->door.y += 1;
    }
  }
  
  return newRoom;
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