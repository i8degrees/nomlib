CFLAGS=/usr/bin/clang++ -std=c++11 -stdlib=libc++ -DNOM_DEBUG -DNOM_DEBUG_ASSERT -DNOM_DEBUG_TRACE -I../include -F../third-party/osx -F../build -I../include/nomlib/json/jsoncpp/
SDL_CFLAGS=-I../third-party/osx/SDL2.framework/Headers -I../third-party/osx/SDL2_image.framework/Headers -I../third-party/osx/SDL2_ttf.framework/Headers
LDFLAGS=-framework nomlib -framework SDL2 -framework SDL2_image -framework SDL2_ttf

all: test

system:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} system.cpp -o system

math:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} math.cpp -o math

graphics:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} -framework SDL2_image graphics.cpp -o graphics

test: system math graphics
