#include "gvge_ant.h"

int SEED = 13874;
const int W = 84;
const int H = 48;

struct Ant{
	uint8_t X;
	uint8_t Y;
};

typedef struct Ant* ANT;

ANT ANT__create(int x, int y){
	ANT temp = (ANT)malloc(sizeof(struct Ant));
	temp->X = x;	temp->Y = y;
	return temp;
}

int ANT__x(ANT self){return self->X;}
int ANT__y(ANT self){return self->Y;}

void ANT__draw(ANT self, uint8_t scrn[W][H]){
	scrn[ANT__x(self)][ANT__y(self)] = 1;
}

void ANT__update(ANT self, uint8_t scrn[W][H]){
	scrn[ANT__x(self)][ANT__y(self)] = 0;
	int newX, newY;
	int mov[3] = {-1,0,1};
	srand(++SEED);
	newX = ANT__x(self) + mov[rand()%3];
	newY = ANT__y(self) + mov[rand()%3];
	if (newX<0) newX = 0;
	if (newY<0) newY = 0;
	scrn[newX][newY] = 1;
	self->X = (uint8_t)newX;
	self->Y = (uint8_t)newY;
}

void GVGE_ANT__INITDISPLAY(uint8_t arr[W][H]){
	for (uint8_t x = 0; x < W; x++){
	for (uint8_t y = 0; y < H; y++){
	arr[x][y]=0;}}
}

void GVGE_ANT__SHOWDISPLAY(uint8_t arr[W][H]){
	for (uint8_t y = 0; y < H; y++){
		for (uint8_t x = 0; x < W; x++){
			int display_int = (int)arr[x][y];
			if (display_int) setPixel(x,y); else clearPixel(x,y);
		}
	}
}
