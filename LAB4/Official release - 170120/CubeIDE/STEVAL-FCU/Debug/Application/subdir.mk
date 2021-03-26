################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/home/rwan/ETH/ES\ with\ Drones/LAB4/Official\ release\ -\ 170120/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xc.s 

OBJS += \
./Application/startup_stm32f401xc.o 

S_DEPS += \
./Application/startup_stm32f401xc.d 


# Each subdirectory must supply rules for building sources it contributes
Application/startup_stm32f401xc.o: /home/rwan/ETH/ES\ with\ Drones/LAB4/Official\ release\ -\ 170120/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xc.s Application/subdir.mk
	arm-none-eabi-gcc -c -mcpu=cortex-m4 -g3 -c -Wa,-W -x assembler-with-cpp -MMD -MP -MF"Application/startup_stm32f401xc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

