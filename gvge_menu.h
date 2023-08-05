#include "gvge_clock.h"
#include "graphics0.h"
#include "setup.h"
#include "gvge.h"

typedef uint8_t BYTE;

uint8_t SELECTED_BTN_INDEX = 0;
uint8_t SELECTED_BTN_ROW_INDEX = 0;
BUTTON* temp;

struct MENU_STRUCT{
	BUTTONROW* BTNGRID;
};
typedef struct MENU_STRUCT* MENU;

void tempUpdate(BUTTON* val){
	temp = val;
}

MENU GVGE_MENU__INIT(){
	BUTTON CLOCK_BTN = BTN__create(0,0,11,11,1,0,1,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CALC_BTN = BTN__create(11,0,11,11,0,0,1,CALC_ICON,GVGE_CLOCK__MAIN);
	BUTTON ANT_BTN = BTN__create(22,0,11,11,0,0,1,ANT_ICON,GVGE_CLOCK__MAIN);
	BUTTON BTN_ARR0[3] = {CLOCK_BTN,CALC_BTN,ANT_BTN};
	
	BUTTON CLOCK_BTN1 = BTN__create(0,11,11,11,0,0,1,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CLOCK_BTN2 = BTN__create(11,11,11,11,0,0,1,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CLOCK_BTN3 = BTN__create(22,11,11,11,0,0,1,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON BTN_ARR1[3] = {CLOCK_BTN1,CLOCK_BTN2,CLOCK_BTN3};
	
	BUTTONROW BTN_ROW0 = BTN_ROW__create(BTN_ARR0);
	BUTTONROW BTN_ROW1 = BTN_ROW__create(BTN_ARR1);
	
	BUTTONROW BTN_ROW_GROUP0[2] = {BTN_ROW0,BTN_ROW1};
	Nokia5110_DisplayBuffer();
	
	//CUR cur;
	
	MENU res = (MENU)malloc(sizeof(struct MENU_STRUCT));
	res->BTNGRID = BTN_ROW_GROUP0;
	return res;
}

void GVGE_MENU__MAIN(MENU menu){
			uint8_t UART_RX;
			BUTTONROW* BTN_ROW_GROUP0 = menu->BTNGRID;
			Nokia5110_DisplayBuffer();
			if (IS_THERE_A_CHAR()){
				UART_RX = CHAR_RX();
				tempUpdate(BTN_ROW__BTNS(BTN_ROW_GROUP0[SELECTED_BTN_ROW_INDEX]));
				switch(UART_RX){
					case 'a':
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						SELECTED_BTN_INDEX--;
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						CHAR_TX(SELECTED_BTN_ROW_INDEX+'0');CHAR_TX(' ');CHAR_TX(SELECTED_BTN_INDEX+'0');CHAR_TX('\n');
						break;
					case 'd':
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						SELECTED_BTN_INDEX++;
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						CHAR_TX(SELECTED_BTN_ROW_INDEX+'0');CHAR_TX(' ');CHAR_TX(SELECTED_BTN_INDEX+'0');CHAR_TX('\n');
						break;
					case 'w':
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						SELECTED_BTN_ROW_INDEX--;
						tempUpdate(BTN_ROW__BTNS(BTN_ROW_GROUP0[SELECTED_BTN_ROW_INDEX]));
						BTN__reset((temp)[SELECTED_BTN_INDEX]);
						CHAR_TX(SELECTED_BTN_ROW_INDEX+'0');CHAR_TX(' ');CHAR_TX(SELECTED_BTN_INDEX+'0');CHAR_TX('\n');
						break;
					case 's':
						BTN__reset(temp[SELECTED_BTN_INDEX]);
						SELECTED_BTN_ROW_INDEX++;
						if (SELECTED_BTN_ROW_INDEX>1){SELECTED_BTN_ROW_INDEX=1;}
						tempUpdate(BTN_ROW__BTNS(BTN_ROW_GROUP0[SELECTED_BTN_ROW_INDEX]));
						BTN__reset((temp)[SELECTED_BTN_INDEX]);
						CHAR_TX(SELECTED_BTN_ROW_INDEX+'0');CHAR_TX(' ');CHAR_TX(SELECTED_BTN_INDEX+'0');CHAR_TX('\n');
						break;
					case 'e':
						BTN__click((temp)[SELECTED_BTN_INDEX]);
					default:
						break;
				}
			}
}

