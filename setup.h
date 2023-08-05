#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void SETUP_ADC(void);
bool IS_CONVERSION_DONE(void);
uint16_t GET_DATA(void);

void SETUP_PORTF(void);
void SETUP_PORTD(void);
void TIMER_1MS(void);

void SETUP_UART0(void);
uint8_t IS_THERE_A_CHAR(void);
uint8_t CHAR_RX(void);
void CHAR_TX(uint8_t CHARACTER);
void STRING_TX(uint8_t STRING[]);

