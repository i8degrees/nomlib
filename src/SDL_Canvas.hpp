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
#include "Color.hpp"
#include "Coords.hpp"
#include "SDL_Rectangle.hpp"
#include "SDL_Image.hpp"
#include "gamelib.h"

namespace nom
{
  class SDL_Canvas
  {
    public:
      SDL_Canvas ( void );
      SDL_Canvas ( void* video_buffer );
      SDL_Canvas ( uint32_t flags, int32_t width, int32_t height, int32_t bitsPerPixel, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
      SDL_Canvas ( void* pixels, int32_t width, int32_t height, int32_t depth, int32_t pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
      ~SDL_Canvas ( void );
      void destroy ( void );

      /// Obtains raw pointer to the object's video surface buffer
      ///
      /// Returns ( SDL_Surface* )
      ///
      void* get ( void ) const;

      /// Is this SDL_Surface* initialized ( != nullptr )?
      bool valid ( void ) const;

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
      bool getCanvasLock ( void ) const;

      // TODO: SDL_CreateRGBSurface
      // TODO: SDL_CreateRGBSurfaceFrom

      bool loadFromImage  ( const std::string& filename, const nom::Color&
                            colorkey = nom::Color ( 0, 0, 0, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

      bool Update ( void* video_buffer );
      void Draw ( void* video_buffer ) const;

      bool setAlpha ( uint8_t opacity, uint32_t flags = SDL_SRCALPHA );

      bool setTransparent ( const nom::Color& color = nom::Color ( 0, 0, 0, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                          );

      bool displayFormat ( void );
      bool displayFormatAlpha ( void );

      void clear ( const nom::Color& color = nom::Color::Blue ) const;


      bool Lock ( void* video_buffer ) const;
      void Unlock ( void* video_buffer ) const;

      int32_t getPixel ( int32_t x, int32_t y );

    private:
      bool mustLock ( void* video_buffer ) const;
      void* canvas_buffer; // SDL_Surface*
      nom::Coords coords;
      nom::Coords offsets;
      nom::Color colorkey;
    };
}

#endif // NOMLIB_SDL_SURFACE_HEADERS defined
