# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++11 -Wall \
	$(shell sdl2-config --cflags) \
	$(shell pkg-config --cflags SDL2_image)
LDFLAGS  := $(shell sdl2-config --libs) \
	$(shell pkg-config --libs SDL2_image)

# Sources
SRCS := main.cpp \
	lib/engine/ray.cpp lib/engine/rayCaster.cpp \
	lib/map/map.cpp    lib/map/miniMap.cpp \
	lib/player/player.cpp \
	lib/world/ceiling.cpp lib/world/floor.cpp lib/world/walls.cpp

# Objects & target
OBJS   := $(SRCS:.cpp=.o)
TARGET := RayCasterEngine

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean