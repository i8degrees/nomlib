/******************************************************************************
    SDL_Surface.h

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_SDL_SURFACE_HEADERS
#define NOMLIB_SDL_SURFACE_HEADERS

#include <iostream>
#include <string>
#include <cassert>

#include "ICanvas.hpp"

#include "SDL.h"

#include "gamelib.h"

//#include "Color.h"
//#include "Coords.h"
#include "gfx.h"
#include "SDL_Display.h"

#include <memory> // for shared_ptr<T>

namespace nom
{
  template <typename T>
  class Surface
  {
  public:
    template <void (D)(T*)> Surface(T* ptr) : p(ptr, D) { }
    template <typename D> Surface(T* ptr, D d) : p(ptr, d) { }

    Surface(T* ptr) : p(ptr) { }
    Surface() : p() { }
    Surface(const Surface<T>& that) : p(that.p) { }

    T* raw_ptr() const { return p.get(); }

    virtual ~Surface() { }

  //protected:
    std::shared_ptr<T> p;
  };
}

namespace nom
{
  class SDL_Canvas: public Surface<SDL_Surface>
  {
    public:
      SDL_Canvas ( void );
      SDL_Canvas ( int32_t width, int32_t height, const Color& colorbit, uint32_t flags = 0 );
      ~SDL_Canvas ( void );
      SDL_Canvas ( SDL_Surface *surface )
      : Surface<SDL_Surface>(surface, SDL_FreeSurface)
      {}
      SDL_Canvas ( const Surface& video_buffer )
      : Surface<SDL_Surface>( video_buffer )

      {
        //canvas_buffer = video_buffer;
      }

      //void setPosition ( const Coords& coords );
      //void setOffsets ( const Coords& offsets );
      void setCanvas ( const Surface<SDL_Surface>& video_buffer );

      bool loadImage ( std::string filename, const Color& colorkey = nom::Color ( -1, -1, -1, -1 ), unsigned int flags = 0 );

      void Draw ( void* video_buffer ) const;
                  //const nom::Coords &coords, const nom::Coords &offsets

      Surface<SDL_Surface> get ( void );

    private:
      Surface<SDL_Surface> canvas_buffer;
      //Coords coords;
      //Coords offsets;
      //Color color;
    };
  class SDL_VideoSurface: public SDL_Canvas
  {
    public:
      SDL_VideoSurface ( SDL_Surface *screen )
      : SDL_Canvas ( screen )
      {}
      SDL_VideoSurface ( int width, int height, int bpp, unsigned int flags = 0 )
      : SDL_Canvas ( SDL_SetVideoMode ( width, height, bpp, flags ) )
      {
      }

      SDL_VideoSurface ( const SDL_VideoSurface& video_buffer )
      : SDL_Canvas ( video_buffer )
      {
      }
      virtual ~SDL_VideoSurface () {}
      //static Surface<SDL_Surface> get()
      //{
        //return SDL_GetVideoSurface();
      //}
    private:
      //
  };
}

#endif // NOMLIB_SDL_SURFACE_HEADERS defined
