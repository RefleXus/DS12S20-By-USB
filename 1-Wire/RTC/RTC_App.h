/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_APP_H
#define __RTC_APP_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx_rtc.h>
#include "MyLib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t RTC_Exec(int argc, const char* const* argv);
uint8_t	RTC_INIT(void);
#endif
