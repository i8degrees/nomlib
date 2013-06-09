/******************************************************************************
    SDL_Primitive.hpp

    SDL Primitives implementation

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_RECTANGLE_HEADERS
#define NOMLIB_SDL_RECTANGLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"
#include "SDL_Drawable.hpp"
#include "gamelib.h"

// Example usage:
// https://gist.github.com/i8degrees/5711070

namespace nom
{
  // This is an inheritance-only class
  class Primitive: public nom::SDL_Drawable
  {
    public:
      Primitive ( void )  : coords ( 0, 0, 0, 0 ), color ( 0, 0, 0, -1 )
      {
        // ...
      }

      virtual ~Primitive ( void )
      {
        // ...
      }

    protected:
      Coords coords;
      Color color;
  };

  class Rectangle: public nom::Primitive
  {
    public:
      Rectangle ( void )  /*: coords ( 0, 0, 0, 0 ), color ( 0, 0, 0, -1)*/
      {
        //
      }

      Rectangle ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

      ~Rectangle ( void )
      {
      }

      void setPosition ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

      void Draw ( void* video_buffer )
      {
        SDL_Rect rectangle = this->coords.getSDL_Rect();
        unsigned int rectangle_color = 0;
        SDL_Surface *buffer = (SDL_Surface*) video_buffer;

        rectangle_color = this->color.getColorAsInt ( buffer->format );

        if ( SDL_FillRect ( buffer, &rectangle, rectangle_color ) != 0 )
        {
          std::cout << "ERR in Gfx::DrawRectangle(): " << SDL_GetError() << std::endl;

          return;
        }

        return;
      }

    private:
      //Coords coords;
      //Color color;
  };
}

#endif // NOMLIB_SDL_RECTANGLE_HEADERS defined
