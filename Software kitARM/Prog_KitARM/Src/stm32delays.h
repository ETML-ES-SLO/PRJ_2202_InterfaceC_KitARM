/*--------------------------------------------------------*/
//	stm32delays.h
/*--------------------------------------------------------*/
//	Description :	Fonctions de delay afin
//			de pouvoir utiliser la même syntaxe
//			que sous CCS.
//
//	Auteurs 		: 	SCA
//
//	Version		:	V1.0
//	Compilateur	:	Keil 5.24
//
// Revu / modifié: 
// -
/*--------------------------------------------------------*/

#ifndef __STM32DELAYS_H
#define __STM32DELAYS_H

#include "stm32f0xx.h"	//nécessaire pour __NOP


/*--------------------------------------------------------*/
// Définition des fonctions prototypes
/*--------------------------------------------------------*/

//attention : le nop ne garantit pas un délai en architecture ARM
// (dû au pipeline du CPU)
// son utilité est plutôt du padding mémoire pour aligner le code.
#define delay_cycle() __nop()

void delay500ns(void); // nécessaire pour lcd
void delay_us(uint32_t); // 32 bits
void delay_ms(uint32_t); // 32 bits

#endif
