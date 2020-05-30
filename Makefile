#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

# Check for DevkitPro before attempting to compile
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

# TOPDIR stores the top level of the directory (ie. the current folder)
TOPDIR ?= $(CURDIR)
include $(DEVKITPRO)/libnx/switch_rules

#---------------------------------------------------------------------------------
# Options for compilation
# TARGET: Name of the output file(s)
# BUILD: Directory where object files & intermediate files will be placed
# SOURCES: List of directories containing source code
# INCLUDES: List of directories containing header files
# ROMFS: Directory containing data to be added to RomFS
# OUTDIR: Directory to structure sdcard contents
# FDIR: Directory where forwarder makefile/files/output is
# FFILE: Name of forwarder nsp
#---------------------------------------------------------------------------------
TARGET		:=	NX-Activity-Log
BUILD		:=	build
SOURCES		:=	source source/nx source/ui source/ui/screen source/ui/element source/ui/overlay source/utils
INCLUDES	:=	include
ROMFS		:=	romfs
OUTDIR		:=	sdcard
FDIR		:=	forwarder
FFILE		:=	exefs.nsp

#---------------------------------------------------------------------------------
# Application version
#---------------------------------------------------------------------------------
VER_MAJOR	:= 1
VER_MINOR	:= 3
VER_MICRO	:= 1

#---------------------------------------------------------------------------------
# Options for .nacp information
#---------------------------------------------------------------------------------
APP_TITLE   := 	NX Activity Log
APP_AUTHOR	:= 	tallbl0nde
APP_VERSION	:=	$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
ICON 		:= 	img/icon.jpg

#---------------------------------------------------------------------------------
# Options for code generation
# LIBS: Libraries to link against
# (I dunno what the rest is)
#---------------------------------------------------------------------------------
LIBS	:=  -lAether -lcurl -lmbedtls -lmbedx509 -lmbedcrypto -lminizip -lnx `sdl2-config --libs` -lSDL2_ttf `freetype-config --libs` -lSDL2_gfx -lSDL2_image -lpng -ljpeg -lwebp -lSimpleIniParser

ARCH	:=	-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE

CFLAGS	:=	-g -Wall -O2 -ffunction-sections $(ARCH) $(DEFINES) $(INCLUDE) -D__SWITCH__ \
			-DVER_MAJOR=$(VER_MAJOR) -DVER_MINOR=$(VER_MINOR) -DVER_MICRO=$(VER_MICRO) -DVER_STRING=\"$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)\" \
			`freetype-config --cflags` `sdl2-config --cflags`

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(PORTLIBS) $(LIBNX) $(CURDIR)/libs/SimpleIniParser $(CURDIR)/libs/Aether\
			$(CURDIR)/libs/json

#---------------------------------------------------------------------------------
# This is all wizardry to me also
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export LD	:=	$(CXX)

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBDIRS),-I$(dir)/include) -I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

#---------------------------------------------------------------------------------
# End of wizardry
# Set .nro flags
#---------------------------------------------------------------------------------

# Add icon and nacp flags (as these will always be present)
export APP_ICON := $(TOPDIR)/$(ICON)
export NROFLAGS += --icon=$(APP_ICON) --nacp=$(CURDIR)/$(TARGET).nacp --romfsdir=$(CURDIR)/$(ROMFS)

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
# Default build target: everything
#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
ifeq ($(wildcard $(CURDIR)/libs/Aether/LICENSE),)
	@$(error "Please run 'git submodule update --init' before running 'make'")
endif
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) -C $(CURDIR)/libs/SimpleIniParser -f $(CURDIR)/libs/SimpleIniParser/Makefile
	@$(MAKE) -C $(CURDIR)/libs/Aether -f $(CURDIR)/libs/Aether/Makefile
	@$(MAKE) -C $(CURDIR)/$(FDIR) -f $(CURDIR)/$(FDIR)/Makefile
	@cp $(FDIR)/$(FFILE) $(ROMFS)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
# Clean removes all build files + sdcard directory
#---------------------------------------------------------------------------------
clean:
	@echo Cleaning ALL build files...
	@rm -rf $(BUILD) $(TARGET).nro $(TARGET).nacp $(TARGET).elf $(OUTDIR) $(ROMFS)/$(FFILE)
	@$(MAKE) -C $(CURDIR)/libs/SimpleIniParser -f $(CURDIR)/libs/SimpleIniParser/Makefile clean
	@$(MAKE) -C $(CURDIR)/libs/Aether -f $(CURDIR)/libs/Aether/Makefile clean
	@$(MAKE) -C $(CURDIR)/$(FDIR) -f $(CURDIR)/$(FDIR)/Makefile clean
	@echo Done!
#---------------------------------------------------------------------------------
else
.PHONY:	all

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# Main target
#---------------------------------------------------------------------------------
# Important binaries are copied into approporiate places
# (I gave up figuring out how to do it properly)
all: $(OUTPUT).nro
	@mkdir -p $(TOPDIR)/$(OUTDIR)/switch/$(TARGET)
	@cp $(OUTPUT).nro $(TOPDIR)/$(OUTDIR)/switch/$(TARGET)/
	@echo
	@echo "All done! Simply copy the folders within ./$(OUTDIR) to your SD card :)"

$(OUTPUT).nro	:	$(OUTPUT).elf $(OUTPUT).nacp
$(OUTPUT).elf	:	$(OFILES)
$(OFILES_SRC)	: $(HFILES_BIN)

#---------------------------------------------------------------------------------
# you need a rule like this for each extension you use as binary data
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
