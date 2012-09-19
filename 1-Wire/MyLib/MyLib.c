/* Includes ------------------------------------------------------------------*/
#include "MyLib.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t strtoint(const char* s1)
 {
	 int32_t temp,dec_mul,i;
	 dec_mul = 1;
	 temp = 0;
	 for(i = strlen(s1)-1; i >= 0; i--)
	 {
		 temp += dec_mul*((uint8_t)*(s1 + i)-0x30);
		 dec_mul *= 10;
	 }
	 return temp;
 }
 
 uint8_t	crc8(const uint8_t *data, const uint8_t len)
 {
	 uint8_t i, j, crc = 0;
	 for(i = 0; i < len; i++)
	 {
		 crc ^= data[i];
		 for(j = 0; j < 8; j++)
			 crc = (crc & 0x01) ? (crc >> 1) ^ 0x8c : crc >> 1;
	 }
	 return crc;
 }

