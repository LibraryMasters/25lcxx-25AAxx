/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.1
        Device            :  PIC24FJ128GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include "mcc_generated_files/system.h"
#include "25lcxx_eeprom_driver/25LCxx_driver_basic.h"
#include "utilies_file.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"

/*
                         Main application
 */
E_25LCxx_info_t e_25LCxxInfo;

uint8_t spi_write(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len);
uint8_t spi_read(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len);
uint8_t spi_write_gpio_cs(uint8_t u8State);

/*application declarations*/
uint32_t timeCheck;
int index;
uint8_t pBuffer[10];
uint8_t dataRead;
void led_blink(void);

/****application declaration***/

uint8_t pDataRead[20]; /**< buffer to hold data read */
const uint8_t pDataWrite[15] = "cedric"; /**< buffer to hold data to write */
uint8_t numByteRead; /**< number of bytes to read/write */
uint8_t singleByteDataRead; /**< data to store single byte data read */
uint16_t dataAddress; /**< eeprom address (read/write)*/
uint32_t byteGet;
uint32_t dateTime = 2305031413;
float floatData = 45.5f;
int err;
uint16_t totalSize, freeSpace, usedSpace;

/***End***/

/**end of global definitions*/

int main(void) {
    // initialize the device
    SYSTEM_Initialize();

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
    e_25LCxx_interface_debug_print("Driver Version: \tV%.1f.%.2d\r\n", (e_25LCxxInfo.driver_version / 1000), (uint8_t) (e_25LCxxInfo.driver_version - (uint8_t) (e_25LCxxInfo.driver_version / 100)*100));

    dataAddress = 0; /**< start read operation at eeprom address 0*/
    numByteRead = 20; /**< read 20 bytes */

    err = e_25LCxx_basic_get_bp_status((uint8_t *) & singleByteDataRead);
    e_25LCxx_interface_debug_print("Block protect status: %d\n", singleByteDataRead);

    //    e_25LCxx_basic_put_byte(10, (uint32_t *)&dateTime,  sizeof(dateTime));

    err = e_25LCxx_basic_read_byte(dataAddress, (uint8_t *) pDataRead, numByteRead);
    for (int index = 0; index < numByteRead; index++) {
        e_25LCxx_interface_debug_print("address read: %d Data Read :%x\n", index, pDataRead[index]);
    }

    //err = e_25LCxx_basic_update(1536,(uint8_t *)pDataWrite, sizeof(pDataWrite));

    for (int index = 0; index < e_25LCxx_basic_get_eeprom_legth(); ++index) {
        err = e_25LCxx_basic_read_byte(index, (uint8_t *) & singleByteDataRead, 1);
        e_25LCxx_interface_debug_print(" add: %d data: 0x%x", index, singleByteDataRead);
        if (index % 10 == 0)
            e_25LCxx_interface_debug_print("\n\r");
    }

    //e_25LCxx_basic_put_byte(10, (uint32_t *)&dateTime,  sizeof(dateTime));

    //    err = e_25LCxx_basic_update(12, (uint8_t *) pDataWrite, sizeof (pDataWrite));

    //        err = e_25LCxx_basic_read_byte(4, (uint8_t *) & singleByteDataRead, 1);
    //    e_25LCxx_interface_debug_print("data address 10: %d\n", singleByteDataRead);

    //    err = e_25LCxx_basic_get_byte(10, (uint32_t *) & byteGet, sizeof (byteGet));
    //    e_25LCxx_interface_debug_print("\nGet date: %lu\n", byteGet);

    err = e_25LCxx_basic_erase_page(64);                                                /**< erasing memory page 64 (address: 1008 - 1023)*/
    err = e_25LCxx_basic_erase_sector(1532, 1540);                                      /**< attempt to erase a protected region */
    err = e_25LCxx_basic_write_byte(2011, (uint8_t *) pDataWrite, sizeof (pDataWrite)); /**< attempt to write block of data bigger than page size, in a protected region*/

    err = e_25LCxx_basic_get_memory_properties((uint16_t*) & totalSize, (uint16_t *) & freeSpace, (uint16_t *) & usedSpace);
    e_25LCxx_interface_debug_print("Total size : %d Bytes\nFree space: %d Bytes\nUsed space: %d Bytes\n", totalSize, freeSpace, usedSpace);


    while (1) {
        non_blocking_task(led_blink, _500_MS_TIMEOUT, timeCheck);
        // Add your application code
    }

    return 1;
}

void led_blink(void) {
    LED_BLUE_Toggle();
    timeCheck = millis();
}

uint8_t spi_write(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len) {

    uint8_t err;
    err = SPI_1_write_block((uint8_t *) & u16Reg, 1, 100);
    if (err != 0) {
        return err;
    }
    if (u16Len == 0) {
        return 0;
    }
    err = SPI_1_write_block((uint8_t *) pBuffer, u16Len, 100);
    if (err != 0) {
        return err;
    }
    return 0;
}

uint8_t spi_read(uint16_t u16Reg, uint32_t *pBuf, uint16_t u16Len) {

    uint8_t err;

    err = SPI_1_write_block((uint8_t *) & u16Reg, 1, 100);
    if (err != 0) {
        return err;
    }

    if (u16Len == 1) {
        err = SPI_1_read_block((uint8_t *) pBuffer, 1, 100);
        if (err != 0) {
            return err;
        }
    }

    err = SPI_1_read_block((uint8_t *) pBuffer, u16Len, 100);
    if (err != 0) {
        return err;
    }
    return 0;
}

uint8_t spi_write_gpio_cs(uint8_t u8State) {

    if (u8State == 1) {

        SS1OUT_SetHigh();
        delay_ms(2);
    } else {
        delay_ms(2);
        SS1OUT_SetLow();

    }

    return 0;
}
/**
 End of File
 */

