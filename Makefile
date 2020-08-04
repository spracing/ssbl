##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [2.29.2] date: [Tue Jan 15 17:34:48 CET 2019] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET ?= SPRACINGH7EXTREME

# Compile-time options
OPTIONS   ?=

######################################
# building variables
######################################
# debug build?
# 1 = Build with debug information and minimal optimization
# empty = Builds with optimizations
DEBUG ?=

# optimization
ifeq ($(DEBUG),1)
	OPT_BASE = 
	LD_OPT =
	CC_OPT_BASE = -fno-fast-math
	OPT_DEBUG = #-Og 
	OPT_SIZE = -Os
else
	OPT_DEBUG = -Og
	OPT_DEFAULT = -O2
	OPT_SIZE = -Os
	OPT_SPEED = -Ofast

	CC_OPT_LTO = -flto 
	LD_OPT_LTO = -flto -fuse-linker-plugin $(OPT_SPEED)
	
	LD_OPT = $(LD_OPT_LTO)
	CC_OPT_BASE = -ffast-math
endif
#######################################
# paths
#######################################

SRC_DIR = src
LIB_DIR = lib

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_ROOT = build
BUILD_DIR_BASE = $(BUILD_ROOT)/$(TARGET)
TARGET_BASE = $(BUILD_ROOT)/SSBL_$(TARGET)

ifeq ($(DEBUG),1)
	TARGET_ARTIFACT_BASE := $(TARGET_BASE)_DEBUG
	BUILD_DIR := $(BUILD_DIR_BASE)/DEBUG
else
	TARGET_ARTIFACT_BASE := $(TARGET_BASE)_RELEASE
	BUILD_DIR := $(BUILD_DIR_BASE)/RELEASE
endif

TARGET_ELF = $(TARGET_ARTIFACT_BASE).elf
TARGET_HEX = $(TARGET_ARTIFACT_BASE).hex
TARGET_BIN = $(TARGET_ARTIFACT_BASE).bin

TARGET_EXST_ELF = $(TARGET_ARTIFACT_BASE)_EXST.elf
TARGET_UNPATCHED_BIN = $(TARGET_ARTIFACT_BASE)_UNPATCHED.bin
TARGET_EXST_HASH_SECTION_FILE = $(BUILD_DIR)/exst_hash_section.bin



######################################
# source
######################################

LINKER_DIR = $(SRC_DIR)/main/link

LINKER_SCRIPTS = \
	$(shell find $(LINKER_DIR)/ -type f -name '*.ld')

#
# Main
#

MAIN_ALL_SOURCES = \
	$(shell find $(SRC_DIR)/ -type f -name '*.c')

MAIN_SOURCE_EXCLUDES = \
	$(filter $(SRC_DIR)/main/target/%,$(MAIN_ALL_SOURCES))

MAIN_FILTERED_SOURCES = $(filter-out $(MAIN_SOURCE_EXCLUDES),$(MAIN_ALL_SOURCES))

MAIN_SOURCES = \
	$(MAIN_FILTERED_SOURCES) \
	$(wildcard $(SRC_DIR)/main/target/$(TARGET)/*.c) \

MAIN_INCLUDES = \
	$(SRC_DIR)/main \
	$(SRC_DIR)/main/target/$(TARGET) \

#
# HAL
#

HAL_DIR = $(LIB_DIR)/main/STM32H7xx_HAL_Driver

HAL_SOURCES = \
	$(HAL_DIR)/Src/stm32h7xx_hal_adc.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_adc_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_cortex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_dac.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_dac_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_hash.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_i2c.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_i2c_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_qspi.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_rtc.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_rtc_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_ll_sdmmc.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_sd.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_sd_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_spi.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_spi_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_tim.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_tim_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_uart.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_uart_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_wwdg.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_rcc.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_rcc_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_flash.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_flash_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_gpio.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_hsem.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_dma.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_dma_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_mdma.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_pcd.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_pcd_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_rng.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_pwr.c \
	$(HAL_DIR)/Src/stm32h7xx_hal_pwr_ex.c \
	$(HAL_DIR)/Src/stm32h7xx_hal.c \
	$(HAL_DIR)/Src/stm32h7xx_ll_usb.c \
	$(HAL_DIR)/Src/stm32h7xx_ll_delayblock.c \

ALL_HAL_SOURCES = \
	$(shell find $(HAL_DIR)/ -type f -name '*.c')

HAL_EXCLUDES = \
	$(filter %_template.c,$(ALL_HAL_SOURCES))

FILTERED_HAL_SOURCES = $(filter-out $(HAL_EXCLUDES),$(ALL_HAL_SOURCES))

HAL_INCLUDES = \
	$(HAL_DIR)/Inc \
	$(HAL_DIR)/Inc/Legacy \

CMSIS_INCLUDES = \
	$(LIB_DIR)/main/CMSIS/Device/ST/STM32H7xx/Include \
	$(LIB_DIR)/main/CMSIS/Include \

#
# Sources
#

C_SOURCES =  \
	$(MAIN_SOURCES) \
	$(HAL_SOURCES) \

# ASM sources
ASM_SOURCES =  \
	$(SRC_DIR)/main/startup_stm32h750xx.s \

# Source with optimization requirements
SPEED_OPTIMISED_SOURCES = \

SIZE_OPTIMISED_SOURCES = \

ALWAYS_SIZE_OPTIMISED_SOURCES = \
	$(HAL_SOURCES) \

NOT_OPTIMISED_SRC = \

######################################
# firmware library
######################################
PERIFLIB_SOURCES = \ 

#######################################
# binaries
#######################################

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CXX = $(PREFIX)g++
OBJCOPY = $(PREFIX)objcopy
DUMP = $(PREFIX)objdump
AR = $(PREFIX)ar
SZ = $(PREFIX)size
RE = $(PREFIX)readelf
HEX = $(OBJCOPY) -O ihex
BIN = $(OBJCOPY) -O binary -S

GCC_REQUIRED_VERSION = 9.2.1
GCC_VERSION = $(shell $(CC) -dumpversion)
ifneq ($(GCC_VERSION), $(GCC_REQUIRED_VERSION))
    $(error $(CC) versiom mismatch.  Expected '$(GCC_REQUIRED_VERSION)', found: '$(GCC_VERSION)')
else ifeq ($(GCC_VERSION),)
    $(error $(CC) not found)
endif

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 
# C defines
C_DEFS =  \
	-DUSE_HAL_DRIVER \
	-DSTM32H750xx \
	-DSTM32H7xx \
	-DUSE_USB_FS \
	-D"HSE_VALUE=8000000" \
	-D"HSE_STARTUP_TIMEOUT=250"\
	-DTARGET_$(TARGET) \
	$(addprefix -D,$(OPTIONS)) \

ifeq ($(DEVELOPER_BUILD),YES)
	C_DEFS := $(C_DEFS) -DDEVELOPER_BUILD
endif

# AS includes
AS_INCLUDES = \

# C includes
C_INCLUDES =  \
	$(addprefix -I,$(MAIN_INCLUDES)) \
	$(addprefix -I,$(HAL_INCLUDES)) \
	$(addprefix -I,$(CMSIS_INCLUDES)) \

CC_OPT_SPEED = $(CC_OPT_BASE) $(OPT_SPEED)
CC_OPT_SIZE = $(CC_OPT_BASE) $(OPT_SIZE)
CC_OPT_DEFAULT = $(CC_OPT_BASE) $(OPT_DEFAULT)
CC_OPT_DEBUG = $(CC_OPT_BASE) $(OPT_DEBUG)
CC_OPT_NONE = 

DEBUG_FLAGS = -g -gdwarf-2
#DEBUG_FLAGS = -g -ggdb3

DEBUG_DEFS = \
-DDEBUG \

ifeq ($(DEBUG), 1)
	C_DEFS := $(C_DEFS) $(DEBUG_DEFS)
else
	C_DEFS := $(C_DEFS) -DNDEBUG
endif

# compile gcc flags
ASFLAGS = \
	$(DEBUG_FLAGS) \
	$(MCU) \
	$(AS_DEFS) \
	$(AS_INCLUDES) \
	-Wall \
	-fdata-sections \
	-ffunction-sections \
	-x assembler-with-cpp \
	-MMD -MP \

CFLAGS = \
	$(DEBUG_FLAGS) \
	$(MCU) \
	$(C_DEFS) \
	$(C_INCLUDES) \
	-Wall \
	-fdata-sections \
	-ffunction-sections \
	-fsignaling-nans \
	-fno-unsafe-math-optimizations \
	-std=gnu99 \

CXXFLAGS = \
	$(DEBUG_FLAGS) \
	$(MCU) \
	$(C_DEFS) \
	$(C_INCLUDES) \
	-Wall \
	-fdata-sections \
	-ffunction-sections \
	-fno-exceptions \
	-fsignaling-nans \
	-fno-unsafe-math-optimizations \
	-std=c++11 \
	
XO_CFLAGS = \
	-mpure-code \

NOXO_CFLAGS = \
	$(CC_OPT_LTO) \

# Generate dependency information
DEP_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT $@

GENERATED_ASSEMBER_FLAGS = -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(<:.c=.lst)

#######################################
# LDFLAGS
#######################################

-include $(SRC_DIR)/main/target/$(TARGET)/target.mk
-include $(SRC_DIR)/main/target/$(TARGET)/$(TARGET).mk

ifeq ($(LD_SCRIPT),)
$(error linker script not specfied) 
endif

# libraries
LIBS = -lc -lm -lnosys -lstdc++
LIBDIR = 
LDFLAGS = \
	$(DEBUG_FLAGS) \
	$(MCU) \
	-specs=nano.specs \
	-specs=nosys.specs \
	-T$(LD_SCRIPT) \
	$(LIBDIR) \
	$(LIBS) \
	-static \
	-Wl,-L$(LINKER_DIR) \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	$(LD_OPT) \


#######################################
# goals
#######################################
# default action: build all
all: $(TARGET_ELF) $(TARGET_HEX) $(TARGET_BIN)


#######################################
# build the application
#######################################
# ASM startup code must be listed first due to an issue with LTO, see https://bugs.launchpad.net/gcc-arm-embedded/+bug/1747966

# list of ASM program objects
OBJECTS = $(addsuffix .o, $(addprefix $(BUILD_DIR)/,$(basename $(ASM_SOURCES)))) 
#$(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# list of C program objects
OBJECTS += $(addsuffix .o, $(addprefix $(BUILD_DIR)/,$(basename $(C_SOURCES))))
vpath %.c $(sort $(dir $(C_SOURCES)))


# list of CPP program objects
OBJECTS += $(addsuffix .o, $(addprefix $(BUILD_DIR)/,$(basename $(CXX_SOURCES))))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

DEPS = \
	$(addsuffix .d, $(addprefix $(BUILD_DIR)/,$(basename $(C_SOURCES)))) \
	$(addsuffix .d, $(addprefix $(BUILD_DIR)/,$(basename $(CXX_SOURCES)))) \
	$(addsuffix .d, $(addprefix $(BUILD_DIR)/,$(basename $(ASM_SOURCES)))) \

ifeq ($(DEBUG),1)
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@mkdir -p $(dir $@)
	$(if $(findstring $<,$(NOT_OPTIMISED_SRC)), \
		@echo "CC (none) $<" && \
		$(CC) $(CC_OPT_NONE) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
	, \
		$(if $(findstring $<,$(ALWAYS_SIZE_OPTIMISED_SOURCES)), \
			@echo "CC (size) $<" "$(STDOUT)" && \
			$(CC) $(CC_OPT_SIZE) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
		, \
			@echo "CC (debug) $<" && \
			$(CC) $(CC_OPT_DEBUG) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
		)\
	)
else
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(if $(findstring $<,$(NOT_OPTIMISED_SRC)), \
		@echo "CC (none) $<" "$(STDOUT)" && \
		$(CC) $(CC_OPT_NONE) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
	, \
		$(if $(findstring $<,$(SPEED_OPTIMISED_SOURCES)), \
			@echo "CC (speed) $<" "$(STDOUT)" && \
			$(CC) $(CC_OPT_SPEED) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
		, \
			$(if $(findstring $<,$(SIZE_OPTIMISED_SOURCES) $(ALWAYS_SIZE_OPTIMISED_SOURCES)), \
				@echo "CC (size) $<" "$(STDOUT)" && \
				$(CC) $(CC_OPT_SIZE) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
			, \
				@echo "CC (default) $<" "$(STDOUT)" && \
				$(CC) $(CC_OPT_DEFAULT) -c $(CFLAGS) $(DEP_FLAGS) $(GENERATED_ASSEMBER_FLAGS) $< -o $@\
			)\
		)\
	)
endif

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "AS (default) $<"
	@$(AS) -c $(ASFLAGS) $(DEP_FLAGS) $< -o $@

$(TARGET_ELF): $(OBJECTS) Makefile $(LINKER_SCRIPTS)
	@echo LD $(LD_SCRIPT) $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo RE $@
	@$(RE) -S $@
	@echo SZ $@
	@$(SZ) $@

$(TARGET_UNPATCHED_BIN): $(TARGET_ELF)
	@echo "Creating BIN (without checksum) $(TARGET_UNPATCHED_BIN)" "$(STDOUT)"
	$(OBJCOPY) -O binary $< $@

$(TARGET_BIN): $(TARGET_UNPATCHED_BIN)
	@echo "Creating EXST $(TARGET_BIN)" "$(STDOUT)"
# Linker script should allow .bin generation from a .elf which results in a file that is the same length as the FIRMWARE_SIZE.
# These 'dd' commands will pad a short binary to length FIRMWARE_SIZE.
	dd if=/dev/zero ibs=1k count=$(FIRMWARE_SIZE) of=$(TARGET_BIN)
	dd if=$(TARGET_UNPATCHED_BIN) of=$(TARGET_BIN) conv=notrunc

	@echo "Generating MD5 hash of binary" "$(STDOUT)"
	openssl dgst -md5 $(TARGET_BIN) > $(TARGET_UNPATCHED_BIN).md5 
	
	@echo "Patching MD5 hash into binary" "$(STDOUT)"
	cat $(TARGET_UNPATCHED_BIN).md5 | awk '{printf("%08x: %s",(1024*$(FIRMWARE_SIZE))-16,$$2);}' | xxd -r - $(TARGET_BIN)
	echo $(FIRMWARE_SIZE) | awk '{printf("-s 0x%08x -l 16 -c 16 %s",(1024*$$1)-16,"$(TARGET_BIN)");}' | xargs xxd

# Note: From the objcopy manual "If you do not specify outfile, objcopy creates a temporary file and destructively renames the result with the name of infile"
# Due to this a temporary file must be created and removed, even though we're only extracting data from the input file.
# If this temporary file is NOT used the $(TARGET_ELF) is modified, and running make a second time will result in
# a) regeneration of $(TARGET_BIN), and
# b) the results of $(TARGET_BIN) will not be as expected.
	@echo "Extracting HASH section from unpatched EXST elf $(TARGET_ELF)" "$(STDOUT)"
	$(OBJCOPY) $(TARGET_ELF) $(TARGET_EXST_ELF).tmp --dump-section .exst_hash=$(TARGET_EXST_HASH_SECTION_FILE)
	rm $(TARGET_EXST_ELF).tmp
	
	@echo "Patching MD5 hash into HASH section" "$(STDOUT)"
	cat $(TARGET_UNPATCHED_BIN).md5 | awk '{printf("%08x: %s",64-16,$$2);}' | xxd -r - $(TARGET_EXST_HASH_SECTION_FILE)
	
	@echo "Patching updated HASH section into $(TARGET_EXST_ELF)" "$(STDOUT)"
	$(OBJCOPY) $(TARGET_ELF) $(TARGET_EXST_ELF) --update-section .exst_hash=$(TARGET_EXST_HASH_SECTION_FILE)

$(TARGET_HEX): $(TARGET_BIN)
	$(if $(EXST_ADJUST_VMA),,$(error "EXST_ADJUST_VMA not specified"))

	@echo "Creating EXST HEX from patched EXST BIN $(TARGET_BIN), VMA Adjust $(EXST_ADJUST_VMA)" "$(STDOUT)"
	$(OBJCOPY) -I binary -O ihex --adjust-vma=$(EXST_ADJUST_VMA) $(TARGET_BIN) $@

$(BUILD_DIR):
	mkdir -p $@
#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR_BASE)
	-rm -f $(TARGET_BASE)*.*
  
#######################################
# dependencies
#######################################
-include $(DEPS)

# *** EOF ***
