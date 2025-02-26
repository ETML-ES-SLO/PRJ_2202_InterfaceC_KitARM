#ifndef STM32_LCD
#define STM32_LCD

//definition des constante

#define SIZE_MAX_LCD 23	
#define CMD_LCD_L 0x03
#define CMD_LCD_H 0x00



//--------------- prototype de fonction-----------------//

void Stm32_WriteLCD(char SerialLine[], char* Text);

void Stm32_LCD_EncodeTrame(char SerialLine[], char size_Text, char Text);

void Stm32_printf_LCD_Console(char SerialLine[]);

#endif
