/* Includes ------------------------------------------------------------------*/
#include "ThermalSensor.h"
/* Private typedef -----------------------------------------------------------*/
#define MAXFLAGLENGTH 3
typedef enum {DeviceSearch,GetTemp,NoSuchFlag}ExecFlagDef;
typedef struct
{
	char FlagString[MAXFLAGLENGTH];
	uint8_t NumArg;
	ExecFlagDef ExecFlag;
} ExecFlagTableDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const ExecFlagTableDef ExecFlagTable[1] = {"-r", 1, GetTemp};
/* Private function prototypes -----------------------------------------------*/
int8_t Device_Search(void);
int8_t Get_Temp(int argc, const char* const* argv);
ExecFlagDef FindExecFlag(int argc, const char* const* argv);
__inline uint8_t IsCommandInExecFlagTable(const char command[], const int index)
{
	return !strcmp(command,ExecFlagTable[index].FlagString);
}
__inline uint8_t IsRightNumOfArg(const int numarg, const int index)
{
	return (uint8_t)((uint8_t)(numarg-1) == ExecFlagTable[index].NumArg);
}
/* Private functions ---------------------------------------------------------*/
 
int8_t	Temp_Exec(int argc, const char* const* argv)
{
	uint8_t OW_Status;
	ExecFlagDef ExecFlag;
	
	OW_Status = OW_ERROR;
	ExecFlag = FindExecFlag(argc, argv);
	
	switch(ExecFlag)
	{
		case 	DeviceSearch:	OW_Status = Device_Search();
												break;
		
		case 	GetTemp:			OW_Status = Get_Temp(--argc,++argv);
												break;
		
		case	NoSuchFlag:
		default:						printf("Invalid flag or number of arguments\r\n");
	}
	
	

	return OW_Status;
}

int8_t	Temp_Init(void)
{
	return OW_Init();
}

int8_t Device_Search(void)
{
	uint8_t OW_Status;
	uint8_t data[8] = {0x02, 0x1c, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xa2};
	
	OW_Status = OW_Reset();
	switch (OW_Status)
	{
		case OW_OK: 					printf("Device was found\r\n");
													break;
		case OW_ERROR: 				printf("Device search is ended with error, try again\r\n");
													break;
		case OW_NO_DEVICE: 		printf("No device on 1-wire was found\r\n");
													break;
		default: 							printf("Device search is ended with unknnown error, try again\r\n");
	}
	
	printf( "%d\r\n", crc8(data,8) );
	
	return OW_Status;
}

int8_t Get_Temp(int argc, const char* const* argv)
{
	uint8_t OW_Status = OW_ERROR;
	
	OW_Status = OW_Send(OW_SEND_RESET,"\xcc",1,NULL,NULL,OW_NO_READ);
	
	switch (OW_Status)
	{
		case OW_OK: 					printf("Command completed successfully\r\n");
													break;
		case OW_ERROR: 				printf("Command completed with error, try again\r\n");
													break;
		case OW_NO_DEVICE: 		printf("No device on 1-wire was found\r\n");
													break;
		default: 							printf("Command completed with unknnown error, try again\r\n");
	}
	
	return OW_Status;
}
ExecFlagDef FindExecFlag(int argc, const char* const* argv)
{
	int i;
	if(argc)
	{
		for(i = 0; i < sizeof(ExecFlagTable)/sizeof(ExecFlagTableDef);i++)
			if(	IsCommandInExecFlagTable(argv[0], i)		&&
					IsRightNumOfArg(argc, i))
				return ExecFlagTable[i].ExecFlag;
		return NoSuchFlag;
	}
	else
		return DeviceSearch;
}

