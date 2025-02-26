/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * Canevas pour le TP microcontrôleur LedKit 
  * Cible : Kit ARM Cortex M0, carte 17400
  * 
  * Auteur /date : SCA / 18.12.2020
  * 
  * Etudiant(s) :  
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "17400.h"
#include "stm32delays.h"
#include "stm32driverlcd.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define RX_BUFFER_SIZE 260
typedef struct Rx_Setting Rx_Setting;
struct Rx_Setting
{
	uint8_t Buffer_Rx[RX_BUFFER_SIZE];
	bool flag_Rx_Uart;
	bool flag_translet_trame;
	uint16_t Size_Trame_Rx;
};



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INIT	1
#define WAIT	2
#define EXE		3

#define MESSAGE 0xFF
#define NOT_SUPPORTED 0xA0
#define ERROR_CRC 0xE0
#define NOT_UNDERSTOOD 0xC0
#define EMPTY 0x00
#define ACKNOWLEDGE_SIZE (uint16_t)5
#define MESSAGE_DEFAULT_SIZE (uint16_t)5
#define HEADER 0
#define CMDLD1 1
#define CMDLD2 2
#define DATALEN 3

#define CRC_CHECK 4
#define DATA_ONE 4
#define DATA_tWO 5
#define DATA_THREE 6
#define DATA_FOUR 7

#define GESTION_LED_ON 0x0001
#define GESTION_LED_OFF 0x0002
#define GESTION_LCD 0x0003
#define GESTION_BTN 0x0004


#define DATALED 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t BTN;

/* USER CODE BEGIN PV */
uint8_t state = INIT ;
Rx_Setting setting_Rx = {{0},0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//*******************************************************
/*Fonction qui calcule le CRC*/
//*******************************************************
uint16_t Function_CRC_check_and_create(uint8_t *data,uint16_t size)
{
	uint8_t Value_CRC = 0; //
	for (uint8_t i = 1 ; i <(size-1); i++)
	{
		Value_CRC = Value_CRC + data[i];
	}
	return (Value_CRC);
}
void Function_Acknowledge_Protocole(uint8_t message_Acknoledge)
{
	setting_Rx.Buffer_Rx[HEADER] = MESSAGE;
	setting_Rx.Buffer_Rx[CMDLD1] = setting_Rx.Buffer_Rx[CMDLD1] | message_Acknoledge;
	setting_Rx.Buffer_Rx[DATALEN] = 0;
	setting_Rx.Buffer_Rx[CRC_CHECK] = Function_CRC_check_and_create(setting_Rx.Buffer_Rx,ACKNOWLEDGE_SIZE);
	HAL_UART_Transmit(&huart1, setting_Rx.Buffer_Rx, ACKNOWLEDGE_SIZE, 100);
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,setting_Rx.Buffer_Rx,RX_BUFFER_SIZE);
}
//*******************************************************
/*Fonction qui vérifie le CRC ainsi que le Header*/
//*******************************************************
void Function_Verification_CRC_Header(void)
{
	if(setting_Rx.flag_Rx_Uart == true)
	{
		setting_Rx.flag_Rx_Uart = false;
		if (setting_Rx.Buffer_Rx[(setting_Rx.Size_Trame_Rx-1)] == Function_CRC_check_and_create(setting_Rx.Buffer_Rx,setting_Rx.Size_Trame_Rx))
		{
			if(setting_Rx.Buffer_Rx[HEADER] == MESSAGE)
			{
				setting_Rx.flag_translet_trame = true;
			}
			else
			{
				Function_Acknowledge_Protocole((uint8_t)NOT_SUPPORTED);
			}
		}
		else
		{
			Function_Acknowledge_Protocole((uint8_t)ERROR_CRC);
		}
	}
}
//*******************************************************
/*Fonction qui traduit la trame et réalise la tache demandé*/
//*******************************************************
void Function_Translate_Trame()
{
	if(setting_Rx.flag_translet_trame == true)
	{
		uint16_t Commande = 0;
		uint8_t Led = 0;
		uint8_t text[250];
		setting_Rx.flag_translet_trame = false;
		Commande = (setting_Rx.Buffer_Rx[CMDLD1]<<8)| setting_Rx.Buffer_Rx[CMDLD2];
			switch (Commande)
			{
				case GESTION_LED_ON:
				{
					Led = setting_Rx.Buffer_Rx[DATALED];
					if(setting_Rx.Buffer_Rx[DATALEN] == 1)
					{
						switch (Led)
						{
							case 0x00:
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
								
								break;
							case 0x01:
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
								
								break;
							case 0x02:
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
							
								break;
							case 0x03:
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
	
								break;
						}
							
					}
					break;
				}
				case GESTION_LED_OFF:
				{
					Led = setting_Rx.Buffer_Rx[DATALED];
					if(setting_Rx.Buffer_Rx[DATALEN] == 1)
					{
						switch (Led)
						{
							case 0x00:
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
								
								break;
							case 0x01:
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
								
								break;
							case 0x02:
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
							
								break;
							case 0x03:
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
								
								break;
						}
					}
					break;
				}
				case GESTION_LCD:
				{
						lcd_clearLine(2);
						lcd_gotoxy(1,2);
						printf_lcd("%s",&setting_Rx.Buffer_Rx[4]);
					
				}
				case GESTION_BTN:
				{
					// envoie l état des boutons
					UART_SendState_BTN(BTN);
					
		
				}
				default :
					Function_Acknowledge_Protocole((uint8_t)NOT_UNDERSTOOD);
					break;
				
			}
			
	}

	HAL_UARTEx_ReceiveToIdle_IT(&huart1,setting_Rx.Buffer_Rx,RX_BUFFER_SIZE);

	
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//variables locales
	uint8_t count_Toggle = 0;
	bool First_Time = true;
	

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	//Init LCD
	lcd_init();
	printf_lcd("Projet PORT_COMM");
	lcd_gotoxy(1,2);
	printf_lcd("Samuel Pitton");		//Subahskanth Mohanasarma
	lcd_bl_on();

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		switch (state)
	  {
	  	  case INIT:
					
					//*******************************************************
					/*Partie initilisation LED 0 clignote pendant 4sec*/ 
					//*******************************************************
	  		  count_Toggle++;
	  		  if (count_Toggle >= 10)
	  		  {
						HAL_GPIO_TogglePin(LED0_PORT,LED0_BIT);
	  			  count_Toggle = 0;
	  		  }
	  		  state = WAIT;
	  		  break;
	  	  case WAIT:
	  		  break;
	  	  case EXE:
	  		  if(First_Time == true)
	  		  {
						HAL_UARTEx_ReceiveToIdle_IT(&huart1,setting_Rx.Buffer_Rx,RX_BUFFER_SIZE); // On active l'interuption de l'uart.
						HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);	// LED0 éteint
						First_Time = false ;
	  		  }
					Function_Verification_CRC_Header(); // Fonction qui permet de verifier le CRC
					Function_Translate_Trame();	//Traduction de la trame et effectue la tache.

					
					
					
					
				//envoie des donées test
				//UART_SendData(datasend,sizeof(datasend));			///////////////////////////////////////////////////////////////////////////////////////////

			  state = WAIT;
	  }
		
		//lecture des état des boutons cycliquement et sauvegade dans BTN
		BTN = Gestion_switch();
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		

	
		
		
		//*** DELAI GLOBAL DANS LA BOUCLE (LA SEULE ATTENTE) ***

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim6)
{
	static int count_ms = 0;
	count_ms++;
	if (count_ms%50 == 0)										
	{
		if (count_ms < 4000)
		{
			state = INIT; 
		}
		else
		{
			state = EXE; // On va dans le etat execution tout les 50ms
			count_ms = 3999;
		}
	}
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart1, uint16_t Size)
{
	setting_Rx.flag_Rx_Uart = true; // Flag reception de message
	setting_Rx.Size_Trame_Rx = Size; // taille du message

}

/*****************************************************************
*
*	Auteur : Samuel Pitton
*	Date de création : 14.05.2024
*	Dernière modification : 17.06.2024
*
*	Entrée: 
*		data : pointeur vers les données à envoyer
*		size : taille des données à envoyer
*	Sortie: -
*
*	Description :
*	Cette fonction envoie des données via UART en utilisant la fonction 
*	HAL_UART_Transmit.
*
*****************************************************************/

void UART_SendData(uint8_t *data, uint16_t size) 													
{
	
    HAL_UART_Transmit(&huart1, data, size, HAL_MAX_DELAY);
}

/////////////////////// GESTION de Bouton /////////////////////////
/*****************************************************************
*
*	Auteur : Samuel Pitton 
*	Date de création : 14.05.2024
*	Dernière modification : 17.06.2024
*
*	Entrée: -
*	Sortie: 
*		uint8_t : état du bouton détecté
*
*	Description :
*	Cette fonction récupère les valeurs des switchs, détecte les changements
*	et détermine l'état du bouton (appuyé ou non) pour chaque switch.
*
*****************************************************************/

uint8_t Gestion_switch(void)
{
	//initialisation des variable loclae
	static uint8_t tab_Sw_prece [4]; // tableau des état des switch precedente
	uint8_t tab_Sw_actu [4]; // tableau des état des switch actuelle
	uint8_t index = 0; // index de switch
	uint8_t pin = 0; //index de pin d entré
	uint8_t i = 0 ; //index de boucle fot
	uint8_t State_BTN = 0;
	
	//recuperation des valeur des switch et sauvegarde dans le tableau actuelle
	for (pin = 0; pin < 4 ; pin++)
	{
		tab_Sw_actu [pin] = HAL_GPIO_ReadPin(GPIOC,(0x0001<<pin)); 
	}

	//gestion des mode et de la vitesse en fonction des switch 
	do
	{
		//test si il y a eu un apuis sur un des Switch
		if((tab_Sw_prece[index] != tab_Sw_actu[index]) && (tab_Sw_prece[index] !=0 ))
		{
			// gestion de la vitesse et du mode en fonction de si il y eu un apuis sur un bouton
			switch (index)
			{
				case 0: // gestion de S2
					State_BTN = CMD_S2_PUSH;
					break;
					
				case 1: // gestion de S3
				State_BTN = CMD_S3_PUSH;
					break;
					
				case 2: // gestion de S4
					State_BTN = CMD_S4_PUSH;
					break;
				case 3: // gestion de S5
					State_BTN = CMD_S5_PUSH;
					break;
				
				default:
					State_BTN = CMD_BNT_NOPUSH;
					break;
			}
		}
		//incrementation de index
		index++;
	} while (index < 4);
	
	//mise a jour de la valeur precedente des switchs
	for (i = 0; i < 4 ; i++)
	{
		 tab_Sw_prece[i]= tab_Sw_actu[i];
	}
	return (State_BTN);
}


/*****************************************************************
*
*	Auteur : Samuel Pitton 
*	Date de création : 14.05.2024
*	Dernière modification : 17.06.2024
*
*	Entrée: 
*		btn : état du bouton à envoyer
*	Sortie: -
*
*	Description :
*	Cette fonction formate et envoie une trame via UART pour indiquer 
*	l'état d'un bouton spécifique.
*
*****************************************************************/

void UART_SendState_BTN(uint8_t btn)
{
    char Buffer[5];
    uint8_t size = 1;  // La taille des données spécifiques à la commande (ici, 1 octet pour 'btn')
    uint8_t CRC_BTN = (HEADER_SEND + CMD_BTN_INDICATOR + size + btn);
    
    // Formatage de la trame
    uint16_t length = sprintf(Buffer, "%c%c%c%c%c%c%c%c",HEADER,HEADER,HEADER,HEADER, CMD_BTN_INDICATOR, size, btn, CRC_BTN);
    
    // Envoi de la trame
		UART_SendData((uint8_t*)Buffer, length); 	
	
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
