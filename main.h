#ifndef MAIN_H
#define MAIN_H

// карта игры

#define MAP_HEIGHT	30
#define MAP_WIDTH	80

char map[MAP_HEIGHT][MAP_WIDTH+1];

#define GAME_DELAY_CONSTANT 40

#define SYMBOL_PLAYER 	'@'
#define SYMBOL_WALL 	'#'
#define SYMBOL_DOOR 	'/'
#define SYMBOL_OWNER 	'N'
#define SYMBOL_APPLE 	'a'

#define COLOR_PLAYER 	COLOR_RED
#define COLOR_WALL 		COLOR_BLUE
#define COLOR_DOOR 		COLOR_YELLOW
#define COLOR_OWNER 	COLOR_MAGENTA

#define COLOR_STANDARD COLOR_GREEN
#define COLOR_TEXT COLOR_WHITE
#define KEY_ESCAPE 27

typedef struct {
    int x;
    int y;
} POINT;

struct {
	char map[MAP_HEIGHT][MAP_WIDTH+1];
	POINT size;
} loc;

typedef struct {
	char name[20];
} TItem;

struct {
	char name[20];
	POINT pos;
	POINT locPos;
	TItem items[20];
} player;

typedef struct {
	char name[20];
	char oType;
	POINT pos;
	char 	item_Message[200];
	TItem 	item_Need;
	int 	item_Cnt;
	TItem 	item_Given;
} TObj;

TObj *obj = NULL;
int objCnt = 0;

#endif // MAIN_H
