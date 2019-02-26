################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MK64F12.c 

OBJS += \
./CMSIS/system_MK64F12.o 

C_DEPS += \
./CMSIS/system_MK64F12.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__USE_CMSIS -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\board" -I"C:\Users\surya\mcuxpresso\01\SDKPackages\SDK_2.3.1_FRDM-K64F\CMSIS\DSP_Lib\Source" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\source" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\drivers" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\CMSIS" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\utilities" -I"C:\Users\surya\Documents\MCUXpressoIDE_10.1.1_606\workspace\rfft\startup" -O3 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


