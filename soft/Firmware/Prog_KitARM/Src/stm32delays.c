/*--------------------------------------------------------*/
//	stm32delays.c
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

#include "stm32delays.h"


#define SYSTICK_FREQ 48000000

/*--------------------------------------------------------*/
// Fonction delay500ns
// Approximative, mais testé >= 500ns
// Testé sur STM32F051R8T7 @ 48 MHz :
//  691 ns optimisation 3
//  628 ns optimisation 0 
/*--------------------------------------------------------*/
void delay500ns(void)
{
	delay_cycle();
}

/*--------------------------------------------------------*/
// Fonction delay_us
// attention, la valeur passée en param doit être < 1000
// sinon, utiliser fonction delay_ms()
// Testé sur STM32F051R8T7 @ 48 MHz :
// environ 100 à 300 ns de surplus, optimisation 0 ou 3.
/*--------------------------------------------------------*/
void delay_us(uint32_t us)
{
	uint32_t startVal, endVal, currentVal;
	uint32_t nTick;
		
	startVal = SysTick->VAL;
	nTick = ((SYSTICK_FREQ/1000000)*(us-1));	//nbre de ticks que systick doit compter
	
	if (startVal >= nTick)	//SysTick décompte normalement ?
	{
		endVal = startVal-nTick;
		do{
			currentVal = SysTick->VAL;
		}
		while ((currentVal <= startVal) && (currentVal > endVal) );
		//on a pas fait le tour ET valeur finale pas atteinte
	}
	else	//SysTick se recycle durant le delay
	{
		endVal = startVal + SysTick->LOAD - nTick;
		do{
			currentVal = SysTick->VAL;
		}
		while ((currentVal <= startVal) || (currentVal > endVal));
		//on a pas fait le tour OU valeur finale pas atteinte
	}		
}

/*--------------------------------------------------------*/
// Fonction delay_ms
// Exacte. Basé sur Systick timer
// Délai testé sur STM32F051R8T7 @ 48 MHz, en optimisation 0 et 3
/*--------------------------------------------------------*/
void delay_ms(uint32_t ms)
{
	//Attention lire doc si appel HAL_Delay depuis interruption
	if (ms>0)
		HAL_Delay(ms-1);	//la fonction HAL_Delay() dure à chaque fois 1 ms de trop...	
}
