#--------------------------------------------------------------------------------
# Roseverse Installer - Makefile
# - Erikku Satō
#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment")
endif

ifneq ($(strip $(V)), 1)
Q ?= @
endif

#----------------------------------------
# Public
#----------------------------------------
DEVKITPPC := $(DEVKITPRO)/devkitPPC
WUT_ROOT := $(DEVKITPRO)/wut
PORTLIBS_PATH := $(DEVKITPRO)/portlibs

#----------------------------------------
# Project
#----------------------------------------
include $(WUT_ROOT)/share/wut_rules

TARGETNAME  = roseverse_installer
TARGET      = inkay

BUILDDIR 	= build
LIBSDIR		= libs
EG2DIR		= eg2
TARGETDIR 	= $(BUILDDIR)/$(TARGET)
FSDIR		= fs
OBJDIR    	= $(TARGETDIR)/obj
DEFDIR    	= $(TARGETDIR)/def
DOXYDIR		= doxygen/doxygen

INCLUDE 	= -I$(WUT_ROOT)/include -I$(PORTLIBS_PATH)/wiiu/include -I.
DEFINES		= -D__WUT__
LIBINC		= -L$(WUT_ROOT)/lib -L$(PORTLIBS_PATH)/wiiu/lib -L$(PORTLIBS_PATH)/ppc/lib

SOURCES 	= $(subst ./,,$(shell find $(EG2DIR) $(TARGET) -name "*.cc"))

CC       	= $(DEVKITPPC)/bin/powerpc-eabi-gcc
CXX       	= $(DEVKITPPC)/bin/powerpc-eabi-g++
STRIP 		= $(DEVKITPPC)/bin/powerpc-eabi-strip
ELF2RPL		= $(DEVKITPRO)/tools/bin/elf2rpl
WUHBTOOL 	= $(DEVKITPRO)/tools/bin/wuhbtool
RPLIMPORT 	= $(DEVKITPRO)/tools/bin/rplimportgen

FIND		= find
RM 			= rm -rf
MKDIR 		= mkdir -p
ECHO 		= echo

CODEDIR		= $(FSDIR)/code
CONTENTDIR 	= $(FSDIR)/content
METADIR		= $(FSDIR)/meta

#----------------------------------------
# Flags
#----------------------------------------
CXXBASE    	= -Os -Wall -Wextra $(INCLUDE) $(MACHDEP) $(DEFINES) -MMD -MP
CXXOPT     	= -ffunction-sections -fdata-sections -fmerge-all-constants -fomit-frame-pointer -fvisibility=hidden
CXXOPT1    	= -finline-small-functions -fno-rtti -fno-threadsafe-statics -fmerge-constants -fno-math-errno
CXXOPT2    	= -fno-use-cxa-atexit -fno-thread-jumps -fno-rtti -flto -fno-common
CXXOPT3 	= -fno-ident -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -fno-builtin
CXXFLAGS   	= $(CXXBASE) $(CXXOPT) $(CXXOPT1) $(CXXOPT2) $(CXXOPT3)

LIBS 		= $(LIBINC) -lwut -lcurl -lmbedtls -lmbedcrypto -lmbedx509 -lz -lbrotlicommon -lbrotlidec
LDFLAGS 	= -flto=auto -Wno-odr -Wl,--gc-sections -Wl,--build-id=none -Wl,--Map=$(TARGETDIR)/$(TARGET).map $(ARCH) $(RPXSPECS) $(LIBS)

MENICO      = $(METADIR)/iconTex.png
TVSPLASH    = $(METADIR)/bootTvTex.png
DRCSPLASH   = $(METADIR)/bootDrcTex.png

WUHBFLAGS = \
	--name="Roseverse Installer" \
	--short-name="Roseverse Installer" \
	--author="Project Rose" \
	--icon="$(MENICO)" \
	--tv-image="$(TVSPLASH)" \
	--drc-image="$(DRCSPLASH)" \
	--content="$(CONTENTDIR)"

OBJFILES 	= $(patsubst %,$(OBJDIR)/%,$(SOURCES:.cc=.o))

#----------------------------------------
# Objectives
#----------------------------------------
.SUFFIXES:
.SECONDARY:
.DEFAULT_GOAL = all
.PHONY: all clean elf rpx wuhb

all: $(OBJDIR) $(TARGETDIR)/$(TARGETNAME).elf $(CODEDIR)/$(TARGETNAME).rpx $(BUILDDIR)/$(TARGETNAME).wuhb

#----------------------------------------
# Build
#----------------------------------------
$(OBJDIR):
	$(Q)$(MKDIR) $@

#----------------------------------------
# .O
#----------------------------------------
$(OBJDIR)/%.o: %.cc
	$(Q)$(MKDIR) $(dir $@)
	$(Q)$(ECHO) "$(notdir $<)"
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

#----------------------------------------
# .D
#----------------------------------------
-include $(OBJFILES:.o=.d)

#----------------------------------------
# .ELF
#----------------------------------------
$(TARGETDIR)/$(TARGETNAME).elf: $(OBJFILES)
	$(Q)$(ECHO) "Linking $(TARGETNAME).elf"
	$(Q)$(CXX) $(OBJFILES) $(LDFLAGS) -o $@
	$(Q)$(STRIP) --strip-unneeded $@

#----------------------------------------
# .RPX
#----------------------------------------
$(CODEDIR)/$(TARGETNAME).rpx: $(TARGETDIR)/$(TARGETNAME).elf
	$(Q)$(ECHO) "Building $(TARGETNAME).rpx"
	$(Q)$(ELF2RPL) $< $@

#----------------------------------------
# .WUHB
#----------------------------------------
$(BUILDDIR)/$(TARGETNAME).wuhb: $(CODEDIR)/$(TARGETNAME).rpx
	$(Q)$(ECHO) "Generating $(TARGETNAME).wuhb"
	$(Q)$(WUHBTOOL) $< $@ $(WUHBFLAGS) > /dev/null

#----------------------------------------
# Clean
#----------------------------------------
clean:
	$(Q)$(RM) $(BUILDDIR)
	$(Q)$(RM) $(DOXYDIR)
	$(Q)$(FIND) . -name "*.rpx" -type f -delete