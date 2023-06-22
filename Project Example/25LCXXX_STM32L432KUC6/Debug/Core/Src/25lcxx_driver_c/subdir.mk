################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/25lcxx_driver_c/25LCxx_driver.c \
../Core/Src/25lcxx_driver_c/25LCxx_driver_basic.c \
../Core/Src/25lcxx_driver_c/25LCxx_driver_interface.c 

OBJS += \
./Core/Src/25lcxx_driver_c/25LCxx_driver.o \
./Core/Src/25lcxx_driver_c/25LCxx_driver_basic.o \
./Core/Src/25lcxx_driver_c/25LCxx_driver_interface.o 

C_DEPS += \
./Core/Src/25lcxx_driver_c/25LCxx_driver.d \
./Core/Src/25lcxx_driver_c/25LCxx_driver_basic.d \
./Core/Src/25lcxx_driver_c/25LCxx_driver_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/25lcxx_driver_c/%.o Core/Src/25lcxx_driver_c/%.su: ../Core/Src/25lcxx_driver_c/%.c Core/Src/25lcxx_driver_c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L432xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-25lcxx_driver_c

clean-Core-2f-Src-2f-25lcxx_driver_c:
	-$(RM) ./Core/Src/25lcxx_driver_c/25LCxx_driver.d ./Core/Src/25lcxx_driver_c/25LCxx_driver.o ./Core/Src/25lcxx_driver_c/25LCxx_driver.su ./Core/Src/25lcxx_driver_c/25LCxx_driver_basic.d ./Core/Src/25lcxx_driver_c/25LCxx_driver_basic.o ./Core/Src/25lcxx_driver_c/25LCxx_driver_basic.su ./Core/Src/25lcxx_driver_c/25LCxx_driver_interface.d ./Core/Src/25lcxx_driver_c/25LCxx_driver_interface.o ./Core/Src/25lcxx_driver_c/25LCxx_driver_interface.su

.PHONY: clean-Core-2f-Src-2f-25lcxx_driver_c

