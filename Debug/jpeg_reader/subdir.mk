################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../jpeg_reader/jpg_imagen.cpp \
../jpeg_reader/jpg_memsrc.cpp \
../jpeg_reader/jpg_readwrite.cpp 

OBJS += \
./jpeg_reader/jpg_imagen.o \
./jpeg_reader/jpg_memsrc.o \
./jpeg_reader/jpg_readwrite.o 

CPP_DEPS += \
./jpeg_reader/jpg_imagen.d \
./jpeg_reader/jpg_memsrc.d \
./jpeg_reader/jpg_readwrite.d 


# Each subdirectory must supply rules for building sources it contributes
jpeg_reader/%.o: ../jpeg_reader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/valentin/workspace/Belkan/include" -I"/home/valentin/workspace/Belkan/jpeg_reader" -O0 -g3 -Wall -c -fmessage-length=0 -g -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


