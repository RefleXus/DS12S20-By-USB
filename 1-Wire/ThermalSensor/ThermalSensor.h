/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __THERMALSENSOR_H
#define __THERMALSENSOR_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "OneWire.h"
#include "MyLib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int8_t	Temp_Exec(int argc, const char* const* argv);
int8_t	Temp_Init(void);
#endif
