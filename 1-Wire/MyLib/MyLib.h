/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYLIB_H
#define __MYLIB_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t strtoint(const char* s1);
uint8_t	crc8(const uint8_t *data, const uint8_t len);
#endif
