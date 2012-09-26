/* Includes ------------------------------------------------------------------*/
#include "ThermalSensor.h"
/* Private typedef -----------------------------------------------------------*/
#define MAXFLAGLENGTH 4
typedef enum { DeviceSearch, SendDataNoRead, SendDataWithRead, NoSuchFlag } ExecFlagDef;
typedef struct
{
	char FlagString[MAXFLAGLENGTH];
	ExecFlagDef ExecFlag;
} ExecFlagTableDef;
typedef struct
{
	uint8_t *DataForRead;
	uint8_t NumByteForRead;
	uint8_t ReadFlag;
	uint8_t *DataToSend;
	uint8_t NumByteToSend;
} CommandToSendDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const ExecFlagTableDef ExecFlagTable[2] = {"-s", SendDataNoRead,
																					 "-sr", SendDataWithRead};
CommandToSendDef CommandToSend = { NULL, NULL, OW_NO_READ, NULL, NULL };
/* Private function prototypes -----------------------------------------------*/
int8_t Device_Search( void );
int8_t SendData( int argc, const char* const* argv );
ExecFlagDef FindExecFlag(int argc, const char* const* argv);
__inline uint8_t IsCommandInExecFlagTable(const char command[], const int index)
{
	return !strcmp(command,ExecFlagTable[index].FlagString);
}
/* Private functions ---------------------------------------------------------*/
 
int8_t	Temp_Exec(int argc, const char* const* argv)
{
	uint8_t OW_Status;
	ExecFlagDef ExecFlag;
	
	OW_Status = OW_ERROR;
	ExecFlag = FindExecFlag(argc--, argv++); //Warning!!!
	
	switch(ExecFlag)
	{
		case 	DeviceSearch:				OW_Status = Device_Search();
															break;
		
		case	SendDataWithRead:		CommandToSend.ReadFlag = OW_READ;
															CommandToSend.NumByteForRead = (uint8_t)atoi(argv[0]);															
															argc--;	argv++;															
		
		case 	SendDataNoRead:			OW_Status = SendData( argc, argv );
															break;
		
		case	NoSuchFlag:
		default:									printf("Invalid flag or number of arguments\r\n");
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

int8_t SendData(int argc, const char* const* argv)
{
	uint8_t OW_Status = OW_ERROR, i;
	
	CommandToSend.NumByteToSend = argc;
	
	CommandToSend.DataToSend = calloc( CommandToSend.NumByteToSend, sizeof(uint8_t) );
	if (!CommandToSend.DataToSend)
	{
		printf("Ошибка при распределении памяти\r\n");
		return OW_Status;
	}
	
	if( CommandToSend.ReadFlag == OW_READ )
	{
		CommandToSend.DataForRead = calloc( CommandToSend.NumByteForRead, sizeof(uint8_t) );
		if (!CommandToSend.DataForRead)
		{
			printf("Ошибка при распределении памяти\r\n");
			return OW_Status;
		}
	}
	
	for( i = 0; i < CommandToSend.NumByteToSend ; i++ )
	{
		CommandToSend.DataToSend[i]  = (uint8_t)atoi( argv[i] );
	}
	
	OW_Status = OW_Send( 	OW_SEND_RESET,
												CommandToSend.DataToSend,
												CommandToSend.NumByteToSend,
												CommandToSend.DataForRead,
												CommandToSend.NumByteToSend,
												CommandToSend.ReadFlag);
	
	for( i = 0; i < CommandToSend.NumByteForRead ; i++ )
	{
		printf("Byte Number %d :%d\n\r",(i+1),CommandToSend.DataForRead[i]);
	}
	
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
	
	free( CommandToSend.DataToSend );
	if( CommandToSend.ReadFlag == OW_READ )
		free( CommandToSend.DataForRead );
	
	return OW_Status;
}
ExecFlagDef FindExecFlag(int argc, const char* const* argv)
{
	int i;
	if(argc)
	{
		for(i = 0; i < sizeof(ExecFlagTable)/sizeof(ExecFlagTableDef);i++)
			if(	IsCommandInExecFlagTable(argv[0], i))
				return ExecFlagTable[i].ExecFlag;
		return NoSuchFlag;
	}
	else
		return DeviceSearch;
}

