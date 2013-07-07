/******************************************************************************

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_SURFACE_HEADERS
#define NOMLIB_SDL_SURFACE_HEADERS

#include <iostream>
#include <string>

#include <SDL/SDL.h>

//#include "gfx/ICanvas.hpp"
#include "math/Color.hpp"
#include "math/Coords.hpp"
#include "gfx/SDL_Image.hpp"
#include "gfx/SDL_Rectangle.hpp"
#include "nomlib_config.hpp"

namespace nom
{
  class SDL_Canvas
  {
    public:
      /// Default constructor; initializes object to its respective defaults
      SDL_Canvas ( void );
      /// Constructor variant for setting the canvas with existing data
      SDL_Canvas ( void* video_buffer );
      /// Copy constructor
      SDL_Canvas ( const nom::SDL_Canvas& other );
      /// Constructor variant for setting the canvas with an empty surface
      ///
      /// As per libSDL docs, this must be called only after video initialization;
      /// (SDL_SetVideoMode)
      ///
      /// http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurface
      SDL_Canvas ( uint32_t flags, int32_t width, int32_t height, int32_t bitsPerPixel, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
      /// Constructor variant for setting the canvas with existing pixel data
      ///
      /// http://sdl.beuc.net/sdl.wiki/SDL_CreateRGBSurfaceFrom
      ///
      /// Note that we are responsible for freeing our own pixels data
      SDL_Canvas ( void* pixels, int32_t width, int32_t height, int32_t depth, int32_t pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask );
      ~SDL_Canvas ( void );

      /// Is this object initialized -- not nullptr?
      bool valid ( void ) const;

      /// Set the canvas with existing surface data
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
      /// I think that we are accessing the value of an
      /// (internal?) property of the SDL_Surface structure that is described as being
      /// "private" as per the docs.
      ///
      /// Return value of this internal property is presumed to be boolean -- no
      /// verification has been made of this. Testing of this method *appears*
      /// to be in working order.
      bool getCanvasLock ( void ) const;

      /// Sets transparency only if colorkey nom::Color alpha value is -1, also
      /// important to note is that we only have an alpha channel surface set
      /// if nom::Color value is not -1 (the default)
      ///
      /// \internal
      /// Clean up this documentation note and also verify that it is
      /// sane to assume that you would not use transparency when you have alpha
      /// surface enabled
      /// \endinternal
      bool loadFromImage  ( const std::string& filename, const nom::Color&
                            colorkey = nom::Color ( -1, -1, -1, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

      bool Update ( void* video_buffer );
      void Draw ( void* video_buffer ) const;

      bool setAlpha ( uint8_t opacity, uint32_t flags = SDL_SRCALPHA );

      bool setTransparent ( const nom::Color& color = nom::Color ( 0, 0, 0, -1 ),
                            uint32_t flags = SDL_RLEACCEL | SDL_SRCCOLORKEY
                          );

      /// As per libSDL docs, we must first initialize the video subsystem before using
      /// this method call, otherwise an access violation fault is sure to occur.
      bool displayFormat ( void );
      /// As per libSDL docs, we must first initialize the video subsystem before using
      /// this method call, otherwise an access violation fault is sure to occur.
      bool displayFormatAlpha ( void );

      /// \internal
      /// Note: this method is not meant to be called inside a loop; memory usage may
      /// run a mock (seems to be fixed by Rectangle::~Rectangle() inside the
      /// SDL_Rectangle class, although it eludes me as to why precisely
      /// \endinternal
      void clear ( const nom::Color& color = nom::Color::Blue ) const;

      bool Lock ( void* video_buffer ) const;
      void Unlock ( void* video_buffer ) const;

      int32_t getPixel ( int32_t x, int32_t y );

      /// Copy assignment constructor
      SDL_Canvas& operator = ( const SDL_Canvas& other );

    private:
      bool mustLock ( void* video_buffer ) const;
      std::shared_ptr<void> canvas_buffer; // SDL_Surface*
      nom::Coords coords;
      nom::Coords offsets;
      nom::Color colorkey;
    };
}

#endif // NOMLIB_SDL_SURFACE_HEADERS defined
