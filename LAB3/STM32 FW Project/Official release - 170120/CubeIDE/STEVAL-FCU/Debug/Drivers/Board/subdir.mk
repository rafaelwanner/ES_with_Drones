################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1.c \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_accelero.c \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_gyro.c \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_magneto.c \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_pressure.c \
/home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_temperature.c 

OBJS += \
./Drivers/Board/steval_fcu001_v1.o \
./Drivers/Board/steval_fcu001_v1_accelero.o \
./Drivers/Board/steval_fcu001_v1_gyro.o \
./Drivers/Board/steval_fcu001_v1_magneto.o \
./Drivers/Board/steval_fcu001_v1_pressure.o \
./Drivers/Board/steval_fcu001_v1_temperature.o 

C_DEPS += \
./Drivers/Board/steval_fcu001_v1.d \
./Drivers/Board/steval_fcu001_v1_accelero.d \
./Drivers/Board/steval_fcu001_v1_gyro.d \
./Drivers/Board/steval_fcu001_v1_magneto.d \
./Drivers/Board/steval_fcu001_v1_pressure.d \
./Drivers/Board/steval_fcu001_v1_temperature.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Board/steval_fcu001_v1.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Board/steval_fcu001_v1_accelero.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_accelero.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1_accelero.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Board/steval_fcu001_v1_gyro.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_gyro.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1_gyro.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Board/steval_fcu001_v1_magneto.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_magneto.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1_magneto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Board/steval_fcu001_v1_pressure.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_pressure.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1_pressure.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Board/steval_fcu001_v1_temperature.o: /home/rwan/ETH/ES\ with\ Drones/LAB3/STM32\ FW\ Project/Official\ release\ -\ 170120/Drivers/BSP/STEVAL_FCU001_V1/steval_fcu001_v1_temperature.c Drivers/Board/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xC -DDEBUG -c -I../../../Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/BSP/Board -I../../../Drivers/BSP/Components/lis3mdl -I../../../Drivers/BSP/Components/lps25hb -I../../../Drivers/BSP/Components/Common -I../../../Drivers/BSP/Components/lsm6dsl -I../../../Drivers/BSP/Components/lps22hb -I../../../Drivers/BSP/Components/lsm303agr -I../../../Drivers/BSP/STEVAL_FCU001_V1 -I../../../Drivers/BSP/Components/lis2mdl -I../../../Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes -I../../../Drivers/BSP/Components/spbtlerf -O0 -ffunction-sections -fdata-sections -Wall -Wno-unused-function -fstack-usage -MMD -MP -MF"Drivers/Board/steval_fcu001_v1_temperature.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

