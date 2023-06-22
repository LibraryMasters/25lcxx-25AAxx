################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/25lcxx_driver/25LCxx_driver.c \
../Core/25lcxx_driver/25LCxx_driver_basic.c \
../Core/25lcxx_driver/25LCxx_driver_interface.c 

OBJS += \
./Core/25lcxx_driver/25LCxx_driver.o \
./Core/25lcxx_driver/25LCxx_driver_basic.o \
./Core/25lcxx_driver/25LCxx_driver_interface.o 

C_DEPS += \
./Core/25lcxx_driver/25LCxx_driver.d \
./Core/25lcxx_driver/25LCxx_driver_basic.d \
./Core/25lcxx_driver/25LCxx_driver_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/25lcxx_driver/%.o Core/25lcxx_driver/%.su: ../Core/25lcxx_driver/%.c Core/25lcxx_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I"E:/Library Master/25LCxxx Eeprom/Project Example/25LCXXX_STM32L432KUC6/Core/25lcxx_driver" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-25lcxx_driver

clean-Core-2f-25lcxx_driver:
	-$(RM) ./Core/25lcxx_driver/25LCxx_driver.d ./Core/25lcxx_driver/25LCxx_driver.o ./Core/25lcxx_driver/25LCxx_driver.su ./Core/25lcxx_driver/25LCxx_driver_basic.d ./Core/25lcxx_driver/25LCxx_driver_basic.o ./Core/25lcxx_driver/25LCxx_driver_basic.su ./Core/25lcxx_driver/25LCxx_driver_interface.d ./Core/25lcxx_driver/25LCxx_driver_interface.o ./Core/25lcxx_driver/25LCxx_driver_interface.su

.PHONY: clean-Core-2f-25lcxx_driver

