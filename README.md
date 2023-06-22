

<div align=center>
<img src="Document/Image/avento.png" width="400" height="200"/>
</div>

## Library Maters 25AAxx/25LCxx 

25AAxx/25LCxx are Serial Electrically Erasable EEPROMs. The memory is accessed via a simple Serial Peripheral Interface (SPI) compatible serial bus.
The bus signals required are a clock input (SCK) plus separate data in (SI) and data out (SO) lines. Access to the device is controlled through a Chip Select (CS) input.

The Library masters 25AAxx/25LCxx is the full-function driver of the SHT4x sensors series ((25LC080C/25LC080D/25LC160C/ 25LC160D/25LC320A/25LC640A/ 25LC128/25LC256/25LC512, 25AA080C/25AA080D/25AA160C/ 25AA160D/25AA320A/25AA640A/ 25LC128/25AA256/25AA512)). The driver is written in C language with Code Blocks, It provides the functions to read and write the memory.

### Table of Cnntents

  - [Install](#Install)
  - [Examples](#Examples)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example interface](#example-interface)
  - [Document](#Document)
  - [How to contribute](#Contribute)
  - [License](#License)
  - [Contact Us](#Contact-Us)
  - [Acknowledgements](#Acknowledgements)

  ### Install
  - The interface.c file expect the below functions to correctly link the driver 
  ```
    - spi_initialize function  ( optional )
    - spi_deinitialize function ( optional )
    - spi_read function ( Mandatory )
    - spi_write function ( Mandatory )
    - chip_select function ( Mandatory )
    - delay function ( Mandatory )
    - print function (optional )
  ```
  - refer to video: "coming soon..."
  - Use example project 



  ### Examples
  - [STM32L432 (STM32CubeIDE)](https://github.com/LibraryMasters/25lcxx-25AAxx/tree/master/Project%20Example/25LCXXX_STM32L432KUC6)
  - [PIC24FJ128GA705 (MPLAB X IDE)](https://github.com/LibraryMasters/25lcxx-25AAxx/tree/master/Project%20Example/25LCXXX_PIC24FJ128GA705)
  - [SAMD21G18 (Atmel studio 7)](https://github.com/LibraryMasters/25lcxx-25AAxx/tree/master/Project%20Example/25LCXXX_ATSAMD21)
  - [ATMEGA4808 (Atmel studio 7)](https://github.com/LibraryMasters/25lcxx-25AAxx/tree/master/Project%20Example/25LCXXX_ATMEGA4808)
  ### Usage
  #### example basic

  ```C
#include "25LCxx_driver_basic.h"

E_25LCxx_info_t e_25LCxxInfo;           /**< info structure object */

/****application declaration***/

uint8_t pDataRead[25];					             /**< buffer to hold data read */
const uint8_t pDataWrite[10] = {0x00, 0x12, 0x54, 0x46, 0x95, 0xa5, 0xb7, 0xc3, 0xf4, 0x2d};						/**< buffer to write */
uint8_t numByteRead;										    /**< number of bytes to read/write */
uint8_t singleByteDataRead;									/**< data to store single byte data read */
uint16_t dataAddress;										    /**< eeprom address (read/write)*/
uint32_t byteGet;
uint32_t dateTime = 2305031413;
float floatData = 45.5f;
int err;
uint16_t totalSize, freeSpace, usedSpace;

/***End***/

int main(void)
{
	/* Initializes MCU, drivers and middle ware */
	atmel_start_init();
	
	err = e_25LCxx_basic_initialize(E_25LC160x_VARIANT, E_25LCXX_PAGE_SIZE_16_BYTE);
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

	/* Replace with your application code */
	
		dataAddress = 0;          /**< start read operation at eeprom address 0*/
		numByteRead = 25;		      /**< read 20 bytes */
	
	  err = e_25LCxx_basic_get_bp_status((uint8_t *)&singleByteDataRead);
		e_25LCxx_interface_debug_print("Block protect status: %d\n", singleByteDataRead);
		
		//err = e_25LCxx_basic_erase_sector(0 , 9);									  /**< attempt to erase a protected region */
		//e_25LCxx_basic_put_byte(10, (uint32_t *)&dateTime,  sizeof(dateTime));
		//err = e_25LCxx_basic_erase_page(1);
	  //err = e_25LCxx_basic_update(0,(uint8_t *)pDataWrite, 10);
		
		err= e_25LCxx_basic_read_byte(dataAddress, (uint8_t *)pDataRead, numByteRead);
		for (int index = 0; index < numByteRead; index++){
			e_25LCxx_interface_debug_print("address read: %d Data Read :%x\n",index, pDataRead[index]);
		}
		
		//err = e_25LCxx_basic_update(0,(uint8_t *)pDataWrite, 10);
		//err = e_25LCxx_basic_erase_page(2);
	
		/*for(int index = 0; index < e_25LCxx_basic_get_eeprom_legth(); ++index){
			err = e_25LCxx_basic_read_byte(index, (uint8_t *)&singleByteDataRead, 1);
			e_25LCxx_interface_debug_print(" add: %d data: 0x%x", index, singleByteDataRead);
			if(index % 10 == 0)
			e_25LCxx_interface_debug_print("\n\r");
		}*/
		
		err = e_25LCxx_basic_get_byte(10, (uint32_t *)&byteGet, sizeof(byteGet)); 
		e_25LCxx_interface_debug_print("\nGet date: %lu\n", byteGet);
		
		err = e_25LCxx_basic_erase_page(64);											  /**< erasing memory page 64 (address: 1008 - 1023)*/
		err = e_25LCxx_basic_erase_sector(1532, 1540);									  /**< attempt to erase a protected region */
		err = e_25LCxx_basic_write_byte(2011,(uint8_t *)pDataWrite, sizeof(pDataWrite));  /**< attempt to write block of data bigger than the page size, in a protected region*/
		
		err = e_25LCxx_basic_get_memory_properties((uint16_t*) &totalSize, (uint16_t *)&freeSpace, (uint16_t *)&usedSpace);
		e_25LCxx_interface_debug_print("Total size : %d Bytes\nFree space: %d Bytes\nUsed space: %d Bytes\n", totalSize, freeSpace, usedSpace);
	
	while (1) {
		LED_GREEN_toggle_level();
		e_25LCxx_interface_delay_ms(1000);
	}
}
  ```

  #### example interface
  
  ```C
  ...

uint8_t e_25LCxx_interface_spi_read(uint16_t u16Reg, uint32_t *pbuf, uint16_t u16Len)
{
    /*call your spi read function here*/
    /*user code begin */
    spi_read(u16Reg, (uint8_t*)pbuf, u16Len);
    /*user code end*/
    return 0;      /**< return success */
}

uint8_t e_25LCxx_interface_spi_write(uint16_t u16Reg, uint32_t *pbuf, uint16_t u16Len)
{
    /*call your spi write function here*/
    /*user code begin */
    spi_write(u16Reg, (uint8_t*)pbuf, u16Len);
    /*user code end*/
    return 0;      /**< return success */
}

uint8_t e_25LCxx_interface_gpio_write(uint8_t u8State)
{
    /*call your gpio write function here*/
    /*user code begin */

    /*user code end*/
    return 0;       /**< return success */
}

uint8_t e_25LCxx_interface_chip_select(uint8_t u8State)
{
    /*call your spi chip select function here*/
    /*user code begin */
    spi_chip_select(u8State);
    /*user code end*/
    return 0;      /**< return success */
}

/**
 * @brief  This function interface delay ms
 * @param[in] u32Ms is the time delay in milliseconds
 * @note      none
 */
void e_25LCxx_interface_delay_ms(uint32_t u32Ms)
{
    /*call your delay function here*/
    /*user code begin */
    delay_ms(u32Ms);
    /*user code end*/

}

void e_25LCxx_interface_debug_print(const char *const fmt, ...)
{
    /*call your call print function here*/
    /*user code begin */
#ifdef E_25LCXX_DEBUG_MODE
    volatile char str[256];
    volatile uint8_t len;
    va_list args;

    memset((char *) str, 0, sizeof (char)*256);
    va_start(args, fmt);
    vsnprintf((char *) str, 256, (char const *) fmt, args);
    va_end(args);

    len = strlen((char *) str);
    printf((char *const)str, len);

    /*user code end*/
#endif
}

  ...
  
  ```

  ### Document
  [datasheet](https://github.com/LibraryMasters/25lcxx-25AAxx/blob/master/Document/25LCXXX-8K-256K-SPI-Serial-EEPROM-High-Temp-Family-Data-Sheet-20002131E.pdf)
  
  ### Contribute
   1. Clone repo and create a new branch: ```https://github.com/LibraryMasters/25LCXX-25AAXX_PR.git```
   2. Make changes and test
   3. Submit a Pull Request with a comprehensive description of changes
  ### License
  [MIT](https://choosealicense.com/licenses/mit/)
### Contact Us

Email address: cedricmalyam@gmail.com

### Acknowledgements 
- @PeterHenderson https://dribbble.com/peterhenderson for the logo
