################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -D__USE_CMSIS -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\board" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\source" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\drivers" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\CMSIS" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\utilities" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\sai_3\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


