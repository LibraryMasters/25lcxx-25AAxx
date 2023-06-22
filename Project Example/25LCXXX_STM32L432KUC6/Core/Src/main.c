/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "25LCxx_driver_basic.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
E_25LCxx_info_t e_25LCxxInfo;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/**
 * @brief demo state machine definition
 * */
typedef enum
{
	WRITE_BYTE         = (int)1,
	READ_BYTE          = (int)2,
	EE_PUT             = (int)3,
	EE_GET             = (int)4,
	EE_UPDATE          = (int)5,
	PAGE_ERASE         = (int)6,
	SECTOR_ERASE       = (int)7,
	SET_BLOCK_PROTECT  = (int)8,
	CHIP_ERASE         = (int)9,
	READ_ENTIRE_MEMORY = (int)10,
	DEMO_IDLE          = (int)0

}e25lcxx_demo_state_t;

typedef struct handle_s
{
	e25lcxx_demo_state_t state;
}e25lxx_state_handler_t;

e25lxx_state_handler_t demo;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t spi_write(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len);
uint8_t spi_read(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len);
void uart_transmit(const char *pString, uint8_t u8Length);
uint8_t spi_write_gpio_cs(uint8_t u8State);

uint16_t u16EE_addr;
uint8_t pBufferWrite[50];
uint8_t pBufferRead[10];
uint32_t u32DateAndTime;
uint16_t u16AddrIndex;
uint8_t u8PreviousState, u8PreviousState1;
uint16_t u16BufferLength;


uint8_t tempBuffer[10] = {0x00, 0x12, 0x54, 0x46, 0x95, 0xA5, 0xb7, 0xc3, 0xF4, 0x2D};

/** EEPROM 25LCxx application definition */
uint8_t pDataRead[20];					                    /**< buffer to hold data read */
const uint8_t pDataWrite[10] = "cedric";						/**< buffer to hold data to write */
uint8_t numByteRead;										/**< number of bytes to read/write */
uint8_t singleByteDataRead;									/**< data to store single byte data read */
uint16_t dataAddress;										/**< eeprom address (read/write)*/
uint32_t byteGet;
uint32_t dateTime = 2305031413;
float floatData = 45.5f;
int err;
uint16_t totalSize, freeSpace, usedSpace;

/**end of global definitions*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   demo.state = DEMO_IDLE;
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  e_25LCxx_basic_initialize(E_25LC160x_VARIANT, E_25LCXX_PAGE_SIZE_16_BYTE);
  E_25LCxx_info(&e_25LCxxInfo);

  e_25LCxx_interface_debug_print("Chip Name: \t%s\r\n", e_25LCxxInfo.chip_name);
  e_25LCxx_interface_debug_print("Manufacture: \t%s\r\n", e_25LCxxInfo.manufacturer_name);
  e_25LCxx_interface_debug_print("Interface: \t%s\r\n", e_25LCxxInfo.interface);
  e_25LCxx_interface_debug_print("Supply Volt Max: \t%.1fV\r\n", e_25LCxxInfo.supply_voltage_max_v);
  e_25LCxx_interface_debug_print("Supply Volt Min: \t%.1fV\r\n", e_25LCxxInfo.supply_voltage_min_v);
  e_25LCxx_interface_debug_print("Maximum Current: \t%.1fmA\r\n", e_25LCxxInfo.max_current_ma);
  e_25LCxx_interface_debug_print("Temperature Max: \t%.1fC\r\n", e_25LCxxInfo.temperature_max);
  e_25LCxx_interface_debug_print("Temperature Min: \t%.1fC\r\n", e_25LCxxInfo.temperature_min);
  e_25LCxx_interface_debug_print("Driver Version: \tV%.1f.%.2d\r\n", (e_25LCxxInfo.driver_version /1000), (uint8_t)(e_25LCxxInfo.driver_version - (uint8_t)(e_25LCxxInfo.driver_version /100)*100));

	dataAddress = 0;          /**< start read operation at eeprom address 0*/
	numByteRead = 20;		  /**< read 20 bytes */

  err = e_25LCxx_basic_get_bp_status((uint8_t *)&singleByteDataRead);
	e_25LCxx_interface_debug_print("Block protect status: %d\n", singleByteDataRead);

	err= e_25LCxx_basic_read_byte(dataAddress, (uint8_t *)pDataRead, numByteRead);
	for (int index = 0; index < numByteRead; index++){
		e_25LCxx_interface_debug_print("address read: %d Data Read :%x\n",index, pDataRead[index]);
	}

	//err = e_25LCxx_basic_update(1536,(uint8_t *)pDataWrite, sizeof(pDataWrite));

	/*for(int index = 0; index < e_25LCxx_basic_get_eeprom_legth(); ++index){
		err = e_25LCxx_basic_read_byte(index, (uint8_t *)&singleByteDataRead, 1);
		e_25LCxx_interface_debug_print(" add: %d data: 0x%x", index, singleByteDataRead);
		if(index % 10 == 0)
		e_25LCxx_interface_debug_print("\n\r");
	}*/

	e_25LCxx_basic_put_byte(14, (uint32_t *)&dateTime,  sizeof(dateTime));

	//err = e_25LCxx_basic_update(12,(uint8_t *)pDataWrite, sizeof(pDataWrite));

	err = e_25LCxx_basic_get_byte(14, (uint32_t *)&byteGet, sizeof(byteGet));
	e_25LCxx_interface_debug_print("\nGet date: %lu\n", byteGet);

	err = e_25LCxx_basic_erase_page(64);											  /**< erasing memory page 64 (address: 1008 - 1023)*/
	err = e_25LCxx_basic_erase_sector(1532, 1540);									  /**< attempt to erase a protected region */
	err = e_25LCxx_basic_write_byte(2011,(uint8_t *)pDataWrite, sizeof(pDataWrite));  /**< attempt to write block of data bigger than page size, in a protected region*/

	err = e_25LCxx_basic_get_memory_properties((uint16_t*) &totalSize, (uint16_t *)&freeSpace, (uint16_t *)&usedSpace);
	e_25LCxx_interface_debug_print("Total size : %d Bytes\nFree space: %d Bytes\nUsed space: %d Bytes\n", totalSize, freeSpace, usedSpace);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	     HAL_Delay(500);
	     HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

//    	 e_25LCxx_basic_update(0x00, (uint8_t *)tempBuffer, 10);


//	     for(int index = 0; index < e_25LCxx_basic_get_eeprom_legth(); index++)
//	     {
//	    	 e_25LCxx_basic_read_byte(index, (uint8_t *)&u8PreviousState, 1);
//	    	 if(u8PreviousState != 0xff){
//	    		 e_25LCxx_interface_debug_print("Index: %d Data: %x \n", index, u8PreviousState);
//	    	 }
//
//	     }




//	     switch((int)demo.state)
//	    		     {
//
//	    				 case DEMO_IDLE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d IDLE\r\n", DEMO_IDLE);
//	    					 if(u8PreviousState == READ_ENTIRE_MEMORY)
//	    					 {
//	    						 e_25LCxx_interface_debug_print("\t\tEND OF DEMO CODE!! :-)\r\n");
//	    					     return 1;
//	    					 }
//	    					 e_25LCxx_interface_debug_print("\t\t\t* Demo code to demonstrate basic functionalities of EEPROM Driver.\r\n");
//	    					 e_25LCxx_interface_debug_print("\t\t\t* Note that it is strongly recommanded to disable debug mode"
//	    							                        "('E_25LCXX_DEBUG_MODE' find in 25LCxx_driver.h ) in order to reduce the code size.\r\n");
//	    					 e_25LCxx_interface_debug_print("\t\t\t* Note that enabling printing of float will increase code size.\r\n");
//	    					 e_25LCxx_interface_debug_print("\t\t\t* In order to enable printing of float on STM32Cube go to:Project->properties->Miscellaneous -> other flags: creat new file and past synthix : -u _printf_float.\r\n");
//	    					 demo.state = WRITE_BYTE;       /**< go to next state to begin testing */
//	    					 break;
//	    				 }
//
//	    				 case WRITE_BYTE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d WRITE BYTE\r\n", WRITE_BYTE);
//	    					 u16EE_addr = 0;                                                        /**< define start address (write from address 0) */
//	    					 u16BufferLength = 10;                                                  /**< set buffer length (number of byte to be written) */
//	    					 if(u8PreviousState == SET_BLOCK_PROTECT){                              /**< if block protect is set, define address to write equal to the eeprom max addr - 10 */
//	    						 u16EE_addr = e_25LCxx_basic_get_eeprom_legth() - u16BufferLength;
//	    					 }
//	    					 memset(pBufferWrite, 34, u16BufferLength);                             /**< Give the 10 first byte of buffer a value of 34 */
//	    					 if(e_25LCxx_basic_write_byte(u16EE_addr, (uint8_t*)pBufferWrite, u16BufferLength)) /**< write n number of byte to last 10 address of eeprom */
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", WRITE_BYTE);
//	    						 demo.state =  DEMO_IDLE;                                           /**< start over from idle */
//	    					 }if(u8PreviousState == SET_BLOCK_PROTECT)
//	    						 demo.state = CHIP_ERASE;
//	    					 else
//	    						 demo.state =  READ_BYTE;                                          /**< move to next state if write successfull*/
//
//	    					 break;
//	    				 }
//
//	    				 case READ_BYTE:
//	    				 {
//	    					 uint8_t ret;
//	    					 e_25LCxx_interface_debug_print("STATE:%d READ BYTE \r\n", READ_BYTE);
//	    					 u16EE_addr = 5;												       /**< define start address to read (read from address 5) */
//	    					 u16BufferLength = 10;                                                 /**< set buffer length to read */
//	    					 memset(pBufferRead, 0x00, u16BufferLength);                           /**< clear buffer before read */
//	    					 ret = e_25LCxx_basic_read_byte(u16EE_addr, (uint8_t*)pBufferRead, u16BufferLength); /**< read 10 byte starting from address defined above*/
//	    					 if(ret != HAL_OK)
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", READ_BYTE);
//	    						 demo.state =  DEMO_IDLE;                                          /**< start over from idle if failed */
//	    						 return 1;														   /**< return error code */
//	    					 }else{
//	    						 for(u16AddrIndex = 0; u16AddrIndex < u16BufferLength; u16AddrIndex++){        /**< print read data */
//	    							 e_25LCxx_interface_debug_print("address: %d, data = %d\r\n", u16AddrIndex + u16EE_addr, pBufferRead[u16AddrIndex]);
//	    						 }
//	    						 if(u8PreviousState == PAGE_ERASE)                                 /**< go to sector erase if previous state was PAGE ERASE */
//	    							 demo.state = SECTOR_ERASE;
//	    						 else
//	    							 demo.state =  EE_PUT;                                         /**< move to next state if write successfull*/
//	    					 }
//
//	    					 break;
//	    				 }
//
//	    				 case EE_PUT:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d EEPROM PUT\r\n", EE_PUT);
//	    					 u16EE_addr = 15;                                                    /**< set address to start writing from */
//	    					 u32DateAndTime = 2209041513;                                        /**< prepare data to be stored as date and time this demo code was written (04/09/2022 - 15:43) */
//	    					 if(e_25LCxx_basic_put_byte(u16EE_addr, (unsigned long long *)&u32DateAndTime, sizeof(u32DateAndTime))) /**< 4 byte is needed to write date and time (from addr : 15 - 18)*/
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", EE_PUT);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }else{
//	    						 demo.state =  EE_GET;                                           /**< move to next state if read successfull*/
//	    					 }
//	    					 break;
//	    				 }
//
//	    				 case EE_GET:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d EEPROM GET\r\n", EE_GET);
//	    					 u16EE_addr = 15;                                                    /**< set address to start reading from */
//	    					 u32DateAndTime = 0x00;                                              /**< make sure date and time variable is cleared before reading */
//	    					 if(e_25LCxx_basic_get_byte(u16EE_addr, (unsigned long long *)&u32DateAndTime, sizeof(u32DateAndTime))) /**< read 4 byte starting for address defined above */
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", EE_GET);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }else{
//	     						 e_25LCxx_interface_debug_print("Time and date stored:%lu\r\n", u32DateAndTime); /**< print read data */
//	    						 if(u8PreviousState == SECTOR_ERASE){
//	    							 demo.state =  SET_BLOCK_PROTECT;                            /**< go to block protect state if previous state was sector erase */
//	    						 }  else{
//	    							 demo.state = EE_UPDATE;
//	    						 }
//
//	    					 }
//	    					 break;
//	    				 }
//
//	    				 case EE_UPDATE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d EEPROM UPDATE\r\n", EE_UPDATE);
//	    	                 u16EE_addr = 19;                                                    /**< set start address*/
//	    	                 u16BufferLength = 3;                                                /**< define buffer length (number of bytes to be updated */
//	    	                 memset(pBufferWrite, 0x52, u16BufferLength);                        /**< set first 3 byte of array to 82 */
//	    	                 if(e_25LCxx_basic_update(u16EE_addr, (uint8_t *)pBufferWrite, u16BufferLength))   /**< run update routine */
//	    	                 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", EE_UPDATE);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    	                 }else{
//	    	                	 demo.state =  PAGE_ERASE;                                        /**< move to next state if write successfull*/
//	    	                 }
//	    					 break;
//	    				 }
//
//	    				 case PAGE_ERASE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d PAGE ERASE\r\n", PAGE_ERASE);
//	    					 if(e_25LCxx_basic_erase_page(1))                                    /**< erase first page of the memory */
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", PAGE_ERASE);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }else{
//	    						 u8PreviousState = demo.state;                                   /**< keep track of previous state */
//	    						 demo.state = READ_BYTE;                                         /**< go back to read state to confirm page has been erased */
//	    					 }
//	    					 break;
//	    				 }
//
//	    				 //Note: Sector erase will erase data from start addr to end addr (255 is the default value when erasing).
//	    				 // Note: This function uses e_25LCxx_basic_update() to perform the write, so does not rewrites the value if it didn't change */
//	    				 //We will attempt to delete the stored date and time we stored in state:EE_PUT
//	    				 case SECTOR_ERASE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d SECTOR ERASE\r\n", SECTOR_ERASE);
//	    					 u16EE_addr = 15;                                                    /**< define sector start address */
//	    					 if(e_25LCxx_basic_erase_sector(u16EE_addr, (u16EE_addr + sizeof(u32DateAndTime))- 1)) /**< execute erase sector instruction from address 15 - 18*/
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", SECTOR_ERASE);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }
//	    					 u8PreviousState = SECTOR_ERASE;                                     /**< keep track of previous state */
//	    	//				 e_25LCxx_interface_debug_print("previous state %d, current state:%d\r\n",u8PreviousState,demo.state);
//	    					 demo.state =  EE_GET;                                               /**< go read if the date and time has been erased successfully */
//	    					 break;
//	    				 }
//
//	    				 case SET_BLOCK_PROTECT:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d SET BLOCK PROTECT\r\n", SET_BLOCK_PROTECT);
//	    					 if(e_25LCxx_basic_set_bp_status(E_25LCxx_BP01))                     /**< write protect the upper 25% of the memory */
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", SET_BLOCK_PROTECT);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }
//	    					 u8PreviousState = demo.state;                                       /**< keep track of previous state */
//	    					 demo.state =  WRITE_BYTE;                                           /**< back to write state and attempt to writebyte into the protected area*/
//	    					 break;
//	    				 }
//
//	    				 // Note: This function uses e_25LCxx_basic_update() to perform the write, so does not rewrites the value if it didn't change */
//	    				 case CHIP_ERASE:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d CHIP ERASE\r\n", CHIP_ERASE);
//	    					 if(e_25LCxx_basic_chip_erase())                                     /**< execute chip erase */
//	    					 {
//	    						 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", SET_BLOCK_PROTECT);
//	    						 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    						 return 1;														 /**< return error code */
//	    					 }
//	    					 u8PreviousState = demo.state;                                       /**< keep track of previous state */
//	    					 demo.state = READ_ENTIRE_MEMORY;                                      /**< go read full memory to confirm chhip erase of the unprotected area */
//	    					 break;
//	    				 }
//
//	    				 case READ_ENTIRE_MEMORY:
//	    				 {
//	    					 e_25LCxx_interface_debug_print("STATE:%d CHIP ERASE\r\n", CHIP_ERASE);
//	    	                 for(u16EE_addr = 0; u16EE_addr < e_25LCxx_basic_get_eeprom_legth(); u16EE_addr++)
//	    	                 {
//	    	                     if(e_25LCxx_basic_read_byte(u16EE_addr, (uint8_t *)pBufferRead, 1)) /**< reda 1 byte at the time and print */
//	    	                     {
//	    	    					 e_25LCxx_interface_debug_print("failed to execute state :%d\r\n", READ_ENTIRE_MEMORY);
//	    	    					 demo.state =  DEMO_IDLE;                                        /**< start over from idle if failed */
//	    	    					 break;
//	    	    					 return 1;														 /**< return error code */
//	    	                     }else{
//	    	                    	e_25LCxx_interface_debug_print("address: %d \t data: %d\r\n",u16EE_addr, pBufferRead[0]);
//	    	                     }
//	    	                 }
//	    	                 u8PreviousState = demo.state;                                           /**< keep track of previous state */
//	    	                 demo.state =  DEMO_IDLE;                                                /**< start over from idle if failed */
//	    					 break;
//	    				 }
//
//	    				 default:
//	    					 e_25LCxx_interface_debug_print("demo code fatal error!!\r\n");
//	    					 return 1;
//	    			     break;
//
//	    		     }
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_8;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SPI_SS_Pin */
  GPIO_InitStruct.Pin = SPI_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI_SS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint8_t spi_write(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len)
{
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&u16Reg, 1, 100);   	      /**< user defined spi write function */
	if(u16Len == 0){return 0;}
	HAL_SPI_Transmit(&hspi1, (uint8_t *)pBuf, u16Len, 100);   	          /**< user defined spi write function */
    return 0;
}

uint8_t spi_read(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len)
{
	HAL_SPI_Transmit(&hspi1, (uint8_t*)&u16Reg, 1, 100);
	if(u16Len == 1)
	{
		HAL_SPI_Receive(&hspi1, (uint8_t *)pBuf, 1, 100);
		return 0;
	}
	HAL_SPI_Receive(&hspi1, (uint8_t *)pBuf, u16Len, 100);
return 0;
}

void uart_transmit(const char *pString, uint8_t u8Length)
{
	HAL_UART_Transmit(&huart2, (const char *) pString, u8Length, 100);
}

 uint8_t spi_write_gpio_cs(uint8_t u8State)
{
   HAL_GPIO_WritePin(SPI_SS_GPIO_Port, SPI_SS_Pin, u8State);
   return 0;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
