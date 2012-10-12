/* Includes ------------------------------------------------------------------*/
#include "RTC_App.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_TimeTypeDef RTC_TimeStructure;
RTC_InitTypeDef RTC_InitStructure;
RTC_AlarmTypeDef  RTC_AlarmStructure;
RTC_DateTypeDef RTC_DateStructure;
/* Private function prototypes -----------------------------------------------*/
void RTC_DateShow(void);
void RTC_TimeShow(void);
/* Private functions ---------------------------------------------------------*/

uint8_t	RTC_INIT(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	PWR_BackupAccessCmd(ENABLE);
	
 	RCC_HSEConfig(RCC_HSE_ON);
 		
 	while(RCC_WaitForHSEStartUp() == ERROR)
 	{
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div8);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WriteProtectionCmd(DISABLE);
	while (RTC_WaitForSynchro() == ERROR)
	{

	}
	
	RTC_InitStructure.RTC_AsynchPrediv = 0x007C;
	RTC_InitStructure.RTC_SynchPrediv = 0x01F3F;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_WriteProtectionCmd(DISABLE);
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		
	}
	RTC_TimeStructure.RTC_Hours   = 0x00;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
	
	return 0;		
}

uint8_t RTC_Exec(int argc, const char* const* argv)
{
	
	if(argc == 0)
	{
		RTC_TimeShow();
	}
	else
	{
		if(!strcmp(argv[0],"/s"))
		{
			if(argc == 4)
			{
				RTC_TimeStructure.RTC_Hours   = strtoint(argv[1]);
				RTC_TimeStructure.RTC_Minutes = strtoint(argv[2]);
				RTC_TimeStructure.RTC_Seconds = strtoint(argv[3]);
				if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
				{
					printf("\r\n	>> !! RTC Set Time failed. !! <<\r\n");
				}
				else
				{
					printf("\r\n	>> !! RTC Set Time success. !! <<\r\n");
					RTC_TimeShow();
				}
			}
			else
			{
				printf("Incorrect argument number!\r\n");
				printf("\r\n  /s command format: \"/s Hours Minutes Seconds\"\r\n");
			}
		}
		else if(!strcmp(argv[0],"/sd"))
		{
			if(argc == 5)
			{
				RTC_DateStructure.RTC_WeekDay   = strtoint(argv[1]);
				RTC_DateStructure.RTC_Date = strtoint(argv[2]);
				RTC_DateStructure.RTC_Month = strtoint(argv[3]);
				RTC_DateStructure.RTC_Year = strtoint(argv[4]);
				RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
				if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
				{
					printf("\r\n>> !! RTC Set Date failed. !! <<\r\n");
				} 
				else
				{
					printf("\r\n>> !! RTC Set Date success. !! <<\r\n");
					RTC_DateShow();
				}
			}
			else
			{
				printf("Incorrect argument number!\r\n");
				printf("\r\n  /sd command format: \"/sd WeekDay(1-7) Date(1-31) Mounth(1-12) Year(0-99)\"\r\n");
			}
		}
		else if(!strcmp(argv[0],"/d") && (argc == 1))
		{
			RTC_DateShow();
		}
	}	
	return 0;
}

void RTC_DateShow(void)
{
  /* Get the current Date */
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  printf("\r\n============== Current Date Display ============================\r\n");
  printf("\r\n  The current date (WeekDay-Date-Month-Year) is:  %0.2d-%0.2d-%0.2d-%0.2d \r\n", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
}
void RTC_TimeShow(void)
{
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	printf("\r\n============== Current Time Display ============================\r\n");
	printf("\r\n  The current time is:  %0.2d:%0.2d:%0.2d \r\n",
							RTC_TimeStructure.RTC_Hours,
							RTC_TimeStructure.RTC_Minutes,
							RTC_TimeStructure.RTC_Seconds);
}
