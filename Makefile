CXX ?= g++

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = out

# extensions #
SRC_EXT = cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
SOURCES_NOTMAIN := $(filter-out src/main.$(SRC_EXT),$(SOURCES))
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -Wall -Wextra -g
INCLUDES = -I include/ -I /usr/local/include 
# Space-separated pkg-config libraries used by this project
LIBS = 

include version.mk

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -MP -MMD -c $< -o $@

# Echo the version numbers into version.h 
update_version: 
	@echo "#pragma once" > include/version.h
	@echo "#define VERSION_MAJOR $(VERSION_MAJOR)" >> include/version.h
	@echo "#define VERSION_MINOR $(VERSION_MINOR)" >> include/version.h
	@echo "#define VERSION_PATCH $(VERSION_PATCH)" >> include/version.h
	@echo 'static const char* VERSION_STRING = "$(VERSION_STRING)";' >> include/version.h
	@echo "Version: $(VERSION_STRING) updated!"

.PHONY: tests
tests:
	g++ tests/testmain.cpp -c $(INCLUDES) $(LIBS) -o $(BUILD_PATH)/testmain.o
	g++ $(BUILD_PATH)/testmain.o $(INCLUDES) $(LIBS) $(SOURCES_NOTMAIN) -o $(BUILD_PATH)/tests && $(BUILD_PATH)/tests -r compact
