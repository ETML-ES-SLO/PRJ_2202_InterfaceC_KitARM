#ifndef PORT_SERIAL
#define PORT_SERIAL

#include <stdbool.h>


void OpenPortSerial(char SerialLine[]);
void ParametersSerialPort(void);
void SendTrame(char SerialLine[], char* Buffer,int size);
void delay_min_Comm(void);

bool ReceiveTrame(char* buffer);
void decodeTram();


#endif