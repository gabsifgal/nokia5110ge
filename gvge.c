#include "gvge.h"

int GVGE__MATH_ROUND(double d){
	return d < 0 ? d - 0.10 : d + 0.5;
}

int GVGE__DATETIME_LEAPDAYS(int lo, int hi){
    if (lo > hi || lo < 1800 || hi > 9999)
        return 0;
    if (lo == hi)
        return 0;
    int lo_4 = (lo - 0) / 4;
    int hi_4 = (hi - 1) / 4;
    int diff = hi_4 - lo_4;

    int lo_c = (lo - 0) / 100;
    int hi_c = (hi - 1) / 100;
    diff -= hi_c - lo_c;

    int lo_q = (lo - 0) / 400;
    int hi_q = (hi - 1) / 400;
    diff += hi_q - lo_q;

    return(diff);
}

uint8_t GVGE__DATETIME_DAYSOFMONTH(uint8_t M){
	switch(M){
		case 0: return 31;
		case 1: return 28;
		case 2: return 31;
		case 3: return 30;
		case 4: return 31;
		case 5: return 30;
		case 6: return 31;
		case 7: return 31;
		case 8: return 30;
		case 9: return 31;
		case 10: return 30;
		case 11: return 31;
		default: return 0;
	}
}

void GVGE__SCREEN_INVERT(void){
	for (uint8_t x = 0; x < 84; x++)
		for (uint8_t y = 0; y < 48; y++)
			invertPixel(x,y);
}

// --- CURSOR --- ///
struct CURSOR{
	uint8_t x; 
	uint8_t y;
};

CUR CURSOR__create(uint8_t x, uint8_t y){
	CUR cur = (CUR)malloc(sizeof(struct CURSOR));
	cur->x = 0;
	cur->y = 0;
	return cur;
}

uint8_t CUR__X(CUR self){return self->x;}
uint8_t CUR__Y(CUR self){return self->y;}


void CURSOR__MOVE(CUR self, uint8_t dir, uint8_t FREE_OR_BTN){
	switch (dir){
		case 'w': self->y--; break;
		case 's': self->y++; break;
		case 'a': self->x--; break;
		case 'd': self->x++; break;
		default: break;
	}
	if (self->x>84){self->x=84;}
	if (self->y>48){self->y=48;}
}
							
// --- BUTTON --- ///

struct BTN{
	uint8_t POSX;
	uint8_t POSY;
	uint8_t W;
	uint8_t H;
	uint8_t BGCOL;
	uint8_t FORECOL;
	uint8_t HAS_PIC;
	uint8_t** ICON;
	void (*FX)(void);
};

void BTN__init(BUTTON self, 
							uint8_t POSX, 
							uint8_t POSY, 
							uint8_t W, 
							uint8_t H, 
							uint8_t BGCOL, 
							uint8_t FORECOL, 
							uint8_t HAS_PIC,
							uint8_t** ICON,
							void (*FX)(void)){
	self->POSX = POSX;
	self->POSY = POSY;
	self->W = W;
	self->H = H;
	self->BGCOL = BGCOL;
	self->FORECOL = FORECOL;
	self->HAS_PIC = HAS_PIC;
	self->ICON = ICON;
	self->FX = FX;
	
	if ((POSX<=84)&&(POSY<=48)){
		drawRectangle(POSX,POSY,POSX+W,POSY+H);
		if (HAS_PIC){
			drawBitmap(POSX+2, POSY+2, ICON[0], sizeof(ICON)/8, 8);
		}
	}
	if (BGCOL){
		for (uint8_t i = POSX+1; i < POSX+W; i++){
			for (uint8_t j = POSY+1; j < POSY+H; j++){
				invertPixel(i,j);
			}
		}
	}
}

BUTTON BTN__create(uint8_t POSX, 
							uint8_t POSY, 
							uint8_t W, 
							uint8_t H, 
							uint8_t BGCOL, 
							uint8_t FORECOL, 
							uint8_t HAS_PIC,
							uint8_t** ICON,
							void (*FX)(void)){
	BUTTON res = (BUTTON)malloc(sizeof(struct BTN));
	BTN__init(res,POSX,POSY,W,H,BGCOL,FORECOL,HAS_PIC,ICON,FX);
	return res;
}

void BTN__reset(BUTTON self){
	clearFilledRectangle(
		self->POSX,
		self->POSY,
		self->POSX+self->W,
		self->POSY+self->H);
	BTN__init(self,self->POSX,self->POSY,self->W,self->H,~(self->BGCOL)&1,self->FORECOL,self->HAS_PIC,self->ICON,self->FX);
}
	
void BTN__destroy(BUTTON self){
	if (self){
		clearFilledRectangle(self->POSX,self->POSY,self->POSX+self->W+1,self->POSY+self->H+1);
		free(self);
	}
}

void BTN__click(BUTTON self){
	self->FX();
}

int BTN__POSX(BUTTON self){return self->POSX;}
int BTN__POSY(BUTTON self){return self->POSY;}
int BTN__W(BUTTON self){return self->W;}
int BTN__H(BUTTON self){return self->H;}
int BTN__BGCOL(BUTTON self){return self->BGCOL;}
int BTN__FORECOL(BUTTON self){return self->FORECOL;}
int BTN__HAS_PIC(BUTTON self){return self->HAS_PIC;}


// --- BUTTON ROW --- //

struct BTN_ROW{
	BUTTON* BTNS;
};

BUTTONROW BTN_ROW__create(BUTTON* BTNS){
	BUTTONROW br = (BUTTONROW)malloc(sizeof(struct BTN_ROW));
	br->BTNS = BTNS;
	return br;
}

BUTTON* BTN_ROW__BTNS(BUTTONROW self){return self->BTNS;}

// --- WINDOW --- //
