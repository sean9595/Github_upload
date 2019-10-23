################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Manipulator_Control_003.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Manipulator_Control_003.list \


# All Target
all: Manipulator_Control_003.elf secondary-outputs

# Tool invocations
Manipulator_Control_003.elf: $(OBJS) $(USER_OBJS) C:\Users\ranos\Desktop\Modu_instiute\Embedded\Manipulator_Control_003\STM32H743ZITX_FLASH.ld
	arm-none-eabi-gcc -o "Manipulator_Control_003.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\ranos\Desktop\Modu_instiute\Embedded\Manipulator_Control_003\STM32H743ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Manipulator_Control_003.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Manipulator_Control_003.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Manipulator_Control_003.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
