/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define PA04 GPIO(GPIO_PORTA, 4)
#define PA05 GPIO(GPIO_PORTA, 5)
#define SPI_CS GPIO(GPIO_PORTA, 6)
#define PA07 GPIO(GPIO_PORTA, 7)
#define led_blue GPIO(GPIO_PORTA, 21)
#define PB02 GPIO(GPIO_PORTB, 2)
#define PB03 GPIO(GPIO_PORTB, 3)

#endif // ATMEL_START_PINS_H_INCLUDED
