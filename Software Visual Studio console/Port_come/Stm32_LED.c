/*****************************************************************
*
*	Nom du fichier : Stm32_LED.c
*	Auteur : Samuel Pitton
*	date de création: 05.03.2024
*	Description:
*	se fichier regroupe les fonction permettant la gestion des LED
*
******************************************************************/


#include<windows.h>
#include<stdio.h>
#include"portserial.h"
#include"Stm32_LED.h"
#include"string.h"



/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de création : 05.03.2024
*	Dernière modification : 17.06.2024
*
*	Entrée:
*		SerialLine[] : descripteur de la communication série,
*		LED : identifiant de la LED à allumer
*	Sortie: -
*
*	Description :
*	Cette fonction envoie une trame via la ligne série pour allumer
*	une LED spécifique sur le module STM32.
*
*****************************************************************/
Stm32_LEDOn(char SerialLine[],char LED)
{
	char Buffer[250];
	int size = 0;
	char CRC = 0x00 + 0x01 + 0x01 + LED;
	sprintf(Buffer, "%c%c%c%c%c%c", HEADER, CMD_LEDON_H, CMD_LEDON_L,0x01,LED, CRC);
	SendTrame(SerialLine, Buffer,6);
}

/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de création : 05.03.2024
*	Dernière modification : 17.06.2024
*
*	Entrée:
*		SerialLine[] : descripteur de la communication série,
*		LED : identifiant de la LED à éteindre
*	Sortie: -
*
*	Description :
*	Cette fonction envoie une trame via la ligne série pour éteindre
*	une LED spécifique sur le module STM32.
*
*****************************************************************/
Stm32_LEDOff(char SerialLine[], char LED)
{

	char Buffer[250];
	int size = 0;
	char CRC = 0x00 + 0x03+ size + LED;
	sprintf(Buffer, "%c%c%c%c%c%c", HEADER, CMD_LEDOFF_H, CMD_LEDOFF_L, 0x01, LED, CRC);
	//size = strlen(Buffer) - 1;
	SendTrame(SerialLine, Buffer, 6);

}








