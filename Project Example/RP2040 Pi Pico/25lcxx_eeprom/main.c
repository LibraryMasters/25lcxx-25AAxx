#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "25lcxx_driver/25LCxx_driver_basic.h"

E_25LCxx_info_t e_25LCxxInfo;

uint8_t spi_transmit(uint16_t u16Register, uint8_t *pData, uint8_t u16Length);
uint8_t spi_receive(uint16_t u16Register, uint8_t *pData, uint8_t u16Length);
void spi_chip_select(uint8_t status);

/****application declaration***/

uint8_t pDataRead[20];					                    /**< buffer to hold data read */
const uint8_t pDataWrite[10] = "cedric";					/**< buffer to hold data to write */
uint8_t numByteRead;										/**< number of bytes to read/write */
uint8_t singleByteDataRead;									/**< data to store single byte data read */
uint16_t dataAddress;										/**< eeprom address (read/write)*/
uint32_t byteGet;
uint32_t dateTime = 2305031413;
float floatData = 45.5f;
int err;
uint16_t totalSize, freeSpace, usedSpace;

/***End***/

int main()
{
    stdio_init_all();
         
    spi_init(spi_default, 250 * 1000);  /**< This example will use SPI0 at 0.25MHz.  */ 
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI); 

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1); 

    err = e_25LCxx_basic_initialize(E_25LC160x_VARIANT, E_25LCXX_PAGE_SIZE_16_BYTE);
	E_25LCxx_info(&e_25LCxxInfo);

    e_25LCxx_interface_delay_ms(3000);
	
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

    while (1)
    {
		
    }
    return 0;
}


uint8_t spi_transmit(uint16_t u16Register, uint8_t *pData, uint8_t u16Length)
{
	spi_write_blocking(spi_default, (uint8_t *)&u16Register, 1);
    if(u16Length == 0){return 0;}
    spi_write_blocking(spi_default, (uint8_t *)pData, u16Length);
	return 0;
}

uint8_t spi_receive(uint16_t u16Register, uint8_t *pData, uint8_t u16Length)
{
   	spi_write_blocking(spi_default, (uint8_t *)&u16Register, 1);
	if(u16Length == 1)
	{
		spi_read_blocking(spi_default, 0, (uint8_t *)pData, 1);
	}
	spi_read_blocking(spi_default, 0, (uint8_t *)pData, u16Length);
	return 0;
}

void spi_chip_select(uint8_t status)
{
	asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, status);  
    asm volatile("nop \n nop \n nop");
}