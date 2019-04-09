PACKAGE=YAROS
PACKAGE_VERSION=1.1
TARGET_NAME=lib$(PACKAGE)-$(MCU)-$(PACKAGE_VERSION)
TARGET = lib/$(TARGET_NAME).a
TARGET_NAME_STRIPPED=$(TARGET_NAME)-stripped
TARGET_STRIPPED = lib/$(TARGET_NAME_STRIPPED).a

MKDIR=build lib

define DO_SRC
	$(filter-out $(GEN_BISON),$(wildcard $1/*.c))
endef

define GEN_MKDIR
	build/$1
endef

SRC_DIR= \
arch/atmega \
devices/adc \
devices/clk \
devices/irq \
devices/twi \
devices/usart \
init \
ipc \
kernel
#BISON=src/parser.y
#GEN_BISON=$(BISON:%.y=%.c)
#CLEAN_BISON=$(GEN_BISON) $(GEN_BISON:%.c=%.h)

MKDIR:=$(MKDIR) $(foreach s,$(SRC_DIR),$(call GEN_MKDIR,$s))

SRC=$(GEN_BISON) $(foreach s,$(SRC_DIR),$(call DO_SRC,$s))
OBJ=$(SRC:%.c=build/%.o)
