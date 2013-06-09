/******************************************************************************
    SDL_Drawable.hpp

    Drawables interface implementation

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_DRAWABLE_HEADERS
#define NOMLIB_SDL_DRAWABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"

#include "Drawable.hpp"

// Example usage:
// https://gist.github.com/i8degrees/5711070

namespace nom
{
  // This is an inheritance-only class
  class SDL_Drawable: public nom::Drawable
  {
    // SDL_Gradient
    protected:
      virtual void Draw ( void* video_buffer ) = 0;

      SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "SDL_Drawable::SDL_Drawable(): Hello, world!" << std::endl << std::endl;
        #endif
      }
      virtual ~SDL_Drawable ( void )
      {
        #ifdef DEBUG_SDL_DRAWABLE_OBJ
          std::cout << "SDL_Drawable::~SDL_Drawable(): Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }
  };

/*

  class Pixel: public Primitive
  {
    public:
      // ...
    private:
      // ...
  };

  class Line: public Primitive
  {
    public:
      // ...
    private:
      // ...
  };
*/
/*
  class Image: public SDL_Drawable
  {
    public:
      Image ( void )  : image_buffer ( NULL ), coords ( 0, 0, 0, 0 ),
                        colorkey ( 0, 0, 0, -1 )
      {}

      ~Image ( void )
      {
        SDL_FreeSurface ( image_buffer );
        image_buffer = NULL;
      }

      bool Load ( const std::string filename, const Color& colorkey, unsigned int flags )
      {
        return false;
      }

      void Draw ( SDL_Surface *video_buffer ) const = 0;

    private:
      SDL_Surface *image_buffer;
      Coords coords;
      Color colorkey;
  };

  class Canvas: public SDL_Drawable
  {
    public:
      Canvas ( void ) : coords ( 0, 0, 0, 0 ), color ( 0, 0, 0, -1 )
      {}

      Canvas ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

      ~Canvas ( void );

      void Draw ( SDL_Surface *video_buffer ) const
      {
        // ...
      }

    private:
      Coords coords;
      Color color;
  };
*/
}

#endif // NOMLIB_SDL_DRAWABLE_HEADERS defined
