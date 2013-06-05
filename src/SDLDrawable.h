/******************************************************************************
    SDLDrawable.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_SDL_DRAWABLE_HEADERS
#define GAMELIB_SDL_DRAWABLE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "Coords.h"
#include "Color.h"

// Example usage:
// https://gist.github.com/i8degrees/5711070

namespace nom
{
  class SDL_Drawable
  {
    // SDL_Font
    // SDL_Gradient
    public:
      virtual void Draw ( SDL_Surface *video_buffer ) const = 0;
  };

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

  class Rectangle: public SDL_Drawable
  {
    public:
      Rectangle ( void ) : coords ( 0, 0, 0, 0 ), color ( 0, 0, 0, -1 )
      {
        //
      }

      Rectangle ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

      void set ( const Coords& coords, const Color& color )
      {
        this->coords = coords;
        this->color = color;
      }

      void Draw ( SDL_Surface *video_buffer ) const
      {
        SDL_Rect rectangle = this->coords.getSDL_Rect();
        unsigned int rectangle_color = 0;

        rectangle_color = this->color.getColorAsInt ( video_buffer->format );

        if ( SDL_FillRect ( video_buffer, &rectangle, rectangle_color ) != 0 )
        {
          std::cout << "ERR in Gfx::DrawRectangle(): " << SDL_GetError() << std::endl;

          return;
        }

        return;
      }

    private:
      Coords coords;
      Color color;
  };
}

#endif // GAMELIB_SDL_DRAWABLE_HEADERS defined
