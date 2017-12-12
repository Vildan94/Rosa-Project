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
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../doc \
../doc/order \
../src \
../src/cpu \
../src/cpu/linkscript \
../src/cpu/startup \
../src/drivers \
../src/include \
../src/include/drivers \
../src/include/kernel \
../src/kernel


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../src/kernel/rosa_testc_sem.c \
../src/kernel/rosa_semaphores.c \
../src/main.c \
../src/kernel/rosa_int.c \
../src/kernel/rosa_ker.c \
../src/kernel/rosa_scheduler.c \
../src/kernel/rosa_tim.c


PREPROCESSING_SRCS +=  \
../src/kernel/rosa_int_asm.S \
../src/kernel/rosa_ker_asm.S


ASM_SRCS += 


OBJS +=  \
src/kernel/rosa_testc_sem.o \
src/kernel/rosa_semaphores.o \
src/main.o \
src/kernel/rosa_int.o \
src/kernel/rosa_int_asm.o \
src/kernel/rosa_ker.o \
src/kernel/rosa_ker_asm.o \
src/kernel/rosa_scheduler.o \
src/kernel/rosa_tim.o

OBJS_AS_ARGS +=  \
src/kernel/rosa_testc_sem.o \
src/kernel/rosa_semaphores.o \
src/main.o \
src/kernel/rosa_int.o \
src/kernel/rosa_int_asm.o \
src/kernel/rosa_ker.o \
src/kernel/rosa_ker_asm.o \
src/kernel/rosa_scheduler.o \
src/kernel/rosa_tim.o

C_DEPS +=  \
src/kernel/rosa_testc_sem.d \
src/kernel/rosa_semaphores.d \
src/main.d \
src/kernel/rosa_int.d \
src/kernel/rosa_int_asm.d \
src/kernel/rosa_ker.d \
src/kernel/rosa_ker_asm.d \
src/kernel/rosa_scheduler.d \
src/kernel/rosa_tim.d

C_DEPS_AS_ARGS +=  \
src/kernel/rosa_testc_sem.d \
src/kernel/rosa_semaphores.d \
src/main.d \
src/kernel/rosa_int.d \
src/kernel/rosa_int_asm.d \
src/kernel/rosa_ker.d \
src/kernel/rosa_ker_asm.d \
src/kernel/rosa_scheduler.d \
src/kernel/rosa_tim.d

OUTPUT_FILE_PATH +=ROSA.elf

OUTPUT_FILE_PATH_AS_ARGS +=ROSA.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../src/cpu/linkscript/link_uc3a0512.lds


# AVR32/GNU C Compiler















src/kernel/%.o: ../src/kernel/%.c
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE)  -x c -I"../src" -I"../src/include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -O0 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc3a0512 -c -c -fmessage-length=0 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE)  -x c -I"../src" -I"../src/include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -O0 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc3a0512 -c -c -fmessage-length=0 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler

src/kernel/%.o: ../src/kernel/%.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Preprocessing Assembler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE) -x assembler-with-cpp -c -mpart=uc3a0512 -I "../src" -I "../src/include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	

src/kernel/%.o: ../src/kernel/%.x
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Preprocessing Assembler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE) -x assembler-with-cpp -c -mpart=uc3a0512 -I "../src" -I "../src/include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	

src/kernel/%.o: ../src/kernel/%.X
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Preprocessing Assembler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE) -x assembler-with-cpp -c -mpart=uc3a0512 -I "../src" -I "../src/include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	

src/kernel/%.o: ../src/kernel/%.sx
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Preprocessing Assembler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE) -x assembler-with-cpp -c -mpart=uc3a0512 -I "../src" -I "../src/include" -I "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3A_DFP\1.0.53\include\AT32UC3A0512"  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -Wa,-g   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR32/GNU Linker : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -nostartfiles -Wl,-Map="ROSA.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections --rodata-writable -Wl,--direct-data -mpart=uc3a0512 ../src/drivers/button.o ../src/drivers/delay.o ../src/drivers/gpio.o ../src/drivers/led.o ../src/drivers/pot.o ../src/drivers/usart.o ../src/kernel/rosa_tim_asm.o ../src/cpu/startup/crt0.o -T../src/cpu/linkscript/link_uc3a0512.lds  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature "ROSA.elf" "ROSA.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "ROSA.elf" "ROSA.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objdump.exe" -h -S "ROSA.elf" > "ROSA.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "ROSA.elf" "ROSA.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-size.exe" "ROSA.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "ROSA.elf" "ROSA.a" "ROSA.hex" "ROSA.lss" "ROSA.eep" "ROSA.map" "ROSA.srec" "ROSA.usersignatures"
	