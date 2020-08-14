# This makefile compiles each component of the overall program
# and then copies compiled files to the output directory

# Codes used to change output colour
COLOUR_BOLD		=	\033[1m
COLOUR_DEFAULT	=	\033[0m

# Path to copy forwarder to
FORWARDER_DEST	=	$(CURDIR)/Application/romfs/exefs.nsp
# Path to copy forwarder from
FORWARDER_SRC	=	$(CURDIR)/Forwarder/exefs.nsp

# All rules are virtual
.PHONY: all application clean clean-application clean-forwarder

# Compile everything and copy compiled files
all: application
	@mkdir -p $(CURDIR)/sdcard/switch/NX-Activity-Log
	@cp $(CURDIR)/Application/NX-Activity-Log.nro $(CURDIR)/sdcard/switch/NX-Activity-Log/
	@echo -e '${COLOUR_BOLD}>> Done! Copy the contents of ./sdcard to your SD Card!${COLOUR_DEFAULT}'

# Compile the application (which depends on the forwarder)
application: $(FORWARDER_DEST)
	@echo -e '${COLOUR_BOLD}>> Application${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Application/

# Compile the forwarder and copy to romfs
$(FORWARDER_DEST): $(FORWARDER_SRC)
	@echo Copying Forwarder to Application romfs...
	@cp $(FORWARDER_SRC) $(FORWARDER_DEST)

$(FORWARDER_SRC): FORCE
	@echo -e '${COLOUR_BOLD}>> Forwarder${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Forwarder/

# Clean all build files
clean: clean-application clean-forwarder
	@rm -rf $(CURDIR)/sdcard
	@echo -e '${COLOUR_BOLD}>> All build files removed!${COLOUR_DEFAULT}'

clean-application:
	@echo -e '${COLOUR_BOLD}>> Application${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Application/ clean

clean-forwarder:
	@echo -e '${COLOUR_BOLD}>> Forwarder${COLOUR_DEFAULT}'
	@$(MAKE) -s -C $(CURDIR)/Forwarder/ clean
	@rm -rf $(FORWARDER_DEST)

FORCE: