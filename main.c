#include <stdio.h> // работа с файлами
#include <stdlib.h> // для exit()
#include <string.h> // для strlen()
#include <unistd.h> // для usleep()

// мои функции и процедуры

#include "core.h"
#include "main.h"

void Save_player();

void BeforeCloseProgram() {
	Save_player();
	CloseNcursesMode();
}

// отрисовка игры

int isMapCellExists(int y, int x) {
	return (x >= 0 && x < MAP_WIDTH && y >= 0 &&  y < MAP_HEIGHT);
}

void PutSymbolToConsole(int y, int x, char ch, int color) {
	if(!isMapCellExists(y, x))
		ErrorCloseProgram("выход за границы игрового поля");
	map[y][x] = ch;
	move(y, x);
	if(color != -1)
		attron(COLOR_PAIR(color));
	addch(ch);
}

void ShowMap() {
	for(int i=0; i<MAP_HEIGHT; i++)
		for(int j=0; j<MAP_WIDTH; j++) {
			char symbol = map[i][j];
			int color = COLOR_STANDARD; // был mapColors[i][j]
			
			if(symbol == SYMBOL_WALL)
				color = COLOR_WALL;
			else if(symbol == SYMBOL_PLAYER)
				color = COLOR_PLAYER;
			else if(symbol == SYMBOL_DOOR)
				color = COLOR_DOOR;
			else if(symbol == SYMBOL_OWNER)
				color = COLOR_OWNER;
			PutSymbolToConsole(i, j, symbol, color);
		}
	refresh();
}

// чужой код

void Init_player(int yLoc, int xLoc, int y, int x, char* name) {
	player.pos.x = x;
	player.pos.y = y;
	sprintf(player.name, "%s", name);
	player.locPos.x = xLoc;
	player.locPos.y = yLoc;
	memset(player.items, 0, sizeof(player.items));
}

void loc_LoadFromFile(char* fileName) {
	memset(&loc.map, ' ', sizeof(loc));
	
	FILE* file = fopen(fileName, "r");
	if(file != NULL) {
		char buffer[80];
		int line = 0;
		loc.size.x = 0;
		loc.size.y = 0;
		
		while(!feof(file)) {
			if(fgets(buffer, MAP_WIDTH, file) > 0) {
				int cnt = strlen(buffer);
				if(buffer[cnt-1] == '\n')
					cnt--;
				
				strncpy(loc.map[line], buffer, cnt);
				line++;
				if(cnt > loc.size.x)
					loc.size.x = cnt;
			}
		}
		loc.size.y = line;
		fclose(file);
	}
	else
		ErrorCloseProgram("файл уровня не найден");
}

void loc_PutOnMap() {
	memcpy(map, loc.map, sizeof(map));
}

void Save_player() {
	FILE* file = fopen(player.name, "wb");
	if(file != NULL) {
		fwrite(&player, 1, sizeof(player), file);
		fclose(file);
	}
	else
		ErrorCloseProgram("файл игрока не найден");
}

void Load_player(char* name) {
	FILE* file = fopen(name, "rb");
	if(file != NULL) {
		fread(&player, 1, sizeof(player), file);
		fclose(file);
	}
	else
		Init_player(0, 0, 5, 5, name);
}

// код второго урока с объектами

TObj* AddObj() {
	objCnt++;
	obj = realloc(obj, sizeof(*obj)*objCnt);
	return obj + objCnt - 1;
}

int player_GetItemCnt(TItem item) {
	int cnt = 0;
	int len = strlen(item.name);
	if(item.name[len-1] == '\n')
		len--;
	
	for(int i=0; i<20; i++)
		if(strncmp(item.name, player.items[i].name, len) == 0)
			cnt++;
	
	return cnt;
}

void obj_LoadFromFile(char* name) {
	objCnt = 0;
	obj = realloc(obj, 0);
	TObj* tmp;
	FILE* f = fopen(name, "rt");
	if(f) {
		while(!feof(f)) {
			tmp = AddObj();
			fgets(tmp->name, 19, f);
			fscanf(f, "%c", &tmp->oType);
			fscanf(f, "%d", &tmp->pos.x);
			fscanf(f, "%d\n", &tmp->pos.y);
			
			fgets(tmp->item_Message, 199, f);
			fgets(tmp->item_Need.name, 19, f);
			fscanf(f, "%d\n", &tmp->item_Cnt);
			
			fgets(tmp->item_Given.name, 19, f);
			int len = strlen(tmp->item_Given.name);
			if(tmp->item_Given.name[len-1] == '\n')
				tmp->item_Given.name[len-1] = '\0';
		}
		fclose(f);
	}
}

void obj_PutOnMap() {
	for(int i=0; i< objCnt; i++)
		map[obj[i].pos.y][obj[i].pos.x] = obj[i].oType;
}

int player_DeleteItem(TItem item, int delCnt) {
	int cnt = player_GetItemCnt(item);
	if(delCnt>cnt)
		return 0;
	
	int len = strlen(item.name);
	if(item.name[len-1] == '\n')
		len--;
	
	for(int i=0; i<20; i++)
		if(strncmp(item.name, player.items[i].name, len) == 0) {
			memset(player.items[i].name, 0, sizeof(player.items[i].name));
			delCnt--;
			if(delCnt < 1)
				return 1;
		}
	return -1;
}

void player_AddItem(TItem item) {
	for(int i=0; i<20; i++)
		if(player.items[i].name[0] == 0) {
			sprintf(player.items[i].name, "%s", item.name);
			return;
		}
}

void obj_StartDialog(TObj* obj) {
	if(obj == NULL)
		return;
	
	char answer;
	do {
		clear();
		attron(COLOR_PAIR(COLOR_TEXT));
		printw("%s\n", obj->name);
		
		if(obj->oType == SYMBOL_DOOR) {
			if(player_GetItemCnt(obj->item_Need) < obj->item_Cnt) {
				printw("\n%s\n", obj->item_Message);
				printw("\n[0] - Exit menu");
				refresh();
				
				scanf("%c", &answer);
			}
			else {
				printw("\nEnter the door?");
				printw("\n[1] - Yes");
				printw("\n[0] - No\n");
				refresh();
				
				scanf("%c", &answer);
				if(answer == '1') {
					player.pos.x += (obj->pos.x - player.pos.x) * 2;
					player.pos.y += (obj->pos.y - player.pos.y) * 2;
					return;
				}
			}
		}
		else if(obj->oType == SYMBOL_OWNER) {
			if(player_GetItemCnt(obj->item_Need) < obj->item_Cnt) {
				printw("\n%s\n", obj->item_Message);
				printw("\n[0] - Exit menu");
				refresh();
				
				scanf("%c", &answer);
			}
			else {
				printw("\nNeed a %s?", obj->item_Given.name);
				printw("\n[1] - Yes");
				printw("\n[0] - No\n");
				refresh();
				
				scanf("%c", &answer);
				if(answer == '1') {
					TItem item;
					sprintf(item.name, "%s", obj->item_Given.name);
					if( player_GetItemCnt(item) == 0
						&& player_DeleteItem(obj->item_Need, obj->item_Cnt) > 0)
						player_AddItem(item);
					return;
				}
			}
		}
		else
			answer = '0';
		refresh();
	}
	while(answer != '0');
}

TObj* obj_GetByXY(int y, int x) {
	for(int i = 0; i<objCnt; i++)
		if(obj[i].pos.x == x && obj[i].pos.y == y)
			return obj + i;
	
	return NULL;
}

void LoadLocation_player() {
	char buffer[100];
	sprintf(buffer, "map/map_%d_%d.txt", player.locPos.x, player.locPos.y);
	loc_LoadFromFile(buffer);
	
	sprintf(buffer, "map/obj_%d_%d.txt", player.locPos.x, player.locPos.y);
	obj_LoadFromFile(buffer);
}

void PutOnMap_player() {
	if(isMapCellExists(player.pos.y, player.pos.x))
		map[player.pos.y][player.pos.x] = SYMBOL_PLAYER;
	
	static int dx = 55;
	for(int i=0; i<20; i++)
		for(int j=0; j<20; j++) {
			char symbol = player.items[i].name[j];
			if(symbol != 0 && symbol != '\n' && isMapCellExists(i, j))
				map[i][dx + j] = symbol;
		}
	
	if(player.pos.x > loc.size.x - 1) {
		player.locPos.x++;
		LoadLocation_player();
		player.pos.x = 1;
	}
	else if(player.pos.x < 0) {
		player.locPos.x--;
		LoadLocation_player();
		player.pos.x = loc.size.x - 1;
	}
	
	if(player.pos.y > loc.size.y - 1) {
		player.locPos.y++;
		LoadLocation_player();
		player.pos.y = 1;
	}
	else if(player.pos.y < 0) {
		player.locPos.y--;
		LoadLocation_player();
		player.pos.y = loc.size.y - 1;
	}
}

// основной цикл

void GameControl() {
	POINT prevPlayerPos = player.pos;
	
	int keypress = wgetch(stdscr);
	
	switch(keypress) {
		case KEY_ESCAPE:
			CloseProgram();
		case 'a':
			player.pos.x--; break;
		case 'd':
			player.pos.x++;	break;
		case 'w':
			player.pos.y--;	break;
		case 's':
			player.pos.y++;	break;
	}
	
	if(isMapCellExists(player.pos.y, player.pos.x))
		if(map[player.pos.y][player.pos.x] != ' ') {
			if(map[player.pos.y][player.pos.x] == SYMBOL_APPLE) {
				TItem item;
				sprintf(item.name, "Apple");
				player_AddItem(item);
				usleep(2000);
			}
			
			TObj* obj = obj_GetByXY(player.pos.y, player.pos.x);
			player.pos = prevPlayerPos;
			obj_StartDialog(obj);
		}
}

int main() {
	setConsoleSize(MAP_WIDTH, MAP_HEIGHT);
	OpenNcursesMode();
	
	Load_player("Antoxa");
	LoadLocation_player();
	
	while(1) {
		loc_PutOnMap();
		obj_PutOnMap();
		
		PutOnMap_player();
		
		ShowMap();
		
		CheckSignals();
		GameControl();
		napms(GAME_DELAY_CONSTANT);
		GameControl();
	}
	
	CloseProgram();
	return 0;
}
