WORKING_DIR := $(shell pwd)
CC := /usr/bin/clang++
CFLAGS = -gfull -O0 -std=c++11 -stdlib=libc++ -Wall -fPIC
LDFLAGS = -shared
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_image SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_image SDL_ttf SDL_mixer)

SRC = src/audio.cpp src/font.cpp src/gfx.cpp src/sprite.cpp src/timer.cpp src/fps.cpp
OBJ = audio.o font.o gfx.o sprite.o timer.o fps.o

TARGET = libgamelib.dylib
TARGET_DIR = gamelib
PREFIX = /usr/local

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o $(TARGET)

audio.o: src/audio.cpp src/audio.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/audio.cpp

font.o: src/font.cpp src/font.h src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/font.cpp

gfx.o: src/gfx.cpp src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/gfx.cpp

sprite.o: src/sprite.cpp src/sprite.h src/gfx.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/sprite.cpp

timer.o: src/timer.cpp src/timer.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/timer.cpp

fps.o: src/timer.cpp src/timer.h src/fps.cpp src/fps.h src/gamelib.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/fps.cpp

clean:
	/bin/rm -rf *.o $(TARGET)

app: install

install:
	/bin/ln -sf $(WORKING_DIR)/$(TARGET) $(PREFIX)/lib; /bin/mkdir -p $(PREFIX)/include/$(TARGET_DIR);
	/bin/ln -sf $(WORKING_DIR)/src/audio.h $(PREFIX)/include/$(TARGET_DIR)/audio.h;
	/bin/ln -sf $(WORKING_DIR)/src/font.h $(PREFIX)/include/$(TARGET_DIR)/font.h;
	/bin/ln -sf $(WORKING_DIR)/src/fps.h $(PREFIX)/include/$(TARGET_DIR)/fps.h;
	/bin/ln -sf $(WORKING_DIR)/src/gamelib.h $(PREFIX)/include/$(TARGET_DIR)/gamelib.h;
	/bin/ln -sf $(WORKING_DIR)/src/gfx.h $(PREFIX)/include/$(TARGET_DIR)/gfx.h;
	/bin/ln -sf $(WORKING_DIR)/src/sprite.h $(PREFIX)/include/$(TARGET_DIR)/sprite.h;
	/bin/ln -sf $(WORKING_DIR)/src/timer.h $(PREFIX)/include/$(TARGET_DIR)/timer.h

uninstall:
	/bin/rm $(PREFIX)/lib/$(TARGET);
	/bin/rm -r $(PREFIX)/include/$(TARGET_DIR)

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SDL_CFLAGS) $(SRC)

sublime-cpp:
	subl src/sprite.cpp src/gfx.cpp src/font.cpp src/audio.cpp src/timer.cpp src/fps.cpp
sublime-h:
	subl src/sprite.h src/gfx.h src/font.h src/audio.h src/timer.h src/fps.h

# DO NOT DELETE
