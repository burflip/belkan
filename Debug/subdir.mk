################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Astar.cpp \
../Brain.cpp \
../GUI.cpp \
../Node.cpp \
../belkan.cpp \
../conexion.cpp \
../dibujosGL.cpp \
../environment.cpp \
../luz.cpp \
../main.cpp \
../retrato.cpp 

CC_SRCS += \
../objeto3d.cc 

OBJS += \
./Astar.o \
./Brain.o \
./GUI.o \
./Node.o \
./belkan.o \
./conexion.o \
./dibujosGL.o \
./environment.o \
./luz.o \
./main.o \
./objeto3d.o \
./retrato.o 

CC_DEPS += \
./objeto3d.d 

CPP_DEPS += \
./Astar.d \
./Brain.d \
./GUI.d \
./Node.d \
./belkan.d \
./conexion.d \
./dibujosGL.d \
./environment.d \
./luz.d \
./main.d \
./retrato.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/valentin/workspace/Belkan/include" -I"/home/valentin/workspace/Belkan/jpeg_reader" -O0 -g3 -Wall -c -fmessage-length=0 -g -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/valentin/workspace/Belkan/include" -I"/home/valentin/workspace/Belkan/jpeg_reader" -O0 -g3 -Wall -c -fmessage-length=0 -g -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


