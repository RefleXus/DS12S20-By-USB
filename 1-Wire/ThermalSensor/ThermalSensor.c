/* Includes ------------------------------------------------------------------*/
#include "ThermalSensor.h"
/* Private typedef -----------------------------------------------------------*/
#define MAXFLAGLENGTH 4
#define SILENT 1
#define NO_SILENT 0
typedef enum {  DeviceSearch, SendDataNoRead, SendDataWithRead,
                SendDataOnlyRead, MesureTempSkipAdress, NoSuchFlag } ExecFlagDef;
typedef struct
{
  char FlagString[MAXFLAGLENGTH];
  ExecFlagDef ExecFlag;
} ExecFlagTableDef;
typedef struct
{
  uint8_t *DataForRead;
  uint8_t NumByteForRead;
  uint8_t ReadStartByte;
  uint8_t *DataToSend;
  uint8_t NumByteToSend;
  uint8_t Reset;
  uint8_t OutputOption;
} CommandToSendDef;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const ExecFlagTableDef ExecFlagTable[4] = { "-s", SendDataNoRead,
                                            "-sr", SendDataWithRead,
                                            "-r", SendDataOnlyRead,
                                            "-m", MesureTempSkipAdress};
CommandToSendDef CommandToSend = { NULL, NULL, OW_NO_READ, NULL, NULL, OW_SEND_RESET, NO_SILENT };
/* Private function prototypes -----------------------------------------------*/
int8_t Device_Search( void );
int8_t SendData( void );
int8_t MeasureTemp( int argc, const char* const* argv );
ExecFlagDef FindExecFlag(int argc, const char* const* argv);
__inline void CommandToSend_StructInit(CommandToSendDef *CommandToSend);
__inline uint8_t MemoryAllotment(void);
__inline void MemoryFill(int argc, const char* const* argv);
__inline void MemoryFree(void);
__inline uint8_t CRCCheck (void);
__inline void CheckOwStatus( uint8_t OW_Status );
__inline void PrintReadData(void);
__inline void PrintTemperature(void);
__inline uint8_t IsCommandInExecFlagTable(const char command[], const int index)
{
  return !strcmp(command,ExecFlagTable[index].FlagString);
}
/* Private functions ---------------------------------------------------------*/
 
int8_t  Temp_Exec(int argc, const char* const* argv)
{
  uint8_t OW_Status;
  ExecFlagDef ExecFlag;
  
  CommandToSend_StructInit(&CommandToSend);
  
  OW_Status = OW_ERROR;
  ExecFlag = FindExecFlag(argc--, argv++);
  
  switch(ExecFlag)
  {
    case  DeviceSearch:           OW_Status = Device_Search();
                                  break;
          
    case  SendDataOnlyRead:       CommandToSend.Reset = OW_NO_RESET;    
          
    case  SendDataWithRead:       CommandToSend.NumByteForRead = (uint8_t)atoi(argv[0]);                              
                                  argc--;  argv++;  
                                  CommandToSend.ReadStartByte = argc;
          
    case  SendDataNoRead:         CommandToSend.NumByteToSend = argc;
                                  if(MemoryAllotment() == OW_ERROR)
                                    return OW_ERROR;
                                  MemoryFill( argc, argv );
                                  OW_Status = SendData();
                                  break;

    case  MesureTempSkipAdress:   OW_Status = MeasureTemp( argc, argv );
                                  break;
    
    case  NoSuchFlag:
    default:                 	printf("Invalid flag or number of arguments\r\n");
  }
  MemoryFree();
  return OW_Status;
}

int8_t  Temp_Init(void)
{
  return OW_Init();
}

int8_t Device_Search(void)
{
  uint8_t OW_Status;
  
  OW_Status = OW_Reset();
  CheckOwStatus(OW_Status);
  
  return OW_Status;
}

int8_t SendData(void)
{
  uint8_t OW_Status = OW_ERROR;
  
  OW_Status = OW_Send(	CommandToSend.Reset,
                        CommandToSend.DataToSend,
                        CommandToSend.NumByteToSend,
                        CommandToSend.DataForRead,
                        CommandToSend.NumByteToSend,
                        CommandToSend.ReadStartByte);
  CheckOwStatus(OW_Status);

  if( OW_Status != OW_OK )
    return OW_Status;

  OW_Status = CRCCheck();

  if(CommandToSend.OutputOption == NO_SILENT)
    PrintReadData();
  
  return OW_Status;
}

int8_t MeasureTemp( int argc, const char* const* argv )
{
  uint8_t OW_Status = OW_ERROR;
  char const *CommandsArray[2]; 
  
  if(argc != 0)
  {
    printf("Must be no arguments with -m flag\r\n");
    return OW_Status;
  }
  //CommandToSend.OutputOption = SILENT;
  CommandsArray[0] = "204";
  CommandsArray[1] = "68";
  CommandToSend.NumByteToSend = 2;  
  if(MemoryAllotment() == OW_ERROR)
    return OW_ERROR;
  MemoryFill( CommandToSend.NumByteToSend, CommandsArray  );
  if(SendData() != OW_OK )
    return OW_ERROR;
  MemoryFree();

  Delay(750);
  
  CommandToSend_StructInit( &CommandToSend );
  //CommandToSend.OutputOption = SILENT;
  CommandsArray[0] = "204";
  CommandsArray[1] = "190";
  CommandToSend.NumByteToSend = 2;
  CommandToSend.NumByteForRead = 9;
  CommandToSend.ReadStartByte = 2;
  if(MemoryAllotment() == OW_ERROR)
    return OW_ERROR;
  MemoryFill( CommandToSend.NumByteToSend, CommandsArray  );
  if(SendData() != OW_OK )
    return OW_ERROR;
  PrintTemperature();  
  MemoryFree();

  return OW_Status;
}

ExecFlagDef FindExecFlag(int argc, const char* const* argv)
{
  uint8_t i;
  if(argc)
  {
    for(i = 0; i < sizeof(ExecFlagTable)/sizeof(ExecFlagTableDef);i++)
      if(  IsCommandInExecFlagTable(argv[0], i))
        return ExecFlagTable[i].ExecFlag;
    return NoSuchFlag;
  }
  else
    return DeviceSearch;
}

void CommandToSend_StructInit(CommandToSendDef *CommandToSend)
{
  CommandToSend->DataForRead = NULL;
  CommandToSend->NumByteForRead = NULL;
  CommandToSend->ReadStartByte = OW_NO_READ;
  CommandToSend->DataToSend = NULL;
  CommandToSend->NumByteToSend = NULL;
  CommandToSend->Reset = OW_SEND_RESET;
  CommandToSend->OutputOption = NO_SILENT;
}

uint8_t MemoryAllotment(void)
{
  CommandToSend.DataToSend = calloc( (CommandToSend.NumByteToSend + CommandToSend.NumByteForRead), sizeof(uint8_t) );
  if (!CommandToSend.DataToSend)
  {
    printf("Error in memory allotment\r\n");
    return OW_ERROR;
  }
  
  if( CommandToSend.ReadStartByte != OW_NO_READ )
  {
    CommandToSend.DataForRead = calloc( CommandToSend.NumByteForRead, sizeof(uint8_t) );
    if (!CommandToSend.DataForRead)
    {
      printf("Error in memory allotment\r\n");
      return OW_ERROR;
    }
  }
  return OW_OK;
}

void MemoryFill(int argc, const char* const* argv)
{  
  uint8_t i;

  if (CommandToSend.DataToSend != NULL)
  {
    for( i = 0; i < argc ; i++ )
    {
      CommandToSend.DataToSend[i]  = (uint8_t)atoi( argv[i] );
    }
    for(i = argc ; i < (argc + CommandToSend.NumByteForRead) ; i++ )
    {
      CommandToSend.DataToSend[i]  = (uint8_t)0xff;
    }
  }
}

void MemoryFree(void)
{
  free( CommandToSend.DataToSend );
  if( CommandToSend.ReadStartByte != OW_NO_READ )
    free( CommandToSend.DataForRead );
  CommandToSend.DataToSend = NULL;
  CommandToSend.DataForRead = NULL;
}

void CheckOwStatus( uint8_t OW_Status )
{
  switch (OW_Status)
  {
    case OW_OK:             //printf("Command completed successfully\r\n");
                            break;
    case OW_ERROR:          printf("Command completed with error, try again\r\n");
                            break;
    case OW_NO_DEVICE:      printf("No device on 1-wire was found\r\n");
                            break;
    default:                printf("Command completed with unknnown error, try again\r\n");
                            break;
  }
}

uint8_t CRCCheck (void)
{
  if(CommandToSend.ReadStartByte != OW_NO_READ)
    if(crc8(CommandToSend.DataForRead,CommandToSend.NumByteForRead))
    {
      printf("CRC Wrong!\n\r");
      return OW_ERROR;
    }
   return OW_OK;
}

void PrintReadData(void)
{
  uint8_t i;

  if(CommandToSend.DataForRead != NULL)
    for( i = 0; i < CommandToSend.NumByteForRead ; i++ )
      printf("Byte Number %d :%d\n\r",(i+1),CommandToSend.DataForRead[i]);
}

void PrintTemperature(void)
{
  float32_t T = 0;
  T = CommandToSend.DataForRead[0] + (CommandToSend.DataForRead[1] << 8);
  T = T * (float32_t)0.5;
  T = T - (float32_t)0.25
      +(float32_t)(16-CommandToSend.DataForRead[6])/16;
  printf("Temperature is %2.3f\r\n",  T );
}
