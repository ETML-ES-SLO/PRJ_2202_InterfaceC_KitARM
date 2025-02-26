#ifndef STM32_LED
#define STM32_LED
// definition des Leds
#define LED0 0x00	
#define LED1 0x01	
#define LED2 0x02	
#define LED3 0x03

//definition de octet de trame
#define HEADER 0xFF
#define	CMD_LEDON_L 0x01
#define	CMD_LEDON_H 0x00
#define CMD_LEDOFF_L 0x02
#define	CMD_LEDOFF_H 0x00


//prototype de fonctions
Stm32_LEDOn(char SerialLine[], char LED);
Stm32_LEDOff(char SerialLine[], char LED);
Stm32_LEDOff(char SerialLine[], char LED);
#endif
