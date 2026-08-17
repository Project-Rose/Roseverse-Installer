#--------------------------------------------------------------------------------
# Roseversé Installer - Makefile
# - Erikku Satō
#--------------------------------------------------------------------------------
# Now builds Custom AuroraChat Client!
#--------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment")
endif

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment")
endif

WUT_ROOT := $(DEVKITPRO)/wut

ifeq ($(strip $(WUT_ROOT)),)
$(error "Please set WUT_ROOT in your environment")
endif

PORTLIBS_PATH := $(DEVKITPRO)/portlibs

ifeq ($(strip $(PORTLIBS_PATH)),)
$(error "Please set PORTLIBS_PATH in your environment")
endif

ifneq ($(strip $(V)), 1)
Q ?= @
endif

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

ELFDATA		= $(TARGETDIR)/$(TARGET).dat
ELFTABLE	= $(TARGETDIR)/$(TARGET).tab
ELFASM		= $(TARGETDIR)/$(TARGET).asm

CC       	= $(DEVKITPPC)/bin/powerpc-eabi-gcc
CXX       	= $(DEVKITPPC)/bin/powerpc-eabi-g++
STRIP 		= $(DEVKITPPC)/bin/powerpc-eabi-strip
OBJDUMP		= $(DEVKITPPC)/bin/powerpc-eabi-objdump
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

LIBS 		= $(LIBINC) -lwut -lcurl -lmbedtls -lmbedcrypto -lmbedx509 -lwut -lz -lbrotlicommon -lbrotlidec
LDFLAGS 	= -flto=auto -Wno-odr -Wl,--gc-sections -Wl,--build-id=none -Wl,--Map=$(TARGETDIR)/$(TARGET).map $(ARCH) $(RPXSPECS) $(LIBS)

WUHBMENICO      = $(METADIR)/iconTex.png
WUHBTVSPLASH    = $(METADIR)/tv-splash.png
WUHBDRCSPLASH   = $(METADIR)/drc-splash.png

WUHBFLAGS = \
	--name="Roseversé Installer" \
	--short-name="Roseversé Installer" \
	--author="Project Rosé" \
	--icon="$(WUHBMENICO)" \
	--tv-image="$(WUHBTVSPLASH)" \
	--drc-image="$(WUHBDRCSPLASH)" \
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
	$(Q)$(OBJDUMP) -h $@ > $(ELFDATA)
	$(Q)$(OBJDUMP) -t $@ > $(ELFTABLE)
	$(Q)$(OBJDUMP) -d $@ > $(ELFASM)
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