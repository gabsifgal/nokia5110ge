#include "gvge_clock.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "setup.h"
/*
typedef uint8_t BYTE;
uint32_t TIMERAW = 1500000;
uint16_t CNTR = 0;
uint8_t ONESEC = 0;

time_t time(time_t *timer){
	return (time_t)++TIMERAW;
}

void GVGE_CLOCK__MAIN(){
	//while(1){
		time_t mytime = time(NULL);
		char * time_str = ctime(&mytime);
		time_str[strlen(time_str)-1] = '\0';
		Nokia5110_Clear();
		text(0,0,(const unsigned char*)time_str,0);
		//}
	//}
}
*/

