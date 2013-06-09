/******************************************************************************
    SDL_Surface.cpp

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Canvas.hpp"

nom::SDL_Canvas::SDL_Canvas ( void )  : canvas_buffer ( nullptr ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ), // only the width, height is used in source blitting
                                        colorkey ( -1, -1, -1, -1 )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::SDL_Canvas(): Hello, world!" << "\n" << std::endl;
  #endif
}

nom::SDL_Canvas::SDL_Canvas ( int32_t width, int32_t height, const Color& colors, uint32_t flags )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::SDL_Canvas(): Hello, world!" << "\n" << std::endl;
  #endif

  // ...Create a new surface here
}

nom::SDL_Canvas::SDL_Canvas ( void* video_buffer )
{
  this->canvas_buffer = (SDL_Surface*) video_buffer;
}

nom::SDL_Canvas::~SDL_Canvas ( void )
{
  #ifdef DEBUG_SDL_CANVAS_OBJ
    std::cout << "nom::SDL_Canvas::~SDL_Canvas(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->freeCanvas();
}

void nom::SDL_Canvas::freeCanvas ( void )
{
  if ( this->canvas_buffer != nullptr )
    SDL_FreeSurface ( canvas_buffer );

  this->canvas_buffer = nullptr;
}

void* nom::SDL_Canvas::get ( void ) const
{
  return this->canvas_buffer;
}

void nom::SDL_Canvas::setCanvas ( SDL_Surface *video_buffer )
{
  this->canvas_buffer = video_buffer;
}

void nom::SDL_Canvas::setCanvas ( void* video_buffer )
{
  this->canvas_buffer = (SDL_Surface*) video_buffer;
}

void nom::SDL_Canvas::setPosition ( const Coords& coords_ )
{
  this->coords = coords_;
}

void nom::SDL_Canvas::setOffsets ( const Coords& offsets_ )
{
  this->offsets = offsets_;
}

const int32_t nom::SDL_Canvas::getCanvasWidth ( void ) const
{
  return this->canvas_buffer->w;
}

const int32_t nom::SDL_Canvas::getCanvasHeight ( void ) const
{
  return this->canvas_buffer->h;
}

void* nom::SDL_Canvas::getCanvasPixelsFormat ( void ) const
{
  return this->canvas_buffer->format;
}

bool nom::SDL_Canvas::loadImageFromFile ( const std::string& filename, const nom::Color& colorkey, uint32_t flags )
{
  nom::SDL_Image image;
  this->canvas_buffer = (SDL_Surface*) image.loadImageFromFile ( filename, colorkey, flags );

  this->offsets.setWidth ( this->canvas_buffer->w );
  this->offsets.setHeight ( this->canvas_buffer->h );
  return true;
}

void nom::SDL_Canvas::Draw ( void* video_buffer )
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = this->coords.getSDL_Rect();
  SDL_Rect blit_offsets = this->offsets.getSDL_Rect();

  if ( (SDL_Surface*) canvas_buffer != nullptr )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( canvas_buffer, &blit_offsets, (SDL_Surface*) video_buffer, &blit_coords ) != 0 )
      {
        #ifdef DEBUG_SDL_CANVAS
          std::cout << "ERR in SDL_Canvas::Draw() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
        #endif
      }
    }
    else
    {
      SDL_BlitSurface ( canvas_buffer, nullptr, (SDL_Surface*) video_buffer, &blit_coords );
    }
  }
}

bool nom::SDL_Canvas::Update ( void* video_buffer ) const
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::Update(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool nom::SDL_Canvas::setAlpha ( void* video_buffer, uint8_t opacity, uint32_t flags )
{
  #ifdef DEBUG_SDL_CANVAS
    if ( opacity > SDL_ALPHA_OPAQUE || opacity < SDL_ALPHA_TRANSPARENT )
      std::cout << "ERR in nom::SDL_Canvas::setAlpha(): " << "opacity value is set out of bounds." << std::endl << std::endl;
  #endif

  if ( SDL_SetAlpha ( (SDL_Surface*) video_buffer, flags, static_cast<uint32_t>( opacity ) ) == -1 )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::setAlpha(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

bool nom::SDL_Canvas::setTransparent  ( void* video_buffer,
                                        const nom::Color& color,
                                        unsigned int flags
                                      )
{
  SDL_Surface *buffer = static_cast<SDL_Surface*> ( video_buffer ); // FIXME
  uint32_t transparent_color = 0;

  if ( buffer == nullptr )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::setTransparent(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( buffer->format );

  if ( SDL_SetColorKey ( buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::setTransparent(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

