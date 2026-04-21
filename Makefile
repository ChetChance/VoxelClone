# CXX ?= C:/msys64/ucrt64/bin/g++.exe
CXX ?= g++
UNAME_S := $(shell uname -s)

CPPFLAGS := -I.
CXXFLAGS := -g -std=c++17
CFLAGS := -g
LDFLAGS :=
LIBS :=

ifeq ($(UNAME_S),Darwin)
	BREW_PREFIX := $(shell brew --prefix 2>/dev/null)
	ifneq ($(BREW_PREFIX),)
		CPPFLAGS += -I$(BREW_PREFIX)/include
		LDFLAGS += -L$(BREW_PREFIX)/lib
	endif
	LIBS += -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	TARGET := main
else ifeq ($(UNAME_S),Linux)
	LDFLAGS += -Llib
	LIBS += -lglfw -lGL -ldl -lpthread -lX11 -lXrandr -lXi
	TARGET := main
else
	LDFLAGS += -Llib
	LIBS += -lglfw3 -lopengl32 -lstdc++
	TARGET := main.exe
endif

SRCS := main.cpp glad.c shader.cpp character.cpp cube.cpp stb_impl.cpp textureHandler.cpp chunk.cpp
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJS := $(patsubst %.c,%.o,$(OBJS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CXX) $(CPPFLAGS) $(CFLAGS) -x c -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo "Targets:"
	@echo "  make        Build $(TARGET)"
	@echo "  make run    Build and run"
	@echo "  make clean  Remove build artifacts"
	@echo ""
	@echo "Common overrides:"
	@echo "  make CXX=clang++"
	@echo "  make CPPFLAGS='-I. -I<path>'"
	@echo "  make LDFLAGS='-L<path>'"
	@echo "  make LIBS='-lglfw -lGL'"