CC = avr-gcc
AC = avr-ar

INC_DIR = include

LIBS =
ARFLAGS = rcs

CPPFLAGS := -I$(INC_DIR) -MMD $(CPPFLAGS)

DIALECT := \
-std=gnu99

WFORMAT := \
-fdiagnostics-color=auto

WARNINGS :=  \
-Wall \
-Wextra \
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
-g2

OPTIMIZATION := \
-Os \
-finline-functions \
-fno-tree-switch-conversion \
-fno-jump-tables

AVR_OPTS = \
-mcall-prologues \
-mmcu=$(MCU)

CODE := \
-fpack-struct

PROF :=

CFLAGS :=  \
$(DIALECT) \
$(WFORMAT) \
$(WARNINGS) \
$(DEBUG) \
$(OPTIMIZATION) \
$(AVR_OPTS) \
$(CODE) \
$(PROF) \
$(CFLAGS)
