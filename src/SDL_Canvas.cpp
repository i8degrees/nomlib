/******************************************************************************
    SDL_Surface.cpp

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Canvas.hpp"

using namespace nom;

SDL_Canvas::SDL_Canvas ( void ) : canvas_buffer ( NULL )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::SDL_Canvas(): Hello, world!" << "\n" << std::endl;
  #endif
}

SDL_Canvas::SDL_Canvas ( int32_t width, int32_t height, const Color& colors, uint32_t flags )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::SDL_Canvas(): Hello, world!" << "\n" << std::endl;
  #endif

  // ...Create a new surface here
}

SDL_Canvas::~SDL_Canvas ( void )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::~SDL_Canvas(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  //if ( this->canvas_buffer != NULL )
    //SDL_FreeSurface ( canvas_buffer );

  //this->canvas_buffer = NULL;
}

void SDL_Canvas::setCanvas ( const Surface<SDL_Surface>& video_buffer )
{
  canvas_buffer = video_buffer;
}

bool SDL_Canvas::loadImage ( std::string filename, const Color& colorkey, unsigned int flags )
{
  canvas_buffer = (SDL_Surface*) Gfx::LoadImage ( filename, colorkey, flags );
  return true;
}

Surface<SDL_Surface> SDL_Canvas::get ( void )
{
  return canvas_buffer;
}

void SDL_Canvas::Draw ( void* video_buffer ) const
{
  // temporary vars to store our wrapped GCoords
    //SDL_Rect blit_coords = coords.getSDL_Rect();
    //SDL_Rect blit_offsets = offsets.getSDL_Rect();

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): NULL surface" << std::endl;
    #endif
    return;
    //return false;
  }

  // Coords ( -1, -1, -1, -1 ) -- our default, is equivalent to NULL for SDL_BlitSurface it seems
  if ( SDL_BlitSurface ( canvas_buffer.raw_ptr(), NULL, (SDL_Surface*) video_buffer, NULL ) != 0 )
    //if ( SDL_BlitSurface ( canvas_buffer.raw_ptr(), &blit_offsets, video_buffer.raw_ptr(), &blit_coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
    #endif

    //return false;
  }

  //return true;
}
