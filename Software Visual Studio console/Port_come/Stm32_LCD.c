/*****************************************************************
*	
*	Nom du fichier : Stm32_LCD.c
*	Auteur : Samuel Pitton 
*	date de création: 05.03.2024
*	Description:
*	se fichier regroupe les fonction permettant d'écrire sur le LCD
*
******************************************************************/

//------------------- librairy standar ---------------------------// 

#include<windows.h>
#include<stdio.h>
#include<string.h>

//------------------- librairy Locale ---------------------------// 

#include"portserial.h"
#include"Stm32_LCD.h"
#include"Stm32_LED.h"

//--------------- Déclaration des constantes--------------------// 

//on veras si besoin


//------------------------- Fonction  --------------------------// 




/*****************************************************************
*
*	Auteur :Subahskanth Mohanasarma, Samuel Pitton
*	Date de création : 05.03.2024
*	Dernière modification : 05.03.2024
*
*	Entrée: descripteur de la comunication serie,
*	taille de la chaine de caratère, chaine de caratère 
*	Sortie: -
*
*	Description :
*	Cette fonction encode une trame pour ecrire sur le LCD puis l envoi.
*
*****************************************************************/
void Stm32_LCD_EncodeTrame(char SerialLine[], char size_Text, char *Text)
{
	char CRC = 0;
	char Buffer[250] = { 0 };
	int size_Buffer = 0;
	int i = 0;

	// Encodage de la trame
	sprintf(Buffer, "%c%c%c%c%s", HEADER,CMD_LCD_H, CMD_LCD_L, size_Text, Text);
	// Calcul du CRC
	for (i = 1; (i < (3 + (size_Text))); i++)
	{
		CRC = CRC + Buffer[i];
	}
	//Mise a joure de la trame
	size_Buffer = 5 + size_Text;
	// on ajoute le CRC dans la trame
	Buffer[size_Buffer - 1] = CRC;
	// Envoie la trame
	SendTrame(*SerialLine, Buffer, size_Buffer);

}


/*****************************************************************
* 
*	Auteur :Subahskanth Mohanasarma, Samuel Pitton
*	Date de création : 05.03.2024
*	Dernière modification : 05.03.2024
*	
*	Entrée: descripteur de la comunication série.
*	Sortie: -
* 
*	Description : 
*	Cette fonction envoie un message taper dans la console
*	vers l'écran LCD à travers une connexion série.	
* 
*****************************************************************/

void Stm32_printf_LCD_Console(char SerialLine[])
{
	//varibale locale
	char Text[250] = { 0 };
	char size_Text;

	// recuperation du clavier 
	fgets(Text, sizeof(Text), stdin); 
	size_Text = strlen(Text) + 1;// +1 prise en compte du char de fin \0

	//test si la taile de la phrase et trop grande
	while(size_Text >= SIZE_MAX_LCD) 
	{
		//message d erreur et demande a l utilisateur de changer sont text
		printf("\n tail du LCD depassé vous avez le droit a %d de charatere \n vous avez actuellement %d charatere\n"
			, (SIZE_MAX_LCD-3), (size_Text-2));

		printf("entrez la nouvelle chaine\n");

		// recuperation du clavier 
		fgets(Text, sizeof(Text), stdin);
		// +1 prise en compte du char de fin \0
		size_Text = strlen(Text) + 1; 
	}
	// envoi le texet
	Stm32_LCD_EncodeTrame(&SerialLine, size_Text, &Text[0]);
}

/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de création : 05.03.2024
*	Dernière modification : 05.03.2024
*
*	Entrée: descripteur de la comunication série,
*	chaine de caratère a envoier
*	Sortie: -
*
*	Description :
*	Cette fonction envoie la chaine de caratère sur le LCD
*
*****************************************************************/
void Stm32_WriteLCD(char SerialLine[], char *Text)
{
	char size_Text;

	//recuperation de la taile du texte
	size_Text = strlen(Text) + 1;

	Stm32_LCD_EncodeTrame(&SerialLine, size_Text, Text);

}