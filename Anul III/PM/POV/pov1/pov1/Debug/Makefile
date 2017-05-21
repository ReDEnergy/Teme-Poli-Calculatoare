################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../src/ \
../src/ASF/ \
../src/ASF/common/ \
../src/ASF/common/boards/ \
../src/ASF/common/utils/ \
../src/ASF/common/utils/interrupt/ \
../src/ASF/common/utils/make/ \
../src/ASF/mega/ \
../src/ASF/mega/boards/ \
../src/ASF/mega/boards/stk600/ \
../src/ASF/mega/boards/stk600/rcx_x/ \
../src/ASF/mega/utils/ \
../src/ASF/mega/utils/assembler/ \
../src/config/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../src/ASF/mega/boards/stk600/rcx_x/init.c \
../src/main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
src/ASF/mega/boards/stk600/rcx_x/init.o \
src/main.o

OBJS_AS_ARGS +=  \
src/ASF/mega/boards/stk600/rcx_x/init.o \
src/main.o

C_DEPS +=  \
src/ASF/mega/boards/stk600/rcx_x/init.d \
src/main.d

C_DEPS_AS_ARGS +=  \
src/ASF/mega/boards/stk600/rcx_x/init.d \
src/main.d

OUTPUT_FILE_PATH +=pov1.elf

OUTPUT_FILE_PATH_AS_ARGS +=pov1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler





src/ASF/mega/boards/stk600/rcx_x/%.o: ../src/ASF/mega/boards/stk600/rcx_x/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -DBOARD=STK600_MEGA  -I"../src" -I"../src/ASF/common/boards" -I"../src/ASF/common/utils" -I"../src/ASF/mega/utils" -I"../src/config"  -O1 -fdata-sections -ffunction-sections -fdata-sections -g3 -Wall -mmcu=atmega324pa -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -DBOARD=STK600_MEGA  -I"../src" -I"../src/ASF/common/boards" -I"../src/ASF/common/utils" -I"../src/ASF/mega/utils" -I"../src/config"  -O1 -fdata-sections -ffunction-sections -fdata-sections -g3 -Wall -mmcu=atmega324pa -c -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.2
	$(QUOTE)D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="pov1.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega324pa -Wl,--relax 
	@echo Finished building target: $@
	"D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "pov1.elf" "pov1.hex"
	"D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "pov1.elf" "pov1.eep" || exit 0
	"D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "pov1.elf" > "pov1.lss"
	"D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "pov1.elf" "pov1.srec"
	"D:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-size.exe" "pov1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "pov1.elf" "pov1.a" "pov1.hex" "pov1.lss" "pov1.eep" "pov1.map" "pov1.srec"
	