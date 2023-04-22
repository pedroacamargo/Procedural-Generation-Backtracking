#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef struct Position {
	int x;
	int y;
} Position;
// Position in X/Y axis for player, rooms...

typedef struct NormalRoom
{
	Position pos;
	int height;
	int width;
	Position door;
	char doorAxis; // x/y -> THIS IS JUST TO HELP IN THE HALLWAY CONNECTION
	// int loot; 1-rara 2-comum
	// int monsters;
	// int difficulty;
} NormalRoom;
// Normal room stats


#endif