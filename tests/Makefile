EXTRA_FLAGS="-DNOM_DEBUG_TESTS"

CFLAGS=/usr/bin/clang++ -std=c++11 -stdlib=libc++ ${EXTRA_FLAGS} -DNOM_DEBUG -DNOM_DEBUG_ASSERT -DNOM_DEBUG_TRACE -I../include -F../third-party/osx -F../build -I../include/nomlib/json/jsoncpp/
SDL_CFLAGS=-I../third-party/osx/SDL2.framework/Headers -I../third-party/osx/SDL2_image.framework/Headers -I../third-party/osx/SDL2_ttf.framework/Headers
LDFLAGS=-framework nomlib -framework SDL2 -framework SDL2_image -framework SDL2_ttf

all: test

system:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} system-tests.cpp -o system-tests

math:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} math-tests.cpp -o math-tests

graphics:
	${CFLAGS} ${SDL_CFLAGS} ${LDFLAGS} -framework SDL2_image graphics-tests.cpp -o graphics-tests

run:
	./run.sh

test: system-tests math-tests graphics-tests
