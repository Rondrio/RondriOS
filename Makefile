CXX = g++
CXXLIBS = -lX11 -lXft -ltinyxml2 -lcairo
CXXINCL = -I/usr/include/freetype2

TARGET = LCARS

FILES = $(wildcard src/*.cc) $(wildcard src/components/*.cc) $(wildcard src/interfaces/*.cc)
OBJECTS = $(patsubst src/%.cc,bin/%.o,$(FILES))

#all: LCARS

LCARS:
	$(CXX) $(FILES) -o LCARS -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf -lX11 -ltinyxml2

$(shell mkdir -p bin/components)

bin/%.o: src/%.cc
	$(CXX) -c $(CXXINCL) $< -o $@
	
all: $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXINCL) $(CXXLIBS) -o bin/LCARS
