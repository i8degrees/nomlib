CFLAGS=/usr/bin/clang++ -std=c++11 -stdlib=libc++ -I../include -D NOM_DEBUG -F${HOME}/Library/Frameworks -F/Library/Frameworks -I${HOME}/Library/Frameworks/SDL2_image.framework/Headers
LDFLAGS=-framework nomlib -framework SDL2

all: test

system:
	${CFLAGS} ${LDFLAGS} system.cpp -o system

math:
	${CFLAGS} ${LDFLAGS} math.cpp -o math

graphics:
	${CFLAGS} ${LDFLAGS} -framework SDL2_image graphics.cpp -o graphics

test: system math graphics
