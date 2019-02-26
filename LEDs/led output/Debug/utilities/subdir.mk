################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c \
../utilities/fsl_debug_console.c \
../utilities/fsl_io.c \
../utilities/fsl_log.c \
../utilities/fsl_str.c 

OBJS += \
./utilities/fsl_assert.o \
./utilities/fsl_debug_console.o \
./utilities/fsl_io.o \
./utilities/fsl_log.o \
./utilities/fsl_str.o 

C_DEPS += \
./utilities/fsl_assert.d \
./utilities/fsl_debug_console.d \
./utilities/fsl_io.d \
./utilities/fsl_log.d \
./utilities/fsl_str.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DFRDM_K64F -DCR_INTEGER_PRINTF -DFREEDOM -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\board" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\source" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\drivers" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\CMSIS" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\utilities" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\frdmk64f_driver_examples_gpio_gpio_led_output\startup" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


