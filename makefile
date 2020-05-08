# make all 			Build the package
# make run			Builds and runs the program
# make clean 		Cleans results of the building and process

# Build Info
EXE			= main
CDEBUG		= -O2
CXXFLAGS	= $(CDEBUG) -Wall -Wextra $(CFLAGS_PLATFORM) -std=c++17 -O2 -w
LDFLAGS		= -lmenu -lncurses $(LDFLAGS_PLATFORM)
INCLUDEDIRS = -I"src/"
LIBSDIR		=

# Project source files
CFILES = $(shell find src -type f -name '*.c')
CXXFILES = $(shell find src -type f -name '*.cpp')
OBJECTS = $(CFILES:.c=.o) \
		  $(CXXFILES:.cpp=.o)

# Verbose mode check
ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

# Make targets
all: dirs $(EXE)
	# Build sucessful!

$(EXE): $(OBJECTS)
	# Linking...
	$(MUTE)$(CXX) $(OBJECTS) -o bin/$(EXE) $(LIBSDIR) $(LDFLAGS)

src/%.o: src/%.cpp
	# Compiling $<
	$(MUTE)$(CXX) $(CXXFLAGS) $(CDEBUG) $< -c -o $@ $(INCLUDEDIRS)

run: all
	# Running...
	$(MUTE)./bin/$(EXE)

clean:
	# Cleaning object files...
	$(MUTE)rm $(VTAG) -f $(OBJECTS)	
	$(MUTE)rm $(VTAG) -f bin/$(EXE)

dirs:
	$(MUTE)mkdir -p bin
	$(MUTE)mkdir -p Data
	
.PHONY: clean dirs