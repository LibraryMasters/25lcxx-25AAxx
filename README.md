

<div align=center>
<img src="Document/Image/avento.png" width="400" height="200"/>
</div>

## Library Maters 25AAxx/25LCxx 

25AAxx/25LCxx are Serial Electrically Erasable EEPROMs. The memory is accessed via a simple Serial Peripheral Interface (SPI) compatible serial bus.
The bus signals required are a clock input (SCK) plus separate data in (SI) and data out (SO) lines. Access to the device is controlled through a Chip Select (CS) input.

The Library masters 25AAxx/25LCxx is the full-function driver of the SHT4x sensors series ((25LC080C/25LC080D/25LC160C/ 25LC160D/25LC320A/25LC640A/ 25LC128/25LC256/25LC512, 25AA080C/25AA080D/25AA160C/ 25AA160D/25AA320A/25AA640A/ 25LC128/25AA256/25AA512)). The driver is written C language with Code Blocks, It provides the functions to read and write the memory.

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
  - [STM32L432 (STM32CubeIDE)](https://github.com/LibraryMasters/sht4x/tree/master/Project%20Example/STM32L432/sht40x_temp_sens_k423kc)
  - [PIC24FJ128GA705 (MPLAB X IDE)](https://github.com/LibraryMasters/sht4x/tree/master/Project%20Example/PIC24FJ128GA705/sht40x_Temp_himidity_sens.X)
  - [SAMD21G18 (Atmel studio 7)](https://github.com/LibraryMasters/sht4x/tree/master/Project%20Example/SAMD21/sht40x_Temp_humidity)
  - [ATMEGA4808 (Atmel studio 7)](https://github.com/LibraryMasters/sht4x/tree/master/Project%20Example/ATMEGA4808/sht40x_Temp_humidity)
  ### Usage
  #### example basic

  ```C
#include "sht40x_driver_basic.h"

sht40x_info_t sht40xInfo;
sht40x_data_t dataRead;

int err;
uint32_t UID;
uint8_t variant;
uint8_t deviceAdd;
float tempBuffer = 0;
float humidityBuffer = 0;
uint8_t NumberSamples = 10;

int main()
{

    sht40x_basic_initialize(SHT40_AD1B_VARIANT);
    sht40x_info(&sht40xInfo);

    sht40x_interface_debug_print("Chip Name: \t%s\r\n", sht40xInfo.chip_name);
    sht40x_interface_debug_print("Manufacturer: \t%s\r\n",sht40xInfo.manufacturer_name);
    sht40x_interface_debug_print("Interface Protocol: \t%s\r\n", sht40xInfo.interface);
    sht40x_interface_debug_print("Supply Volt Max: \t%.1f V\r\n",sht40xInfo.supply_voltage_max_v);
    sht40x_interface_debug_print("Supply Volt Min: \t%.1f V\r\n",sht40xInfo.supply_voltage_min_v);
    sht40x_interface_debug_print("Maximum Current: \t%.1f uA\r\n",sht40xInfo.max_current_ma);
    sht40x_interface_debug_print("Max Temperature: \t%.1f C\r\n",sht40xInfo.temperature_max);
    sht40x_interface_debug_print("Diver Version: \t\tV%.1f.%.2d\r\n",(sht40xInfo.driver_version /1000), (uint8_t)(sht40xInfo.driver_version - (uint8_t)(sht40xInfo.driver_version / 100)*100));

    err =  sht40x_basic_get_temp_rh(SHT40X_PRECISION_HIGH, &dataRead);
    if(err)
    {
      sht40x_interface_debug_print("failed to read\n");
    }
    sht40x_interface_debug_print("\nTemp C: %.2f\n", dataRead.temperature_C);
    sht40x_interface_debug_print("Temp F: %.2f\n", dataRead.temperature_F);
    sht40x_interface_debug_print("Humidity: %.2f\n", dataRead.humidity);

    /**Measure Temp and humidity with n number of samples */
    err = sht40x_basic_get_temp_humidity_nSample(SHT40X_PRECISION_HIGH, &dataRead, NumberSamples);
    sht40x_interface_debug_print("\nTemp C sampled: %.2f\n", dataRead.temperature_C);
    sht40x_interface_debug_print("Humidity sampled: %.2f\n", dataRead.humidity);

    /** Get device unique ID */
    err = sht40x_basic_get_serial_number((uint32_t*) &UID);
    if(err)
    {
      /**< do something */
    }
    sht40x_interface_debug_print("serial number : %lu\n", UID);

    /** Activate heater and measure temperature */
    err = sht40x_basic_activate_heater(SHT40X_HEATER_POWER_20mW_100mS, &dataRead);
    sht40x_interface_debug_print("\nHeater Temp C: %.2f\n", dataRead.temperature_C);
    sht40x_interface_debug_print("Heater Temp F: %.2f\n", dataRead.temperature_F);
    sht40x_interface_debug_print("Heater Humidity: %.2f\n", dataRead.humidity);
    return 0;
}
  ```

  #### example interface
  
  ```C
  ...

uint8_t sht40x_interface_i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    /*call your i2c read function here*/
    /*user code begin */
     if(i2c_read(addr, reg, buf, len) != 0)
     {
         return 1;
     }
    /*user code end*/
    return 0; /**< success */
}

uint8_t sht40x_interface_i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    /*call your i2c write function here*/
    /*user code begin */
     if(i2c_write(addr, reg, buf, len) != 0)
     {
         return 1;
     }
    /*user code end*/
    return 0; /**< success */
}

void sht40x_interface_delay_ms(uint32_t u32Ms)
{
    /*call your delay function here*/
    /*user code begin */
    delay_ms(u32Ms);          //(use your IDE api delay function)
    /*user code end*/
}

void sht40x_interface_debug_print(const char *const fmt, ...)
{
    /*call your call print function here*/
    /*user code begin */
//#ifdef SHT40X_DEBUG_MODE
    volatile char str[256];
    volatile uint8_t len;
    va_list args;

    memset((char *) str, 0, sizeof (char)*256);
    va_start(args, fmt);
    vsnprintf((char *) str, 256, (char const *) fmt, args);
    va_end(args);

    len = strlen((char *) str);
    //   EUSART1_Write_Text((const char *) str, len);        /**< example of a usart function */
      printf((char *const *)str, len);                  /**< example of printf function, comment out if not used */

    /*user code end*/
//#endif
}

  ...
  
  ```

  ### Document
  [datasheet](https://github.com/LibraryMasters/sht4x/blob/master/Document/Datasheet_SHT4x%20temperature%20sensor.pdf)
  
  ### Contribute
   1. Clone repo and create a new branch: ```https://github.com/LibraryMasters/sht4x_PR.git```
   2. Make changes and test
   3. Submit a Pull Request with a comprehensive description of changes
  ### License
  [MIT](https://choosealicense.com/licenses/mit/)
### Contact Us

Email address: cedricmalyam@gmail.com

### Acknowledgements 
- @PeterHenderson https://dribbble.com/peterhenderson for the logo
