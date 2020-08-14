# This makefile compiles each component of the overall program
# and then copies compiled files to the output directory

# Codes used to change output colour
COLOUR_BOLD		=	\033[1m
COLOUR_DEFAULT	=	\033[0m

# All rules are virtual
.PHONY: all build clean

# Compile everything and copy compiled files
all: build
	@mkdir -p $(CURDIR)/sdcard/switch/NX-Activity-Log
	@cp $(CURDIR)/Application/NX-Activity-Log.nro $(CURDIR)/sdcard/switch/NX-Activity-Log/

# Compile both the application and forwarder, doing the forwarder first
# (as it is need in the romfs of the application)
build:
	@echo -e '${COLOUR_BOLD}>> Forwarder${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Forwarder/
	@echo -e '${COLOUR_BOLD}>> Application${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Application/
	@echo -e '${COLOUR_BOLD}>> Done!${COLOUR_DEFAULT}'

# Clean all build files
clean:
	@rm -rf $(CURDIR)/sdcard
	@echo -e '${COLOUR_BOLD}>> Forwarder${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Forwarder/ clean
	@echo -e '${COLOUR_BOLD}>> Application${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Application/ clean
	@echo -e '${COLOUR_BOLD}>> Done!${COLOUR_DEFAULT}'