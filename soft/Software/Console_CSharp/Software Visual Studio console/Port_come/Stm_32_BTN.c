/*****************************************************************
*
*	Nom du fichier : Stm32_LED.c
*	Auteur : Samuel Pitton
*	date de cr�ation: 05.03.2024
*	Description:
*	se fichier regroupe les fonction permettant la gestion des LED
*
******************************************************************/


#include<windows.h>
#include<stdio.h>
#include"portserial.h"
#include"Stm32_BTN.h"
#include"string.h"

/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de cr�ation : 04.06.2024
*	Derni�re modification : 17.06.2024
*
*	Entr�e:
*		SerialLine[] : descripteur de la communication s�rie
*	Sortie: -
*
*	Description :
*	Cette fonction envoie une trame via la ligne s�rie pour obtenir
*	l'�tat du bouton sur le module STM32.
*
*****************************************************************/
Stm32_Get_state_Button(char SerialLine[])
{

	char Buffer[250];
	int size = 0;
	char CRC = 0x00 + 0x03 + size;
	sprintf(Buffer, "%c%c%c%c%c%c", HEADER, CMD_BTN_H, CMD_BTN_L, 0x01, CRC);
	SendTrame(SerialLine, Buffer, 6);

}

