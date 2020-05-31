PROJECT_NAME     := uart_pca10040
TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := _build

PROJ_DIR := src

LINKER_DIR := ldscripts

$(OUTPUT_DIRECTORY)/nrf52832_xxaa.out: \
  LINKER_SCRIPT  := $(LINKER_DIR)/gcc_nrf52.ld

# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/libraries/nRF52_Drivers/nrf52/gcc_startup_nrf52.S \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/nrf52/system_nrf52.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/clock/clock.c \
  $(PROJ_DIR)/serial_port/serial_port.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/spis/spis.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/gpio/gpio.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/uart/uart.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/radio/radio.c \
  $(PROJ_DIR)/libraries/Utils/utils.c \
  $(PROJ_DIR)/libraries/nRF52_Drivers/rtc/rtc.c \
  $(PROJ_DIR)/packets/packets.c \
  $(PROJ_DIR)/network/network.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR)/ \
  $(PROJ_DIR)/libraries \
  $(PROJ_DIR)/libraries/nRF52_Drivers/toolchain/cmsis/include \
  $(PROJ_DIR)/libraries/nRF52_Drivers/nrf52 \
  

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O0 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_74
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall #-Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)
# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52832_XXAA
ASMFLAGS += -DNRF52_PAN_74

# Linker flags
LDFLAGS += $(OPT)
#LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

nrf52832_xxaa: CFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: CFLAGS += -D__STACK_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__HEAP_SIZE=8192
nrf52832_xxaa: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: nrf52832_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52832_xxaa
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

#TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc
TEMPLATE_PATH := .

#include $(TEMPLATE_PATH)/Makefile.common
include Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall

#SDK_CONFIG_FILE := ../config/sdk_config.h
#CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
#sdk_config:
#	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
