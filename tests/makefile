
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




# Microcontroller
MCU = atmega324pa


# Project
TARGET = $(BIN_DIR)/test
AVRDUDE_PROGRAMMERID := usbasp # Target ID
HEXROMTRG = $(TARGET).hex
HEX_TARGET = $(HEXROMTRG) $(OBJ_DIR)/$(TARGET).ee.hex


# Directories
BIN_DIR = bin
INC_DIR = include
LIB_DIR = lib
OBJ_DIR = build

SRC = $(shell find . -name *.$(SRC_EXT))
OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:%.$(SRC_EXT)=%.$(OBJ_EXT))))

VPATH = src

# Extension
SRC_EXT = c
INC_EXT = h
OBJ_EXT = o
DEP_EXT = d

# Sources and Objects
INC := $(shell echo $(INC_DIR)/*.$(INC_EXT))

# Preprocessor
CPPFLAGS = -I$(INC_DIR) -I../include


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
-finline-functions

AVR_OPTS :=  \
-mmcu=$(MCU) \
-mcall-prologues

CODE := \
-fpack-struct

PROF := \
-fstack-usage \
-fprofile-use

CFLAGS :=  \
$(DIALECT) \
$(WFORMAT) \
$(WARNINGS) \
$(DEBUG) \
$(OPTIMIZATION) \
$(AVR_OPTS) \
$(CODE) \
$(PROF)


LIBS := -lYAROS-$(MCU)-1.1
LDFLAGS :=  -L../lib -Wl,-Map,$(TARGET).map -mmcu=$(MCU)


OBJCOPY := avr-objcopy
AVRDUDE := avrdude


# Utils
RM := rm -rf
HEXFORMAT := ihex


# To clean
CLEANER := $(OBJ_DIR)/
CLEAN := $(BIN_DIR)/

all: directories $(TARGET)

directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	@echo "CREATE $(OBJ_DIR)/"
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@echo "CREATE $(BIN_DIR)/"
	@mkdir -p $(BIN_DIR)

# Production des fichiers hex a partir des fichiers elf
$(BIN_DIR)/%.hex: $(TARGET)
	@echo "AVR COPY"
	@$(OBJCOPY) -j .text -j .data -O $(HEXFORMAT) $< $@

$(TARGET): $(OBJ) ../lib/libYAROS-atmega324pa-1.0.0.a
	@echo "LD $^ $(LIBS) -> $@"
	@$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	@avr-strip --remove-section=.comment $@ -o $@-stripped

../lib/libYAROS-atmega324pa-1.0.0.a:
	@echo "MAKE .."
	@make -s -C ..

-include $(OBJ:%.$(OBJ_EXT)=%.$(DEP_EXT))

$(OBJ_DIR)/%.$(OBJ_EXT): %.$(SRC_EXT)
	@echo "CC $< -> $@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

install: $(HEXROMTRG)
	@echo "AVR $(MCU)"
	@$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) -p $(MCU) -P -e -U flash:w:$(HEXROMTRG)

clean:
	@echo "CLEAN $(CLEAN)"
	@$(RM) $(CLEAN)

cleaner: clean
	@echo "CLEAN $(CLEANER)"
	@$(RM) $(CLEANER)


.PHONY: all clean cleaner
