/******************************************************************************

    Configuration file for nomlib library

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_CONFIG_HEADERS
#define NOMLIB_CONFIG_HEADERS

#include <iostream>
#include "nomlib_types.hpp"

// nomlib version
#define NOMLIB_VERSION_MAJOR 0
#define NOMLIB_VERSION_MINOR 0

// Identification the operating system
#if defined ( _WIN32) || defined ( __WIN32__ )
  #define NOMLIB_SYSTEM_WINDOWS
#elif defined ( linux ) || defined ( __linux )
  #define NOMLIB_SYSTEM_LINUX
#elif defined ( __APPLE__ ) || defined ( MACOSX ) || defined ( macintosh ) || defined ( Macintosh )
  #define NOMLIB_SYSTEM_OSX
#else
  #warning This operating system is not officially supported by nomlib
#endif


// No debugging logged
#define NOMLIB_DEBUG_0 0

// Level 1 is partial verbosity; class errors are logged
#define NOMLIB_DEBUG_1 1

// Level 2 is full verbosity; includes logging of each class entry and exit
#define NOMLIB_DEBUG_2 2

// Pretty print C macro functions
//
// Is there a better way of doing this?
#define NOMLIB_DEBUG std::cout << __FILE__ << ":" << __LINE__ << " " << __PRETTY_FUNCTION__ << std::endl << std::endl;

#define NOMLIB_DEBUG_WITH_SDL std::cout << __FILE__ << ":" << __LINE__ << " " << __PRETTY_FUNCTION__ << SDL_GetError << std::endl << std::endl;

#define NOMLIB_DEBUG_OBJ std::cout << __PRETTY_FUNCTION__ << std::endl;

#define DEBUG_TRANSFORMABLE
//#define DEBUG_TRANSFORMABLE_OBJ

#define DEBUG_COLOR
//#define DEBUG_COLOR_OBJ

#define DEBUG_COORDS
//#define DEBUG_COORDS_OBJ

#define DEBUG_AUDIO
#define DEBUG_AUDIO_OBJ

#define DEBUG_SDL_FONT
#define DEBUG_SDL_FONT_OBJ

#define DEBUG_BITMAP_FONT
#define DEBUG_BITMAP_FONT_OBJ

//#define DEBUG_SDL_GRADIENT
#define DEBUG_SDL_GRADIENT_OBJ

#define DEBUG_SPRITE
#define DEBUG_SPRITE_OBJ

#define DEBUG_SDL_INPUT
#define DEBUG_SDL_INPUT_OBJ

//#define DEBUG_ISTATE
#define DEBUG_ISTATE_OBJ

#define DEBUG_OSXFS
//#define DEBUG_OSXFS_OBJ

#define DEBUG_SDL_TIMER
//#define DEBUG_SDL_TIMER_OBJ

#define DEBUG_TIMER_FPS
//#define DEBUG_TIMER_FPS_OBJ

#define DEBUG_SDL_MESSAGEBOX
#define DEBUG_SDL_MESSAGEBOX_OBJ

#define DEBUG_SDL_CURSOR
//#define DEBUG_SDL_CURSOR_OBJ

//#define DEBUG_IDRAWABLE_OBJ
//#define DEBUG_SDL_DRAWABLE_OBJ

#define DEBUG_SDL_PIXEL
//#define DEBUG_SDL_PIXEL_OBJ

#define DEBUG_SDL_LINE
//#define DEBUG_SDL_LINE_OBJ

#define DEBUG_SDL_RECT
//#define DEBUG_SDL_RECT_OBJ

#define DEBUG_FONT
#define DEBUG_FONT_OBJ

#define DEBUG_SDL_DISPLAY
#define DEBUG_SDL_DISPLAY_OBJ

#define DEBUG_SDL_CANVAS
#define DEBUG_SDL_CANVAS_OBJ

#define DEBUG_GAMESTATES
#define DEBUG_GAMESTATES_OBJ

#define DEBUG_SDL_APP
#define DEBUG_SDL_APP_OBJ

#define DEBUG_SDL_IMAGE
#define DEBUG_SDL_IMAGE_OBJ

#endif // NOMLIB_CONFIG_HEADERS defined
