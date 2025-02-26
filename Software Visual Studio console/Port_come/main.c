#include<windows.h>
#include<stdio.h>
#include"portserial.h"
#include"Stm32_LED.h"
#include"Stm32_LCD.h"
#include"Stm32_BTN.h"
#include "time.h"
#include <stdbool.h>

#define PORT_COM   "\\\\.\\COM6"    //FT230X Basic UART // attention instaler le driver FT230xs disponible sur le site web 
// remplacer COM6 par le COM detecter sur le PC

//delay en ms
void my_delay(int i) 
{
	clock_t start, end;
	start = clock();
	while (((end = clock()) - start) * 1000.0 / CLOCKS_PER_SEC <= i);
}




main()
{
	// initialisation des variable pour la demo
	int i = 0;
	int led = 4;
	int C = 9;
	char tb_char[] = "00000000000000000000";


	//-- Ouverture du la comunication --//
	OpenPortSerial(PORT_COM);
	ParametersSerialPort();
	Stm32_WriteLCD(PORT_COM, tb_char);



		while (1)
		{

			//------- CODE de TEST Pour la reception des Bouton ----//
			Stm32_Get_state_Button(PORT_COM);
			my_delay(500);


			//deode la tramme recue et affiche la trame
			decodeTram();
			//my_delay(2000);



			//---- code demo cheniard sur le LCD et les LED -----//
			for (size_t i = 0; i < 20; i++)
			{
				tb_char[i] = '-';
				tb_char[C] = '|';
				C = i;
				Stm32_WriteLCD(PORT_COM, tb_char);
			}
			for (size_t i = 0; i < 4; i++)
			{

				Stm32_LEDOn(PORT_COM, i);
				Stm32_LEDOff(PORT_COM, led);

				led = i;		
			}
			
		}
}