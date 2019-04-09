RM = rm -rf
INSTALL_PROGRAM=$(INSTALL)
INSTALL_DATA=${INSTALL} -m 644

-include local.mk
include mk/std.mk
include mk/cc.mk
include mk/src.mk

VERBOSE_LEVEL?=1

CLEAN=$(MKDIR)

ifndef VERBOSE
.SILENT:
endif

define DO_MKDIR
	if [ ! -d $1 ]; then $(QUIET_MKDIR); mkdir -p $1; fi
endef

V	      = @
Q	      = $(V:1=)
QUIET_CC      = $(Q:@=@echo    '     CC       '$@;)
QUIET_GEN     = $(Q:@=@echo    '     GEN      '$@;)
QUIET_LINK    = $(Q:@=@echo    '     LINK     '$@;)
QUIET_YACC    = $(Q:@=@echo    '     YACC     '$@;)
QUIET_MKDIR   =        echo    '     MKDIR    '$1
QUIET_RMF     =        echo    '     RM       '
QUIET_RMD     =        echo    '     RMDIR    '


# RULES
all: $(MKDIR) $(TARGET)

$(MKDIR):
	$(foreach d,$@,$(call DO_MKDIR,$d))

$(TARGET): $(OBJ)
	@echo "LD $(subst lib,,$@)"
	@$(AR) $(ARFLAGS) $@ $^

$(TARGET_STRIPPED): $(TARGET)
	@echo "STRIP $< -> $@"
	@avr-strip --remove-section=.comment $< -o $@

-include $(OBJ:%.o=%.d)

build/%.o: %.c
	$(QUIET_CC)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	for item in $(CLEAN); do \
	if [ -f $$item ] ; then \
		$(QUIET_RMF) $$item; \
		$(RM) $$item ; \
        elif [ -d $$item ] ; then \
		$(QUIET_RMD) $$item ; \
                $(RM) -r $$item ; \
	fi \
	done

#config:	$(INC_DIR)/kconfig.h

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

print-%:
	@echo $* = $($*)

.PHONY: \
all \
clean \
config \
dist \
doc \
install \
uninstall \
print
