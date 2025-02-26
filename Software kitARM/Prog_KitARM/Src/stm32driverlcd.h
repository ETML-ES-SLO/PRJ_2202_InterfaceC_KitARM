/*--------------------------------------------------------*/
//	stm32driverlcd.h
/*--------------------------------------------------------*/
//	Description :		Driver pour LCD STM32
//   Code basé sur driver LCD pour carte PIC32
//
//  Cible :
//   Modèle LCD NHD-C0220AA-FSW-FTW
//   Carte 17400A KitArmM0
//
//	Auteurs 		: 	SCA
//
//	Version		:	V1.0
//	Compilateur	:	Keil 5.24
//
// Revu / modifié: 
// -
/*--------------------------------------------------------*/

#ifndef __STM32DRIVERLCD_H
#define __STM32DRIVERLCD_H

#include <stdarg.h>

/*--------------------------------------------------------*/
// Définition des fonctions prototypes
/*--------------------------------------------------------*/
//uint8_t lcd_read_byte( void );
//void lcd_send_nibble( uint8_t n );
//void lcd_send_byte( uint8_t address, uint8_t n );
void lcd_init(void);
void lcd_gotoxy( uint8_t x, uint8_t y);
void lcd_putc( uint8_t c);
void lcd_put_string_ram( char *ptr_char );
//void lcd_put_string_rom( const char *ptr_char );
//char lcd_getc( uint8_t x, uint8_t y);
void lcd_bl_on( void ); 
void lcd_bl_off( void );
void printf_lcd( const char *format,  ...);
void lcd_clearLine( unsigned char NoLine);

#endif
