/******************************************************************************
    SDL_Canvas.hpp

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_SURFACE_HEADERS
#define NOMLIB_SDL_SURFACE_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "SDL.h"

//#include "ICanvas.hpp"
#include "Color.h"
#include "Coords.h"

#include "SDL_Primitive.hpp"
#include "SDL_Rectangle.hpp"
#include "SDL_Image.hpp"
#include "gamelib.h"

namespace nom
{
  class SDL_Canvas
  {
    public:
      SDL_Canvas ( void );
      SDL_Canvas ( int32_t width, int32_t height, const Color& colorbit, uint32_t flags = 0 );
      SDL_Canvas ( void* video_buffer );
      ~SDL_Canvas ( void );
      void freeCanvas ( void );

      /// Obtains raw pointer to the object's video surface buffer
      ///
      /// ( SDL_Surface* )
      void* get ( void ) const;

      /// SDL compatibility wrapper
      void setCanvas ( SDL_Surface *video_buffer );

      void setCanvas ( void* video_buffer );

      void setPosition ( const Coords& coords_ );
      void setOffsets ( const Coords& offsets_ );

      const int32_t getCanvasWidth ( void ) const;
      const int32_t getCanvasHeight ( void ) const;
      uint32_t getCanvasFlags ( void ) const;
      u_short getCanvasPitch ( void ) const;
      void* getCanvasPixels ( void ) const;
      void* getCanvasPixelsFormat ( void ) const;
      const nom::Coords getCanvasBounds ( void ) const;
      void setCanvasBounds ( const nom::Coords& clip_bounds );

      // ( bool ) mustLock
      // lockCanvas
      // unlockCanvas
      // SDL_CreateRGBSurface
      // SDL_CreateRGBSurfaceFrom

      bool loadFromImage  ( const std::string& filename, const nom::Color&
                            colorkey = nom::Color ( 0, 0, 0, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

      void Draw ( void* video_buffer );
      bool Update ( void* video_buffer ) const;

      bool setAlpha ( void* video_buffer, uint8_t opacity,
                      uint32_t flags = SDL_SRCALPHA );

      bool setTransparent ( const nom::Color& color = nom::Color ( 0, 0, 0, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                          );

      bool displayFormat ( void );
      bool displayFormatAlpha ( void );

      void clear ( const nom::Color& color = nom::Blue );

      bool mustLock ( void );
      bool lockCanvas ( void );
      bool unlockCanvas ( void );

    private:
      SDL_Surface *canvas_buffer;
      nom::Coords coords;
      nom::Coords offsets;
      nom::Color colorkey;
    };
}

#endif // NOMLIB_SDL_SURFACE_HEADERS defined
