# CC ?= C:/msys64/ucrt64/bin/g++.exe
CC ?= g++
CFLAGS := -g -std=c++17 -I.
LDFLAGS := -Llib
LIBS := -lglfw3 -lopengl32 -lstdc++

SRCS := main.cpp glad.c shader.cpp character.cpp cube.cpp stb_impl.cpp textureHandler.cpp chunk.cpp
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJS := $(patsubst %.c,%.o,$(OBJS))

TARGET := main.exe

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
