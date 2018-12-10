
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Olivier Dion <olivier.dion@polymtl.ca>

SHELL=/bin/sh

# Microcontroller
MCU = atmega324pa
ARCH = atmega

# Device's Driver
DD = $(MCU)

# Target Archive
TARGET = $(TARGET_DIR)/libYAROS.a

# Directories
TARGET_DIR = lib
INC_DIR = include
OBJ_DIR = build
CONF_DIR = scripts

# MCU Device's Drivers
SRC =$(shell for dd in $(DD) ; do find . -iname *$$dd*.$(SRC_EXT); done)

# Add Kernel Source File
SRC += $(shell echo kernel/*.$(SRC_EXT))
SRC += $(shell echo init/*.$(SRC_EXT))
SRC += $(shell echo arch/$(ARCH)/*.$(SRC_EXT))
SRC += $(shell echo ipc/*.$(SRC_EXT))

OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:%.$(SRC_EXT)=%.$(OBJ_EXT))))

VPATH = \
arch/$(ARCH): \
drivers/adc: \
drivers/clk: \
drivers/irq: \
drivers/twi: \
drivers/usart: \
kernel: \
init: \
ipc

# Extensions
SRC_EXT = c
INC_EXT = h
OBJ_EXT = o
DEP_EXT = d

# Sources and Objects
INC := $(shell echo $(INC_DIR)/*.$(INC_EXT))

# Preprocessor
CPPFLAGS = -I$(INC_DIR) -MMD

# Compiler
CC := avr-gcc

DIALECT := \
-std=gnu99 \
-funsigned-bitfields \
-funsigned-char

WFORMAT := \
-fmessage-length=72 \
-fdiagnostics-color=auto

WARNINGS :=  \
-Wall \
-Wnonnull \
-Wfloat-equal \
-Wendif-labels \
-Wshadow \
-Wlarger-than-512 \
-Wcast-align \
-Wsign-compare \
-Waggregate-return \
-Wpadded \
-Wredundant-decls \
-Winline

DEBUG := \
-g3

OPTIMIZATION := \
-Os \
-finline-functions \
-fno-tree-switch-conversion \
-fno-jump-tables

AVR_OPTS :=  \
-mmcu=$(MCU) \
-mcall-prologues

CODE := \
-fpack-struct

PROF := \
-fstack-usage

CFLAGS :=  \
$(DIALECT) \
$(WFORMAT) \
$(WARNINGS) \
$(DEBUG) \
$(OPTIMIZATION) \
$(AVR_OPTS) \
$(CODE) \
$(PROF)

# Linker
AC = avr-ar
LIBS :=
LDFLAGS :=

# Utils
RM := rm -rf

# To clean
CLEANER := $(OBJ_DIR)/ $(INC_DIR)/kconfig.h
CLEAN := $(TARGET_DIR)/



# RULES

all: directories $(INC_DIR)/kconfig.h $(TARGET)

directories: $(OBJ_DIR) $(TARGET_DIR)

$(OBJ_DIR):
	@echo "CREATE $(OBJ_DIR)/"
	@mkdir -p $(OBJ_DIR)

$(TARGET_DIR):
	@echo "CREATE $(TARGET_DIR)/"
	@mkdir -p $(TARGET_DIR)

$(TARGET): $(OBJ)
	@echo "LD $(subst $(TARGET_DIR)/,,$@)"
	@$(AR) rcs $@ $^


$(INC_DIR)/kconfig.h:
	@python3 $(CONF_DIR)/kconfigure

-include $(OBJ:%.$(OBJ_EXT)=%.$(DEP_EXT))

$(OBJ_DIR)/%.$(OBJ_EXT): %.$(SRC_EXT)
	@echo "CC $(subst $(OBJ_DIR)/,,$@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "CLEAN $(CLEAN)"
	@$(RM) $(CLEAN)

cleaner: clean
	@echo "CLEAN $(CLEANER)"
	@$(RM) $(CLEANER)

config:	$(INC_DIR)/kconfig.h


tests:
	@make -C tests


.PHONY: all clean cleaner config tests
