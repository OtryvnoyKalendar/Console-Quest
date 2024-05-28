#ifndef MAIN_H
#define MAIN_H

// карта игры

#define MAP_HEIGHT	30
#define MAP_WIDTH	80

char map[MAP_HEIGHT][MAP_WIDTH+1];

#define GAME_DELAY_CONSTANT 40

#define SYMBOL_PLAYER 	'@'
#define SYMBOL_WALL 	'#'

#define COLOR_PLAYER 	COLOR_RED
#define COLOR_WALL 		COLOR_BLUE

#define COLOR_STANDARD COLOR_GREEN
#define KEY_ESCAPE 27

typedef struct {
    int x;
    int y;
} POINT;

struct {
	char map[MAP_HEIGHT][MAP_WIDTH+1];
	POINT size;
} loc;

struct {
	char name[20];
	POINT pos;
	POINT locPos;
} player;

#endif // MAIN_H
