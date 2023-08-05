// gvge.h -- GaV Graphical Environment for Nokia 5110 LCD (84x48)
#include <stdlib.h>
#include <stdint.h>
#include "Nokia5110.h"

int GVGE__MATH_ROUND(double d);
uint8_t GVGE__DATETIME_DAYSOFMONTH(uint8_t M);
int GVGE__DATETIME_LEAPDAYS(int lo, int hi);
void GVGE__SCREEN_INVERT(void);

struct CURSOR;
typedef struct CURSOR* CUR;
CUR CURSOR__create(uint8_t x, uint8_t y);
void CURSOR__MOVE(CUR self, uint8_t dir, uint8_t FREE_OR_BTN);
uint8_t CUR__X(CUR self);
uint8_t CUR__Y(CUR self);

struct BTN;
typedef struct BTN* BUTTON;
BUTTON BTN__create(uint8_t POSX, 
							uint8_t POSY, 
							uint8_t W, 
							uint8_t H, 
							uint8_t BGCOL, 
							uint8_t FORECOL, 
							uint8_t HAS_PIC,
							uint8_t** ICON,
							void (*FX)(void));
void BTN__reset(BUTTON self);
void BTN__destroy(BUTTON self);
void BTN__click(BUTTON self);
int BTN__POSX(BUTTON self);
int BTN__POSY(BUTTON self);
int BTN__W(BUTTON self);
int BTN__H(BUTTON self);
int BTN__BGCOL(BUTTON self);
int BTN__FORECOL(BUTTON self);

struct BTN_ROW;
typedef struct BTN_ROW* BUTTONROW;
BUTTONROW BTN_ROW__create(BUTTON* BTNS);
BUTTON* BTN_ROW__BTNS(BUTTONROW self);
