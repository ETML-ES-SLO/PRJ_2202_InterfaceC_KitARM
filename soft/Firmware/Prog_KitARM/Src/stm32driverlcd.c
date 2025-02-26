/*--------------------------------------------------------*/
//	stm32driverlcd.c
/*--------------------------------------------------------*/
//	Description :		Driver pour LCD STM32
//   Code basé sur driver LCD pour carte PIC32
//
//  Cible :
//   Modèle LCD NHD-C0220AA-FSW-FTW 2x20 car.
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

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "Stm32DriverLcd.h"
#include "Stm32Delays.h"


//pins de comm avec le LCD
#define LCD_DB4_Pin GPIO_PIN_8
#define LCD_DB5_Pin GPIO_PIN_9
#define LCD_DB6_Pin GPIO_PIN_10
#define LCD_DB7_Pin GPIO_PIN_11
#define LCD_E_Pin   GPIO_PIN_12
#define LCD_RW_Pin  GPIO_PIN_13
#define LCD_RS_Pin  GPIO_PIN_14
#define LCD_BL_Pin  GPIO_PIN_15

/*--------------------------------------------------------*/
// Définition du tableau pour l'adresse des lignes
/*--------------------------------------------------------*/
const uint8_t taddrLines[5] = { 0,    // ligne 0 pas utilisé
																0,    // ligne 1 commence au caractère 0
																0x40};// ligne 2 commence au caractère 64 (0x40)

/*--------------------------------------------------------*/
// Fonctions
/*--------------------------------------------------------*/
//uint8_t lcd_read_byte( void )
//{
//	uint8_t readVal;
//	//UINT8_BITS lcd_read_byte;
////  LCD_DB4_T = 1; // 1=input
////  LCD_DB5_T = 1;
////  LCD_DB6_T = 1;
////  LCD_DB7_T = 1;	
//	GPIOC->MODER = GPIOC->MODER & 0xFF00FFFF;	//mettre 4 signaux DB7..DB4 en entrée
//	GPIOC->BSRR = LCD_RW_Pin; //LCD_RW_W = 1;
//	delay500ns(); //ds0066 demande 0.5us
//	GPIOC->BSRR = LCD_E_Pin; //LCD_E_W = 1;
//	delay500ns(); //ds0066 demande 0.5us
////      lcd_read_byte.bits.b7 = LCD_DB7_R;
////      lcd_read_byte.bits.b6 = LCD_DB6_R;
////      lcd_read_byte.bits.b5 = LCD_DB5_R;
////      lcd_read_byte.bits.b4 = LCD_DB4_R;
//	readVal = (GPIOC->IDR & 0x0F00) >> 4;
//	GPIOC->BRR = LCD_E_Pin; //LCD_E_W = 0; // attention e pulse min = 500ns à 1 et autant à 0
//	delay500ns();
//	GPIOC->BSRR = LCD_E_Pin; //LCD_E_W = 1;
//	delay500ns();
////	lcd_read_byte.bits.b3 = LCD_DB7_R;
////	lcd_read_byte.bits.b2 = LCD_DB6_R;
////	lcd_read_byte.bits.b1 = LCD_DB5_R;
////	lcd_read_byte.bits.b0 = LCD_DB4_R;
//  readVal |= (GPIOC->IDR & 0x0F00) >> 8;
//	GPIOC->BRR = LCD_E_Pin; //LCD_E_W = 0;
//	delay500ns();
////  LCD_DB4_T = 0; // 0=output
////  LCD_DB5_T = 0;
////  LCD_DB6_T = 0;
////  LCD_DB7_T = 0;
//	GPIOC->MODER = GPIOC->MODER | 0x00550000; //remettre 4 signaux DB7..DB4 en sortie
//		
//	return(readVal); //return(lcd_read_byte.Val);
//}

/*--------------------------------------------------------*/
// Lecture du busy flag de l'afficheur
// Retour : 1 si occupé, 0 sinon
//
// ATTENTION : sur l'afficheur NHD-C0220AA-FSW-FTW,
// en mode de comm. 4 bits, on lit d'abord les 4 LSB, puis 
// les 4 MSB (donc le busy flag sur D7) arrive a la 2eme 
// validation de l'enable
// voir datasheet du contoleur LCD Novatek NT7605 
// version >= 2.2 (la 2.1 avait une erreur précisément au 
// check du BF en interface 4 bits).
// Il est possible que ce contrôleur NT7605 soit non 
// compatible avec le standard Hitachi 44780 au niveau
// de la lecture du BF.
bool lcd_checkbf( void )
{
	uint8_t readVal;

	GPIOC->MODER = GPIOC->MODER & 0xFF00FFFF;	//mettre 4 signaux datas DB7..DB4 en entrée
	
	GPIOC->BRR = LCD_RS_Pin; 	//ajout SCA signal RS à 0
	GPIOC->BSRR = LCD_RW_Pin; //signal RW à 1 (lecture)
	
	//1ere pulse validation sur signal E (les LSB ne sont pas lus)
	delay500ns(); //pulse enable à 1 min 300ns, période min 500ns
	GPIOC->BSRR = LCD_E_Pin; //signal Enable à 1
	delay500ns(); //pulse enable à 1 min 300ns, période min 500ns
	GPIOC->BRR = LCD_E_Pin; //signal Enable à 0
	
	//2eme pulse validation sur signal E (les MSB contenant le BF sont lus)	
	delay500ns();	//pulse enable à 1 min 300ns, période min 500ns
	GPIOC->BSRR = LCD_E_Pin;	//signal Enable à 1
	delay500ns();	//pulse enable à 1 min 300ns, période min 500ns
	readVal = (GPIOC->IDR >> 11) & 0x0001;	//check BF
	GPIOC->BRR = LCD_E_Pin; 	//signal Enable à 0
	//delay500ns();

	GPIOC->MODER = GPIOC->MODER | 0x00550000; //remettre 4 signaux DB7..DB4 en sortie
		
	return(readVal == 0x0001);
}

/*--------------------------------------------------------*/
// Envoi d'un nibble (4 bits)
void lcd_send_nibble( uint8_t n )
{
	uint16_t writeVal;
	
	//met en sortie les 4 bits à écrire
	writeVal = GPIOC->ODR;	//lecture port
	writeVal &= 0xF0FF;			//masquage bits à changer
	writeVal |= (uint16_t)n << 8;			//met nouveaux bits
	GPIOC->ODR = writeVal;	//récrit nouvelle valeur
	
	//toggle signal enable pour validation des datas à écrire
	delay500ns();	//pulse enable à 1 min 300ns, période min 500ns
	GPIOC->BSRR = LCD_E_Pin;	//signal Enable à 1
	delay500ns(); //pulse enable à 1 min 300ns, période min 500ns
	GPIOC->BRR = LCD_E_Pin;	//signal Enable à 0   
	//delay500ns();
}

/*--------------------------------------------------------*/
// Envoi d'un octet en 2 nibbles
// La ligne RS est pilotée selon paramètre "address"
// (1 pour écriture donnée, 0 pour écriture registre config
void lcd_send_byte( uint8_t address, uint8_t n )
{
	while (lcd_checkbf());	//attente lcd plus busy
	
	//pilotage ligne de ctrl RS
	if (address)
		GPIOC->BSRR = LCD_RS_Pin;	//signal RS à 1 (data)
	else
		GPIOC->BRR  = LCD_RS_Pin;	//signal RS à 0 (commande)
	
	GPIOC->BRR = LCD_RW_Pin;	//signal RW à 0 (écriture)
	//LCD_E déjà à 0
	lcd_send_nibble(n >> 4);	//écriture MSB
	lcd_send_nibble(n & 0xf);	//écriture LSB
}

/*--------------------------------------------------------*/
// init interface 4 bits selon datasheet contrôleur NT7605
// http://www.newhavendisplay.com/app_notes/NT7605.pdf
void lcd_init(void)
{
	GPIOC->MODER = (GPIOC->MODER & 0x0000FFFF) | 0x55550000; //mettre 8 signaux LCD en sortie

	//mise à 0 des signaux ctrl
	GPIOC->BRR = LCD_E_Pin;	//signal Enable à 0
	delay500ns(); // si LCD_E était à 1, on attend (pulse enable à 1 min 300ns, période min 500ns)
	GPIOC->BRR = LCD_RS_Pin;	//signal RS à 0, demandé pour une commande
	GPIOC->BRR = LCD_RW_Pin;	//signal RW à 0 (écriture)
	
	delay_ms(30);	//wait 30 ms after Vdd
	
	//function set : interface 4 bits, 2 lignes, police 5x8 px
	lcd_send_nibble(0x02); // correspond à 0x20 en interface 8 bits
	lcd_send_nibble(0x08); //
	lcd_send_nibble(0x02); // correspond à 0x20 en interface 8 bits	
	lcd_send_nibble(0x08); //
			
	delay_us(40); //wait > 40us
	
	//Display on/off control
	lcd_send_nibble(0x00); // 
	lcd_send_nibble(0x0C); // display on, cursor off, blink off 	
	//lcd_send_nibble(0x0F); // display on, cursor on, blink on 
	
	delay_us(40); //wait > 40us

	// clear display
	lcd_send_nibble(0x00); 
	lcd_send_nibble(0x01);  
	
	delay_ms(2); //wait > 1.64 ms
	
	//entry mode set
	lcd_send_nibble(0x00);  
	//lcd_send_nibble(0x02); //increment mode, entire shift off
	lcd_send_nibble(0x06); //increment mode, entire shift off
}

/*--------------------------------------------------------*/
// Positionnement du curseur ligne y, colonne x
// La numérotation commence à 1
void lcd_gotoxy( uint8_t x, uint8_t y)
{
	uint8_t address;
	address = taddrLines[y];
	address+=x-1;
	lcd_send_byte(0,0x80|address);
}

/*--------------------------------------------------------*/
//Ecriture un caractère
void lcd_putc( uint8_t c)
{
	switch (c) 
	{
    	case '\f'   : 	lcd_gotoxy(1,2);	break;  	// formfeed (nouvelle page) => début ligne 2
     	case '\n'   : 	lcd_gotoxy(1,2);    break;	//retour à la ligne => début ligne 2
     	//case '\b'   : 	lcd_send_byte(0,0x10);	break; //backspace
     	default     : 	lcd_send_byte(1,c);	break;
   }
}

/*--------------------------------------------------------*/
void lcd_put_string_ram( char *ptr_char )
{
	while (*ptr_char != 0) 
	{
    	lcd_putc(*ptr_char);
    	ptr_char++;
  	}
}

///*--------------------------------------------------------*/
//void lcd_put_string_rom( const char *ptr_char )
//{
//	while (*ptr_char != 0) 
//	{
//    	lcd_putc(*ptr_char);
//    	ptr_char++;
//  	}
//}
//
///*--------------------------------------------------------*/
//char lcd_getc( uint8_t x, uint8_t y)
//{
//	uint8_t value;
//    lcd_gotoxy(x,y);
//    while ( lcd_read_byte() & 0x80 ); // wait until busy flag is low
//    GPIOC->BSRR = LCD_RS_Pin;	//LCD_RS_W = 1;
//    value = lcd_read_byte();
//    GPIOC->BRR = LCD_RS_Pin;	//LCD_RS_W = 0;
//    return(value);
//}

/*--------------------------------------------------------*/
// Allumage backlight
void lcd_bl_on( void )
{
	GPIOC->BSRR = LCD_BL_Pin;	//LCD_BL_W = 1;
}

/*--------------------------------------------------------*/
// Extinction backlight
void lcd_bl_off( void )
{
	GPIOC->BRR = LCD_BL_Pin;	//LCD_BL_W = 0;
} 

/*--------------------------------------------------------*/
// Fonction compatible printf
// Auteur C. Huber 15.05.2014
void printf_lcd( const char *format,  ...)
{
    char Buffer[21];
    va_list args;
    va_start(args, format);

    vsprintf(Buffer, format, args);
    lcd_put_string_ram(Buffer);

    va_end(args);
}

/*--------------------------------------------------------*/
// Efface une ligne (par écriture 20 espace)
void lcd_clearLine( unsigned char NoLine)
{
    int i;
    if (NoLine >= 1 && NoLine <= 2)  {

        lcd_gotoxy( 1, NoLine) ;
        for (i = 0 ; i < 20 ; i++)
        {
            lcd_send_byte(1,0x20);
        }
     }
}
