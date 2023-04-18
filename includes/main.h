#ifndef __MAIN_H__
#define __MAIN_H__

#include "imports.h"

void drawRoom(NormalRoom * room);
NormalRoom * createNormalRoom(int *rows, int *cols);
NormalRoom * randomizePosition(WINDOW * wnd,NormalRoom * room, int col, int row, int first, int iterations);
int checkPos(NormalRoom * room, int randomY, int randomX, WINDOW * wnd, int cols, int rows);

#endif