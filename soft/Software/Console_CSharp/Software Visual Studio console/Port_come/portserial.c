#include<windows.h>
#include<stdio.h>
#include "time.h"
#include <stdbool.h>
static HANDLE hComm;                          // Handle to the Serial port

static BOOL   Status;

void delay_min_Comm(void)
{
	int i = 100; //100 ms 
	clock_t start, end;
	start = clock();
	while (((end = clock()) - start) * 1000.0 / CLOCKS_PER_SEC <= i);
}

void OpenPortSerial(char SerialLine[])
{


	printf("\n\n +==========================================+");
	printf("\n |  Serial Transmission -> KIT ARM	       |");
	printf("\n +==========================================+\n");
	/*----------------------------------- Opening the Serial Port --------------------------------------------*/

	hComm = CreateFileA(SerialLine,                //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("\n   Error! - Port %s can't be opened", SerialLine);
	else
		printf("\n   Port %s Opened\n ", SerialLine);
}

void ParametersSerialPort(void)
{
	DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings
	if (Status == FALSE)
		printf("\n    Error! in GetCommState()");

	dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None


	SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

		printf("\n\n    Setting DCB Structure Successfull\n");
		printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
		printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
		printf("\n       StopBits = %d", dcbSerialParams.StopBits);
		printf("\n       Parity   = %d\n\n", dcbSerialParams.Parity);
}

void SendTrame(char SerialLine[],char * Buffer,int size)
{
	/*----------------------------- Writing a Character to Serial Port----------------------------------------*/
	DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
	DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

	dNoOFBytestoWrite = sizeof(Buffer); // Calculating the no of bytes to write into the port

	Status = WriteFile(hComm,               // Handle to the Serialport
		Buffer,            // Data to be written to the port 
		size,   // No of bytes to write into the port
		&dNoOfBytesWritten,  // No of bytes written to the port
		NULL);

	delay_min_Comm();
		

}

//https://github.com/rrmhearts/Serial-Programming-Win32API-C/blob/master/USB2SERIAL_Read/Reciever%20(PC%20Side)/USB2SERIAL_Read_W32.c

/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de cr�ation : 21.05.2024
*	Derni�re modification : 21.05.2024
*
*	Entr�e:
*		buffer : pointeur vers le buffer pour stocker les donn�es re�ues
*	Sortie:
*		bool : true si la trame a �t� re�ue avec succ�s, false sinon
*
*	Description :
*	Cette fonction lit une trame de 8 caract�res depuis le port s�rie
*	et stocke les donn�es dans le buffer fourni. La fonction retourne
*	true si la lecture est r�ussie, et false en cas d'erreur.
*
*****************************************************************/

bool ReceiveTrame(char *buffer)
{
	DWORD bytesRead;
	static int charsReceived = 0; // compteur de caract�res re�us

	while (charsReceived < 8)
	{
		// Lire un caract�re depuis le port s�rie
		if (!ReadFile(hComm, buffer + charsReceived, 1, &bytesRead, NULL))
		{
			// G�rer les erreurs de lecture
			printf("Erreur lors de la lecture des donn�es s�rie.\n");
			return false;
		}

		if (bytesRead > 0)
		{
			charsReceived++;
		}
	}
	return true;
}


/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de cr�ation : 21.05.2024
*	Derni�re modification : 17.06.2024
*
*	Entr�e: -
*	Sortie: -
*
*	Description :
*	Cette fonction attend de recevoir une trame valide et d�code
*	si la trame est destin�e � un bouton ou � un capteur de temp�rature.
*	Elle affiche le type de trame re�u.
*
*****************************************************************/

void decodeTram()
{
	static char buffer[8];

	if (ReceiveTrame(&buffer))
	{
		// Afficher le bouton qui a �t� appuier
		printf("Caract�res re�us : %d\n", buffer[6]);
	}

}





