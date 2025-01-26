################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/UART.c \
../Core/Src/checking_functions.c \
../Core/Src/config.c \
../Core/Src/custom_callbacks.c \
../Core/Src/input_capture.c \
../Core/Src/main.c \
../Core/Src/oscillator.c \
../Core/Src/startup.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/symmetry_adjustment.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/timers.c \
../Core/Src/utility.c 

OBJS += \
./Core/Src/UART.o \
./Core/Src/checking_functions.o \
./Core/Src/config.o \
./Core/Src/custom_callbacks.o \
./Core/Src/input_capture.o \
./Core/Src/main.o \
./Core/Src/oscillator.o \
./Core/Src/startup.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/symmetry_adjustment.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/timers.o \
./Core/Src/utility.o 

C_DEPS += \
./Core/Src/UART.d \
./Core/Src/checking_functions.d \
./Core/Src/config.d \
./Core/Src/custom_callbacks.d \
./Core/Src/input_capture.d \
./Core/Src/main.d \
./Core/Src/oscillator.d \
./Core/Src/startup.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/symmetry_adjustment.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/timers.d \
./Core/Src/utility.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/UART.cyclo ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/UART.su ./Core/Src/checking_functions.cyclo ./Core/Src/checking_functions.d ./Core/Src/checking_functions.o ./Core/Src/checking_functions.su ./Core/Src/config.cyclo ./Core/Src/config.d ./Core/Src/config.o ./Core/Src/config.su ./Core/Src/custom_callbacks.cyclo ./Core/Src/custom_callbacks.d ./Core/Src/custom_callbacks.o ./Core/Src/custom_callbacks.su ./Core/Src/input_capture.cyclo ./Core/Src/input_capture.d ./Core/Src/input_capture.o ./Core/Src/input_capture.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/oscillator.cyclo ./Core/Src/oscillator.d ./Core/Src/oscillator.o ./Core/Src/oscillator.su ./Core/Src/startup.cyclo ./Core/Src/startup.d ./Core/Src/startup.o ./Core/Src/startup.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/symmetry_adjustment.cyclo ./Core/Src/symmetry_adjustment.d ./Core/Src/symmetry_adjustment.o ./Core/Src/symmetry_adjustment.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su ./Core/Src/timers.cyclo ./Core/Src/timers.d ./Core/Src/timers.o ./Core/Src/timers.su ./Core/Src/utility.cyclo ./Core/Src/utility.d ./Core/Src/utility.o ./Core/Src/utility.su

.PHONY: clean-Core-2f-Src

