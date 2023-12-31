#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "25lcxx_driver/25LCxx_driver_basic.h"

/**BOARD SELECT (comment one of the two)*/
//#define MICRO_MODE_BOARD					    /**< using the micro mode from SparkFun*/
#define PI_PICO_BOARD						    /**< using the pi pico board */

#define SPI_SCK			22						/*< spi clock pin*/
#define SPI_NSS			21						/**< spi slave select pin*/
#define SPI_MOSI		23						/**< spi master out slave in pin*/
#define SPI_MISO		20						/**< spi master in slave out pin*/

E_25LCxx_info_t e_25LCxxInfo;

uint8_t spi_transmit(uint16_t u16Register, uint8_t *pData, uint8_t u16Length);
uint8_t spi_receive(uint16_t u16Register, uint8_t *pData, uint8_t u16Length);
void spi_chip_select(uint8_t status);

/****application declaration***/

uint8_t pDataRead[20];					                    /**< buffer to hold data read */
const uint8_t pDataWrite[10] = "cedric";					/**< buffer to hold data to write */
uint8_t numByteRead = 20;									/**< number of bytes to read/write */
uint8_t singleByteDataRead;									/**< data to store single byte data read */
uint16_t dataAddress = 0;									/**< eeprom address (read/write)*/
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

	#ifdef PI_PICO_BOARD 
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI); 

	//Chip select is active-low, so we'll initialize it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
	#endif

	#ifdef MICRO_MODE_BOARD 
	gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI); 

	gpio_init(SPI_NSS);
    gpio_set_dir(SPI_NSS, GPIO_OUT);
    gpio_put(SPI_NSS, 1);
	#endif

	err = e_25LCxx_basic_initialize(E_25LC160x_VARIANT, E_25LCXX_PAGE_SIZE_16_BYTE); /**< initialize chip, pass in the variant and page size */
	E_25LCxx_info(&e_25LCxxInfo);

	e_25LCxx_interface_delay_ms(4000);

	e_25LCxx_interface_debug_print("Chip Name: \t%s\r\n", e_25LCxxInfo.chip_name);
	e_25LCxx_interface_debug_print("Manufacture: \t%s\r\n", e_25LCxxInfo.manufacturer_name);
	e_25LCxx_interface_debug_print("Interface: \t%s\r\n", e_25LCxxInfo.interface);
	e_25LCxx_interface_debug_print("Supply Volt Max: \t%.1fV\r\n", e_25LCxxInfo.supply_voltage_max_v);
	e_25LCxx_interface_debug_print("Supply Volt Min: \t%.1fV\r\n", e_25LCxxInfo.supply_voltage_min_v);
	e_25LCxx_interface_debug_print("Maximum Current: \t%.1fmA\r\n", e_25LCxxInfo.max_current_ma);
	e_25LCxx_interface_debug_print("Temperature Max: \t%.1fC\r\n", e_25LCxxInfo.temperature_max);
	e_25LCxx_interface_debug_print("Temperature Min: \t%.1fC\r\n", e_25LCxxInfo.temperature_min);
	e_25LCxx_interface_debug_print("Driver Version: \tV%.1f.%.2d\r\n", (e_25LCxxInfo.driver_version / 1000), (uint8_t)(e_25LCxxInfo.driver_version - (uint8_t)(e_25LCxxInfo.driver_version / 100) * 100));

	/* eeprom data write */
	e_25LCxx_basic_write_byte(dataAddress, (uint8_t *)pDataWrite, sizeof(pDataWrite)); /**< write a block of data*/
	e_25LCxx_interface_delay_ms(2000);

	/* eeprom data read */
	e_25LCxx_basic_read_byte(dataAddress, (uint8_t *)pDataRead, numByteRead);
	for (int index = 0; index < numByteRead; index++)
	{
		e_25LCxx_interface_debug_print("address read: %d Data Read :%x\n", index, pDataRead[index]);
	}
	e_25LCxx_interface_delay_ms(2000);

	/* eeprom put */
	e_25LCxx_basic_put_byte(11, (uint32_t *)&dateTime, sizeof(dateTime)); /**< write a 4 bytes long integer, This function is equivalent to the Arduino "eeprom_put" function  */
	e_25LCxx_interface_delay_ms(2000);

	/* eeprom get */
	e_25LCxx_basic_get_byte(11, (uint32_t *)&byteGet, sizeof(byteGet)); /**< get a 4-byte long integer from address 11 (this function equivalent to Arduino "eeprom_get" function) */
	e_25LCxx_interface_debug_print("\nGet date: %lu\n", byteGet);

	/* eeprom update */
	e_25LCxx_basic_update(0, (uint8_t *)pDataWrite, 10); /**< This function is an equivalent to the arduino "eeprom_update" function */

	/* check the block protect status */
	e_25LCxx_basic_get_bp_status((uint8_t *)&singleByteDataRead);
	e_25LCxx_interface_debug_print("Block protect status: %d\n", singleByteDataRead);

	err = e_25LCxx_basic_erase_page(64);		   /**< erasing memory page 64 (address: 1008 - 1023)*/
	err = e_25LCxx_basic_erase_sector(1532, 1540); /**< attempt to erase a protected region */

	err = e_25LCxx_basic_get_memory_properties((uint16_t *)&totalSize, (uint16_t *)&freeSpace, (uint16_t *)&usedSpace);
	e_25LCxx_interface_debug_print("Total size : %d Bytes\nFree space: %d Bytes\nUsed space: %d Bytes\n", totalSize, freeSpace, usedSpace);

	for(int index = 0; index < e_25LCxx_basic_get_eeprom_legth(); index++){
	  err = e_25LCxx_basic_read_byte(index, (uint8_t *)&singleByteDataRead, 1);
	  e_25LCxx_interface_debug_print(" add: %d data: 0x%x,", index+1, singleByteDataRead);
	  if(index % 10 == 0)
	  e_25LCxx_interface_debug_print("\n\r");
	}

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
	#ifdef PI_PICO_BOARD
	asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, status);  
    asm volatile("nop \n nop \n nop");
	#endif

	#ifdef MICRO_MODE_BOARD
	asm volatile("nop \n nop \n nop");
    gpio_put(SPI_NSS, status);  
    asm volatile("nop \n nop \n nop");
	#endif
}
