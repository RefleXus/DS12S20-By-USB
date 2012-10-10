/**
  ******************************************************************************
  * @file    SysTick/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

GPIO_InitTypeDef GPIO_InitStructure;
volatile uint8_t ConsoleDataReady = 0;
volatile uint8_t argc;
char *argv[_COMMAND_TOKEN_NMB];
char commandline[_COMMAND_LINE_LEN];
/* Private function prototypes -----------------------------------------------*/
void cortexm4f_enable_fpu(void);
int CommandsExecute(int argc, const char* const* argv);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */     
       
  /* Initialize Leds mounted on STM32F4-Discovery board */
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);

	RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(SystemCoreClock / 1000);
  
  cortexm4f_enable_fpu();
		
	
	/* USB configuration */
  USBD_Init(&USB_OTG_dev,
        USB_OTG_FS_CORE_ID,
        &USR_desc, 
        &USBD_CDC_cb, 
        &USR_cb);
				
	RTC_INIT();
	Temp_Init();			
	
	
  while (1)
  {
    if(ConsoleDataReady)
        CommandsExecute( argc, argv );
    
    STM_EVAL_LEDToggle(LED3);		
		Delay(100);
		STM_EVAL_LEDToggle(LED3);
		
		STM_EVAL_LEDToggle(LED5);
    Delay(100);
		STM_EVAL_LEDToggle(LED5);
		
    STM_EVAL_LEDToggle(LED6);
    Delay(100);
		STM_EVAL_LEDToggle(LED6);

    STM_EVAL_LEDToggle(LED4);		
		Delay(100);
		STM_EVAL_LEDToggle(LED4);		

  }
}

int ConsoleExecute (int targc, const char* const* targv)
{
  
  if(!ConsoleDataReady)
  {
    ConsoleDataReady = 1;
    argc = targc;
    commandline = "";
    for( i = 0 ; i < argc; i++ )
    {
      argv[i] = strncat( commandline , targv[i]);
    }
  }
  return 0;
}

int CommandsExecute (int argc, const char* const* argv)
{

	if(argc >= 1 && (!strcmp(argv[0],"rtc")))
	{
		printf("Found allowed command %s \r\n",argv[0]);
		RTC_Exec(--argc,++argv);
	}
	else if((argc >= 1 && (!strcmp(argv[0],"temp"))))
	{
		printf("Found allowed command %s \r\n",argv[0]);
		Temp_Exec(--argc,++argv);
	}
	else
	{
		printf("Command %s is unknown \r\n",argv[0]);
	}
	ConsoleDataReady = 0;
	return 0;
}

void cortexm4f_enable_fpu() {
    /* set CP10 and CP11 Full Access */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
