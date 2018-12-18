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

include target.autogen.in
include package.autogen.in

SHELL=/bin/sh

prefix?=/usr/local
exec_prefix?=$(prefix)

# Executable for user
bindir?=$(exec_prefix)/bin

# Shell executable for administrator
sbindidr?=$(exec_prefix)/sbin

# Executable for program
libexecdir?=$(exec_prefix)/libexec

# Root directory for read-only architecture-independant data files
datarootdir?=$(prefix)/share

# Directory for installing idiosyncratic (particular) read-only
# architecture-independant data files
datadir?=$(datarootdir)

# Directory for installing read-only data files that pertain to a
# single machine
sysconfdir?=$(prefix)/etc

# Directory for installing archiecture-independant data files which
# the programs modify while they run
sharedstatedir?=$(prefix)/com

# Directory for installing architecture-independent data files which
# the programs modifiy while they run
localstatedir?=$(prefix)/var

# Directory for installing data files which the programs modify while
# they run, theat pertain to one specifig machine and persist longer
# thant the execution of the program
runstatedir?=$(localstatedir)/run

# Directory for installing header files to be included by the user
# programs
includedir?=$(prefix)/include

# Directory for installing header files for use with compilers other
# than GCC
oldincludedir?=/usr/include

# Directory for installing documentation files (other than Info)
docdir?=$(datarootdir)/doc/YAROS

# Directory for installing the Info files
infodir?=$(datarootdir)/info

# Directories for installing documentation files
htmldir?=$(docdir)
dvidir?=$(docdir)
pdfdir?=$(docdir)
psdir?=$(docdir)

# Directory for object files and libraries of object code
libdir?=$(exec_prefix)/lib

# Directory for installing any Emacs Lisp files
lispdir?=$(datarootdir)/emacs/site-lisp

# Directory for installing locale-specifig message
localedir?=$(datarootdir)/locale

# Top-level directory for installing man pages
#mandir?=$(datarootdir)/man

# Directory for installing section 1
#man1dir=$(mandir)/man1

# Section 2
#man2dir=$(mandir)/man2

#manext?=.
#man1ext=$(manext)1
#man2ext=$(manext)2

# Directory for the sources being compiled
#srcdir?=

# microcontroller
MCU = atmega324pa
ARCH = atmega

# Target Archive
PACKAGE?=YAROS

TARGET_NAME=lib$(PACKAGE)-$(PACKAGE_VERSION)
TARGET = $(TARGET_DIR)/$(TARGET_NAME).a
TARGET_NAME_STRIPPED=$(TARGET_NAME)-stripped
TARGET_STRIPPED = $(TARGET_DIR)/$(TARGET_NAME_STRIPPED).a

# Directories
TARGET_DIR = lib
INC_DIR = include
BUILD_DIR = build
CONF_DIR = scripts


# Add Kernel Source File
SRC = $(shell find $(SRC_PATH) -iname *.$(SRC_EXT))
OBJ = $(addprefix $(BUILD_DIR)/,$(SRC:%.$(SRC_EXT)=%.$(OBJ_EXT)))

VPATH = \
arch/$(ARCH): \
devices: \
kernel: \
init: \
ipc

SRC_PATH = \
arch/$(ARCH)/ \
devices/ \
kernel/ \
init/ \
ipc/

BUILD_PATH = $(addprefix $(BUILD_DIR)/, $(SRC_PATH))
BUILD_PATH += build/info/ build/html/ build/dvi/ build/pdf/ build/ps/

DIRECTORIES = $(BUILD_DIR) $(TARGET_DIR) $(BUILD_PATH)

# Extensions
SRC_EXT = c
INC_EXT = h
OBJ_EXT = o
DEP_EXT = d

# Include files
INC := $(shell find  $(INC_DIR)/ -iname "*.$(INC_EXT)" -type f)

# Preprocessor
CPPFLAGS = -I$(INC_DIR) -MMD -DVERBOSE_LEVEL=5

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

PROF :=

_CFLAGS :=  \
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
# We only link to avrlibC
LIBS :=
ARFLAGS := rcs

# Utils
RM := rm -rf

# To clean
CLEANER := $(shell find . -iname "*autogen*" -type f) ./config.status ./target.def
CLEAN := $(TARGET_DIR)/ $(BUILD_DIR)/ $(INC_DIR)/kconfig.h

INSTALL_PROGRAM=$(INSTALL)
INSTALL_DATA=${INSTALL} -m 644


# RULES
all: $(DIRECTORIES) $(TARGET)

clean:
	@for item in $(CLEAN); do \
	if [ -f $$item ] || [ -d $$item ] ; then \
		echo "CLEAN $$item" ; \
		$(RM) $$item ; \
	fi \
	done

cleaner: clean
	@for item in $(CLEANER); do \
	if [ -f $$item ] || [ -d $$item ]; then \
		echo "CLEAN $$item" ; \
		$(RM) $$item ; \
	fi \
	done


config:	$(INC_DIR)/kconfig.h
	autogen -L templates templates/package.def

dist:
	@mkdir $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r arch $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r drivers $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r include $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r init $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r ipc $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r kernel $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r scripts $(PACKAGE)-$(PACKAGE_VERSION)
	@cp -r tests $(PACKAGE)-$(PACKAGE_VERSION)
	@cp AUTHORS $(PACKAGE)-$(PACKAGE_VERSION)
	@cp LICENSE $(PACKAGE)-$(PACKAGE_VERSION)
	@cp makefile $(PACKAGE)-$(PACKAGE_VERSION)
	@cp README.org $(PACKAGE)-$(PACKAGE_VERSION)
	@tar -zcf $(PACKAGE)-$(PACKAGE_VERSION).tar.gz $(PACKAGE)-$(PACKAGE_VERSION)
	@rm -rf $(PACKAGE)-$(PACKAGE_VERSION)
	@echo DIST $(PACKAGE)-$(PACKAGE_VERSION).tar.gz


install:
	@install -D -T $(TARGET) $(libdir)/$(TARGET_NAME).a
	@echo INSTALLED $(TARGET_NAME).a in $(libdir)

install-strip: $(TARGET_STRIPPED)
	@install -D -T $(TARGET_STRIPPED) $(libdir)/$(TARGET_NAME_STRIPPED).a
	@echo INSTALLED	$(TARGET_NAME_STRIPPED).a in $(libdir)

uninstall:
	@rm -f $(libdir)/$(TARGET_NAME).a
	@echo DELETED $(libdir)/$(TARGET_NAME).a
	@rm -f $(libdir)/$(TARGET_NAME_STRIPPED).a
	@echo DELETED $(libdir)/$(TARGET_NAME_STRIPPED).a

install-info:

install-html:

install-dvi:

install-pdf:

install-ps:

info: $(DIRECTORIES) build/info/YAROS.info

html: $(DIRECTORIES) build/html/YAROS.html

dvi: $(DIRECTORIES) build/dvi/YAROS.dvi

pdf: $(DIRECTORIES) build/pdf/YAROS.pdf

ps: $(DIRECTORIES) build/ps/YAROS.ps

MAKEINFO = makeinfo
MAKEPDF = texi2pdf

build/info/YAROS.info: doc/YAROS.texi
	@cd build/info && $(MAKEINFO) --info ../../$<

build/html/YAROS.html: doc/YAROS.texi doc/style.css
	@cd build/html && $(MAKEINFO) --no-split --html --css-include=../../doc/style.css ../../$<

build/dvi/YAROS.dvi: doc/YAROS.texi
	@cd build/dvi && $(MAKEINFO) --dvi ../../$<

build/pdf/YAROS.pdf: doc/YAROS.texi
	@cd build/pdf && $(MAKEPDF) --command="@set tex-pdf" ../../$<

build/ps/YAROS.ps: doc/YAROS.texi
	@cd build/ps && $(MAKEINFO) --ps ../../$<

tests: $(TARGET)
	@make -C tests

.PHONY: \
all \
clean \
cleaner \
config \
dist \
pdf \
ps \
dvi \
html \
info \
install \
uninstall \
tests

$(BUILD_DIR)/%/:
	@echo "CREATE $@"
	@mkdir -p $@

$(BUILD_DIR):
	@echo "CREATE $@"
	@mkdir -p $@

$(TARGET_DIR):
	@echo "CREATE $(TARGET_DIR)/"
	@mkdir -p $(TARGET_DIR)

$(TARGET): $(INC_DIR)/kconfig.h $(OBJ)
	@echo "LD $(subst $(TARGET_DIR)/,,$@)"
	@$(AR) $(ARFLAGS) $@ $^

$(TARGET_STRIPPED): $(TARGET)
	@echo "STRIP $< -> $@"
	@avr-strip --remove-section=.comment $< -o $@

$(INC_DIR)/kconfig.h:
	@python3 $(CONF_DIR)/kconfigure

-include $(OBJ:%.$(OBJ_EXT)=%.$(DEP_EXT))

$(BUILD_DIR)/%.$(OBJ_EXT): %.$(SRC_EXT)
	@echo "CC $(subst $(BUILD_DIR)/,,$@)"
	@$(CC) $(CPPFLAGS) $(_CFLAGS) $(CFLAGS) -c $< -o $@

# Autogen
%.autogen:
	@echo "Please execute ./configure before using the makefile"
	@exit 1
