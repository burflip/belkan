################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/tools/ppm.cpp 

OBJS += \
./include/tools/ppm.o 

CPP_DEPS += \
./include/tools/ppm.d 


# Each subdirectory must supply rules for building sources it contributes
include/tools/%.o: ../include/tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/valentin/workspace/Belkan/include" -I"/home/valentin/workspace/Belkan/jpeg_reader" -O0 -g3 -Wall -c -fmessage-length=0 -g -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


