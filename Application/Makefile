#----------------------------------------------------------------------------------------------------------------------
# Default target is 'all'
#----------------------------------------------------------------------------------------------------------------------
.DEFAULT_GOAL := all
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Check if DEVKITPRO exists in current environment
#----------------------------------------------------------------------------------------------------------------------
ifndef DEVKITPRO
$(error DEVKITPRO is not present in your environment. This can be fixed by sourcing switchvars.sh from /opt/devkitpro/)
endif
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Include switch build toolchain file
#----------------------------------------------------------------------------------------------------------------------
include $(DEVKITPRO)/libnx/switch_rules
#----------------------------------------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# Options for compilation
# TARGET: Name of the output file(s)
# BUILD: Directory where object files & intermediate files will be placed
# INCLUDES: List of directories containing header files
# SOURCES: List of directories containing source code
# ROMFS: Directory containing data to be added to RomFS
# LIBS: Libraries to link against
# LIBDIRS: Directories of libraries
# FORWARDER: Path to forwarder which must be present in order to compile
#---------------------------------------------------------------------------------
TARGET		:=	NX-Activity-Log
BUILD		:=	build
INCLUDES	:=	include
SOURCES		:=	source
ROMFS		:=	romfs
LIBS		:=	-lAether -lcurl -lnx `sdl2-config --libs` -lSDL2_ttf `freetype-config --libs` -lSDL2_gfx -lSDL2_image -lpng -ljpeg -lwebp -lSimpleIniParser
LIBDIRS		:=	$(PORTLIBS) $(LIBNX) $(CURDIR)/libs/Aether $(CURDIR)/libs/json $(CURDIR)/libs/SimpleIniParser
FORWARDER	:=	$(ROMFS)/exefs.nsp

#---------------------------------------------------------------------------------
# Application version
#---------------------------------------------------------------------------------
VER_MAJOR	:= 1
VER_MINOR	:= 5
VER_MICRO	:= 0

#---------------------------------------------------------------------------------
# Options for .nacp information
#---------------------------------------------------------------------------------
APP_TITLE   := 	NX Activity Log
APP_AUTHOR	:= 	tallbl0nde
APP_VERSION	:=	$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)
ICON 		:= 	icon.jpg

#---------------------------------------------------------------------------------
# Options for code generation
#---------------------------------------------------------------------------------
OBJDIR		:=	$(BUILD)/objs
DEPDIR		:=	$(BUILD)/deps
ARCH		:=	-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE
INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) $(foreach dir,$(LIBDIRS),-I$(dir)/include)
ASFLAGS		:=	-g $(ARCH)
LD			:=	$(CXX)
LDFLAGS		:=	-specs=$(DEVKITPRO)/libnx/switch.specs -g $(ARCH)
LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)
NROFLAGS	:=	--icon=$(CURDIR)/$(ICON) --nacp=$(CURDIR)/$(TARGET).nacp --romfsdir=$(CURDIR)/$(ROMFS)
OUTPUT		:=	$(CURDIR)/$(TARGET)

#---------------------------------------------------------------------------------
# Flags to pass to compiler
#---------------------------------------------------------------------------------
DEFINES		:=	-D__SWITCH__ -DVER_MAJOR=$(VER_MAJOR) -DVER_MINOR=$(VER_MINOR) -DVER_MICRO=$(VER_MICRO) -DVER_STRING=\"$(VER_MAJOR).$(VER_MINOR).$(VER_MICRO)\"
CFLAGS		:=	-g -Wall -O2 -ffunction-sections $(ARCH) $(DEFINES) $(INCLUDE) `freetype-config --cflags` `sdl2-config --cflags`
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++2a

#----------------------------------------------------------------------------------------------------------------------
# Definition of variables which store file locations
#----------------------------------------------------------------------------------------------------------------------
CFILES		:= $(foreach dir,$(SOURCES),$(shell find $(dir)/ -name "*.c"))
CPPFILES	:= $(foreach dir,$(SOURCES),$(shell find $(dir)/ -name "*.cpp"))
OFILES		:= $(filter %.o, $(foreach dir,$(SOURCES),$(CPPFILES:$(dir)/%.cpp=$(OBJDIR)/%.o)))
OFILES		+= $(filter %.o, $(foreach dir,$(SOURCES),$(CFILES:$(dir)/%.c=$(OBJDIR)/%.o)))
DEPS		:= $(filter %.d, $(foreach dir,$(SOURCES),$(CPPFILES:$(dir)/%.cpp=$(DEPDIR)/%.d)))
TREE		:= $(sort $(patsubst %/,%,$(dir $(OFILES))))
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Include dependent files if they already exist
#----------------------------------------------------------------------------------------------------------------------
ifeq "$(MAKECMDGOALS)" ""
-include $(DEPS)
endif
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Define few virtual make targets
#----------------------------------------------------------------------------------------------------------------------
.PHONY: all clean libs
#----------------------------------------------------------------------------------------------------------------------
all: $(FORWARDER) $(OUTPUT).nro
$(OUTPUT).nro:	$(OUTPUT).elf $(OUTPUT).nacp
$(OUTPUT).elf:	$(OFILES)
	@echo Linking $(TARGET).elf
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
$(OFILES): | libs

.SECONDEXPANSION:
define crule
$(OBJDIR)/%.o: $(1)/%.c | $$$$(@D)
	@echo Compiling $$*.o...
	@$(CC) -MMD -MP -MF $$(@:$$(OBJDIR)/%.o=$$(DEPDIR)/%.d) $$(CFLAGS) -o $$@ -c $$<
endef
define cpprule
$(OBJDIR)/%.o: $(1)/%.cpp | $$$$(@D)
	@echo Compiling $$*.o...
	@$(CXX) -MMD -MP -MF $$(@:$$(OBJDIR)/%.o=$$(DEPDIR)/%.d) $$(CXXFLAGS) -o $$@ -c $$<
endef

$(foreach source,$(SOURCES),$(eval $(call crule,$(source))))
$(foreach source,$(SOURCES),$(eval $(call cpprule,$(source))))

# Don't build .nro without the forwarder being present
$(FORWARDER):
ifeq ($(wildcard $(CURDIR)/romfs/exefs.nsp),)
	@$(error 'romfs/exefs.nsp' missing: Please run make from the top directory to properly compile the program)
endif

#----------------------------------------------------------------------------------------------------------------------
# 'libs' compiles all libraries
#----------------------------------------------------------------------------------------------------------------------
libs:
ifeq ($(wildcard $(CURDIR)/libs/Aether/LICENSE),)
	@$(error 'Application/libs/Aether' missing: Please run 'git submodule update --init!)
endif
ifeq ($(wildcard $(CURDIR)/libs/SimpleIniParser/LICENSE),)
	@$(error 'Application/libs/SimpleIniParser' missing: Please run 'git submodule update --init!)
endif
	@$(MAKE) -s -C $(CURDIR)/libs/Aether/
	@$(MAKE) -s -C $(CURDIR)/libs/SimpleIniParser/

#----------------------------------------------------------------------------------------------------------------------
# 'clean' removes ALL app build files (but not libraries)
#----------------------------------------------------------------------------------------------------------------------
clean:
	@echo Cleaning Application build files...
	@rm -rf $(BUILD) $(TARGET).nro $(TARGET).nacp $(TARGET).elf
	@$(MAKE) -s -C $(CURDIR)/libs/Aether/ clean
	@$(MAKE) -s -C $(CURDIR)/libs/SimpleIniParser/ clean

#----------------------------------------------------------------------------------------------------------------------
# Define rule recipe `$(TREE)` (creates directories for .o and .d files)
#----------------------------------------------------------------------------------------------------------------------
$(TREE): %:
	@mkdir -p $@
	@mkdir -p $(@:$(OBJDIR)%=$(DEPDIR)%)
