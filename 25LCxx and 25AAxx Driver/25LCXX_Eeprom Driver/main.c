#include "25LCxx_driver_basic.h"
#include <math.h>

/**
 * @brief convert hexadecimal to binary
 * @param[in] hex_number_in in the number to pass into the function
 * @param[out] *bin_number_out point to converted binary string
 * @return status code
 * @note the function currently support 2 byte
 */

uint8_t validate_page_boundary1(uint8_t u8PageSize, uint16_t u16Address, uint8_t u8Length);
void hex_to_bin(uint16_t hex_number_in, char *bin_number_out);

E_25LCxx_info_t e_25LCxxInfo;

uint32_t data1 = 0xfe3250;
uint8_t data2[4] = {0xff, 0x32, 0x50} ;

uint32_t dataOut = 0;
uint16_t count = 2;
uint8_t size = 3;
uint32_t tempData;

int result;
int ptr;

int main()
{
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

    return 0;
}

/**
 * @brief convert hexadecimal to binary
 * @param[in] hex_number_in in the number to pass into the function
 * @param[out] *bin_number_out point to converted binary string
 * @return status code
 * @note the function currently support 2 byte
 */
void hex_to_bin(uint16_t hex_number_in, char *bin_number_out)
{
    uint8_t index;
    for (index = 0; index < 8; index++)       /**< use 16 and 0x8000 for 2 byte*/
    {
        if (hex_number_in & (0x80 >> index))
            *bin_number_out = '1';
        else
            *bin_number_out = '0';

        bin_number_out++;
    }
    *bin_number_out = '\0';
}

























