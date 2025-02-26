#ifndef STM32_BTN
#define STM32_BTN
// definition des Leds
#define LED0 0x00	
#define LED1 0x01	
#define LED2 0x02	
#define LED3 0x03

//definition de octet de trame
#define HEADER 0xFF
#define CMD_BTN_L 0x04
#define CMD_BTN_H 0x00

//prototype de fonctions
Stm32_Get_state_Button(char SerialLine[]);
#endif
