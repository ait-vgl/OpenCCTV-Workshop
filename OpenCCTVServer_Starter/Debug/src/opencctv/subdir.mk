################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/Exception.cpp \
../src/opencctv/ServerController.cpp 

OBJS += \
./src/opencctv/Exception.o \
./src/opencctv/ServerController.o 

CPP_DEPS += \
./src/opencctv/Exception.d \
./src/opencctv/ServerController.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/%.o: ../src/opencctv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


