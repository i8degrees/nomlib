/******************************************************************************

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Canvas.hpp"

nom::SDL_Canvas::SDL_Canvas ( void )  : canvas_buffer ( nullptr, SDL_FreeSurface ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ), // only the width, height is used in source blitting
                                        colorkey ( 0, 0, 0, -1 )
{
NOMLIB_LOG_INFO;
}

nom::SDL_Canvas::SDL_Canvas ( void* video_buffer )  : canvas_buffer ( static_cast<SDL_Surface*> ( video_buffer ), SDL_FreeSurface )
{
NOMLIB_LOG_INFO;
}

nom::SDL_Canvas::SDL_Canvas ( const SDL_Canvas& other ) : canvas_buffer ( static_cast<SDL_Surface*> ( other.canvas_buffer.get() ), SDL_FreeSurface )
{
NOMLIB_LOG_INFO;
}

nom::SDL_Canvas::SDL_Canvas ( uint32_t flags, int32_t width, int32_t height, int32_t bitsPerPixel, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask )
{
NOMLIB_LOG_INFO;

  this->canvas_buffer.reset(); // nullptr

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurface ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask ), SDL_FreeSurface );
}

nom::SDL_Canvas::SDL_Canvas ( void* pixels, int32_t width, int32_t height, int32_t depth, int32_t pitch, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask )
{
NOMLIB_LOG_INFO;

  this->canvas_buffer.reset(); // nullptr

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask ), SDL_FreeSurface );
}

nom::SDL_Canvas::~SDL_Canvas ( void )
{
NOMLIB_LOG_INFO;

  this->canvas_buffer.reset(); // nullptr
}

bool nom::SDL_Canvas::valid ( void ) const
{
  if ( this->canvas_buffer.get() != nullptr )
    return true;
  else
    return false;
}

void nom::SDL_Canvas::setCanvas ( void* video_buffer )
{
  this->canvas_buffer.reset ( static_cast<SDL_Surface*> ( video_buffer ), SDL_FreeSurface );
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
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->w;
}

const int32_t nom::SDL_Canvas::getCanvasHeight ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->h;
}

uint32_t nom::SDL_Canvas::getCanvasFlags ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->flags;
}

uint16_t nom::SDL_Canvas::getCanvasPitch ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pitch;
}

void* nom::SDL_Canvas::getCanvasPixels ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pixels;
}

void* nom::SDL_Canvas::getCanvasPixelsFormat ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format;
}

const nom::Coords nom::SDL_Canvas::getCanvasBounds ( void ) const
{
  SDL_Rect clip_buffer; // temporary storage struct
  nom::Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) , &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds = nom::Coords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
}

void nom::SDL_Canvas::setCanvasBounds ( const nom::Coords& clip_bounds )
{
  SDL_Rect clip = clip_bounds.getSDL_Rect(); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &clip );
}

bool nom::SDL_Canvas::getCanvasLock ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->locked;
}

bool nom::SDL_Canvas::loadFromImage ( const std::string& filename, const nom::Color& colorkey, uint32_t flags )
{
  nom::SDL_Image image; // holds our image in memory during transfer

  this->canvas_buffer = std::shared_ptr<void> ( image.loadFromFile ( filename ) );

  if ( this->valid() == false )
  {
NOMLIB_LOG_ERR ( "Could not load canvas image file: " + filename );
    return false;
  }

  if ( colorkey.alpha == -1 )
    this->setTransparent ( colorkey, flags );

  if ( colorkey.alpha != -1 )
    this->displayFormatAlpha(); // Optimized video surface with an alpha channel
  else
    this->displayFormat(); // Optimized video surface without an alpha channel

  // Update our canvas clipping bounds with the new source
  this->offsets.setSize ( this->getCanvasWidth(), this->getCanvasHeight() );

  return true;
}

void nom::SDL_Canvas::Draw ( void* video_buffer ) const
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = this->coords.getSDL_Rect();
  SDL_Rect blit_offsets = this->offsets.getSDL_Rect();

  // Perhaps also check to see if video_buffer is nullptr?
  if ( this->valid() )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &blit_offsets, static_cast<SDL_Surface*> ( video_buffer ), &blit_coords ) != 0 )
NOMLIB_LOG_ERR ( SDL_GetError() );
        return;
    }
    else
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), nullptr, (SDL_Surface*) video_buffer, &blit_coords ) != 0 )
NOMLIB_LOG_ERR ( SDL_GetError() );
        return;
    }
  }
}

bool nom::SDL_Canvas::Update ( void* video_buffer )
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }
  return true;
}

bool nom::SDL_Canvas::setAlpha ( uint8_t opacity, uint32_t flags )
{
NOMLIB_ASSERT ( ! ( opacity > SDL_ALPHA_OPAQUE ) || ( opacity < SDL_ALPHA_TRANSPARENT ) );

  if ( SDL_SetAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, static_cast<uint32_t>( opacity ) ) == -1 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool nom::SDL_Canvas::setTransparent  ( const nom::Color& color,
                                        uint32_t flags )
{
  uint32_t transparent_color = 0;

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( this->getCanvasPixelsFormat() );

  if ( SDL_SetColorKey ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, transparent_color ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool nom::SDL_Canvas::displayFormat ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormat ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), SDL_FreeSurface );

NOMLIB_ASSERT ( this->valid() );

  return true;
}

bool nom::SDL_Canvas::displayFormatAlpha ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormatAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), SDL_FreeSurface );

NOMLIB_ASSERT ( this->valid() );

  return true;
}

void nom::SDL_Canvas::clear ( const nom::Color& color ) const
{
  nom::Rectangle rect ( nom::Coords ( 0, 0, this->getCanvasWidth(), this->getCanvasHeight() ), color );

  rect.Draw ( this->canvas_buffer.get() );
}

bool nom::SDL_Canvas::mustLock ( void* video_buffer ) const
{
  if ( SDL_MUSTLOCK ( static_cast<SDL_Surface*> ( video_buffer ) ) )
    return true;
  else
    return false;
}

bool nom::SDL_Canvas::Lock ( void* video_buffer ) const
{
  if ( this->mustLock ( video_buffer ) )
    SDL_LockSurface ( static_cast<SDL_Surface*> ( video_buffer ) );
  else
    return false;
  return true;
}

void nom::SDL_Canvas::Unlock ( void* video_buffer ) const
{
  SDL_UnlockSurface ( static_cast<SDL_Surface*> ( video_buffer ) );
}

int32_t nom::SDL_Canvas::getPixel ( int32_t x, int32_t y )
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );

  //Convert the pixels to 32 bit
  uint32_t *pixels = ( uint32_t * ) buffer->pixels;

  //Get the pixel requested
  return pixels[ ( y * buffer->w ) + x ];
}

nom::SDL_Canvas& nom::SDL_Canvas::operator = ( const SDL_Canvas& other )
{
  this->canvas_buffer = other.canvas_buffer;

  return *this;
}
