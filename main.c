#include <stdio.h>
#include <time.h>
#include <string.h>
#include "setup.h"
#include "Nokia5110.h"
#include "gvge.h"
#include "gvge_clock.h"
#include "gvge_ant.h"
#include "graphics0.h"

#define UART_INSTR__CLOCK_MAIN_MODE 0
#define UART_INSTR__CLOCK_UPDTIME_MODE 1

uint8_t REFRESH_SCREEN_COUNTER;
uint8_t REFRESH_SCREEN;
uint8_t REFRESH_RATE_MS = 100; // 30 FPS -> 33

// --- UART INSTRUCTIONS --- //
char UART_ENTER_CHAR[] = "_";
char UART_INSTR__UPDTIME[] = "updtime";
char UART_INSTR__RETURN[] = "return";
char UART_INSTR__THEME[] = "theme";

BUTTON* temp;

void tempUpdate(BUTTON* val){
	temp = val;
}

uint64_t TIMERAW = 15000000;
uint16_t CNTR = 0;
uint8_t ONESEC = 0;

void SysTick_Handler(void){
	if(++REFRESH_SCREEN_COUNTER>REFRESH_RATE_MS){
		REFRESH_SCREEN_COUNTER=0;
		REFRESH_SCREEN=1;
	}
	if(++CNTR>1000){
		CNTR=0;
		ONESEC=1;
	}
}

time_t time(time_t *timer){
	return (time_t)++TIMERAW;
}

void GVGE_CLOCK__MAIN(){
	char time_wdm[30];
	char time_year[30];
	char time_hms[30];
	time_t mytime;
	struct tm * timeinfo;
	uint8_t MODE = 0;
	uint8_t THEME = 0;
	
	uint8_t e = 0;
	char buffer[50];
	while(1){
		if ((UART0_FR_R & UART_FR_RXFE)==0){
			buffer[e] = UART0_DR_R&0xFF;
			e++;
			if (MODE) CHAR_TX('>');
			STRING_TX((uint8_t *)buffer);
			CHAR_TX('\n');
		}
		
		if (strstr(buffer, UART_ENTER_CHAR) != NULL) {
			if (strstr(buffer, UART_INSTR__UPDTIME) != NULL) {
				MODE = UART_INSTR__CLOCK_UPDTIME_MODE;
				CHAR_TX('>');
				memset(buffer,0,strlen(buffer));
				e=0;
			}
		}
		
		if (strstr(buffer, UART_ENTER_CHAR) != NULL) {
			if (strstr(buffer, UART_INSTR__THEME) != NULL) {
				STRING_TX((uint8_t*)"Theme updated.\n");
				THEME = ~(THEME);
				memset(buffer,0,strlen(buffer));
				e=0;
			}
		}
		
		if (MODE==UART_INSTR__CLOCK_UPDTIME_MODE){
			if (strstr(buffer, UART_ENTER_CHAR) != NULL) {
				int DAY_BUFFER, MONTH_BUFFER, YEAR_BUFFER, HOUR_BUFFER, MINUTE_BUFFER, SECOND_BUFFER;
				sscanf(buffer,"%d %d %d %d %d %d",&DAY_BUFFER,
																					&MONTH_BUFFER,
																					&YEAR_BUFFER,
																					&HOUR_BUFFER,
																					&MINUTE_BUFFER,
																					&SECOND_BUFFER);
				struct tm tm = {.tm_sec		=	SECOND_BUFFER,
												.tm_min		=	MINUTE_BUFFER,
												.tm_hour	=	HOUR_BUFFER,
												.tm_mday	=	DAY_BUFFER,
												.tm_mon		=	MONTH_BUFFER-1,
												.tm_year	=	YEAR_BUFFER-1900,
												.tm_isdst =	-1};
				time_t t = mktime(&tm);
				TIMERAW = t;
				memset(buffer,0,strlen(buffer));
				e=0;
				MODE = UART_INSTR__CLOCK_MAIN_MODE;
			}
		}

		if (ONESEC==1){ 
			Nokia5110_Clear(); Nokia5110_ClearBuffer();
			ONESEC=0;
			mytime = time(NULL);
			timeinfo = localtime(&mytime);
			strftime(time_hms,30,"%H:%M:%S ",timeinfo);
			strftime(time_year,30,"%Y",timeinfo);
			strftime(time_wdm,30,"%a %d %b",timeinfo);
			text(3,3,(const unsigned char*)time_wdm,1);
			text(3,11,(const unsigned char*)time_year,1);
			text(3,19,(const unsigned char*)time_hms,1);
			if (THEME) GVGE__SCREEN_INVERT();
			Nokia5110_DisplayBuffer();
		}
	}
}

void GVGE_ANT__MAIN(void){
	uint8_t display[W][H];
	GVGE_ANT__INITDISPLAY(display);
	
	ANT ant0 = ANT__create(15,15);
	ANT__draw(ant0,display);
	
	while(1){
		if (ONESEC==1){
			ONESEC=0;
			Nokia5110_Clear(); Nokia5110_ClearBuffer();
			ANT__update(ant0,display);
			GVGE_ANT__SHOWDISPLAY(display);
			Nokia5110_DisplayBuffer();
		}
	}
}

int main(){
		Nokia5110_Init();
	Nokia5110_ClearBuffer();	
  SETUP_ADC();
	SETUP_UART0();	
	SETUP_PORTD();
	SETUP_PORTF();	
	TIMER_1MS();
	
	uint8_t SELECTED_BTN_INDEX = 0;
	uint8_t SELECTED_BTN_ROW_INDEX = 0;
	
	BUTTON CLOCK_BTN = BTN__create(0,0,11,11,1,0,1,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CALC_BTN = BTN__create(11,0,11,11,0,0,1,CALC_ICON,GVGE_CLOCK__MAIN);
	BUTTON ANT_BTN = BTN__create(22,0,11,11,0,0,1,ANT_ICON,GVGE_ANT__MAIN);
	BUTTON BTN_ARR0[3] = {CLOCK_BTN,CALC_BTN,ANT_BTN};
	
	BUTTON CLOCK_BTN1 = BTN__create(0,11,11,11,0,0,0,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CLOCK_BTN2 = BTN__create(11,11,11,11,0,0,0,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON CLOCK_BTN3 = BTN__create(22,11,11,11,0,0,0,CLOCK_ICON,GVGE_CLOCK__MAIN);
	BUTTON BTN_ARR1[3] = {CLOCK_BTN1,CLOCK_BTN2,CLOCK_BTN3};
	
	BUTTONROW BTN_ROW0 = BTN_ROW__create(BTN_ARR0);
	BUTTONROW BTN_ROW1 = BTN_ROW__create(BTN_ARR1);
	
	BUTTONROW BTN_ROW_GROUP0[2] = {BTN_ROW0,BTN_ROW1};
	Nokia5110_DisplayBuffer();
	while(1){
		if (REFRESH_SCREEN){
			REFRESH_SCREEN=0;
			//Nokia5110_DisplayBuffer();
			uint8_t UART_RX;
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
						for (uint8_t i = 0; i < 2; i++){
							for (uint8_t j = 0; j < 3; j++){
								BTN__destroy(BTN_ROW__BTNS(BTN_ROW_GROUP0[i])[j]);
							}
						}
						BTN__click((temp)[SELECTED_BTN_INDEX]);
						break;
					default:
						break;
				}
			}
		}
	}
}



