#ifndef __HALLWAYS_H__
#define __HALLWAYS_H__

void drawHallway(NormalRoom * newRoom,NormalRoom * room, WINDOW * wnd);
// Given the previous room and the actual room, connect each room

int isThereAFloor(NormalRoom * room);

int isInsideAWall(NormalRoom * room, int move, char axis);

int calculateDistanceRooms(NormalRoom * newRoom, NormalRoom * room);
// Calculate the distance between 2 rooms

int calculateNewDistance(Position * newDoor, Position * door, int n, char axis);
// Calculate the distance between 2 doors
// Axis needs to be 'y' or 'x';

int calculateDistanceXAxis(Position * newDoor, Position * door, int n);

int calculateDistanceYAxis(Position * newDoor, Position * door, int n);

#endif