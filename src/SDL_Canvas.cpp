/******************************************************************************
    SDL_Canvas.cpp

    Abstraction of SDL Surfaces

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Canvas.hpp"

nom::SDL_Canvas::SDL_Canvas ( void )  : canvas_buffer ( nullptr ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ), // only the width, height is used in source blitting
                                        colorkey ( 0, 0, 0, -1 )
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

  // ...Create a new surface here?
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

// FIXME: methinks ought to be return (SDL_Surface*) this->canvas_buffer or so?
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

uint32_t nom::SDL_Canvas::getCanvasFlags ( void ) const
{
  return this->canvas_buffer->flags;
}

uint16_t nom::SDL_Canvas::getCanvasPitch ( void ) const
{
  return this->canvas_buffer->pitch;
}

void* nom::SDL_Canvas::getCanvasPixels ( void ) const
{
  return this->canvas_buffer->pixels;
}

void* nom::SDL_Canvas::getCanvasPixelsFormat ( void ) const
{
  return this->canvas_buffer->format;
}

const nom::Coords nom::SDL_Canvas::getCanvasBounds ( void ) const
{
  SDL_Rect clip_buffer; // temporary storage struct
  nom::Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( this->canvas_buffer, &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds.setCoords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
}

void nom::SDL_Canvas::setCanvasBounds ( const nom::Coords& clip_bounds )
{
  SDL_Rect clip = clip_bounds.getSDL_Rect(); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( this->canvas_buffer, &clip );
}

bool nom::SDL_Canvas::loadFromImage ( const std::string& filename, const nom::Color& colorkey, uint32_t flags )
{
  nom::SDL_Image image; // holds our image in memory during transfer

  if ( image.loadFromFile ( filename ) == false )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::loadFromImage(): " << std::endl << std::endl;
    #endif
    return false;
  }

  // Sets our canvas with our acquired image surface
  setCanvas ( image.get() );

  if ( colorkey.getAlpha() == -1 ) // FIXME
    setTransparent ( colorkey, flags );

  if ( colorkey.getAlpha() != -1 )
    displayFormatAlpha(); // Optimized video surface with an alpha channel
  else
    displayFormat(); // Optimized video surface without an alpha channel

  // Update our canvas clipping bounds with the new source
  this->offsets.setWidth ( this->getCanvasWidth() );
  this->offsets.setHeight ( this->getCanvasHeight() );

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

bool nom::SDL_Canvas::Update ( void* video_buffer )
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

bool nom::SDL_Canvas::setTransparent  ( const nom::Color& color,
                                        uint32_t flags )
{
  uint32_t transparent_color = 0;

  if ( this->get() == nullptr )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::setTransparent(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( this->getCanvasPixelsFormat() );

  if ( SDL_SetColorKey ( static_cast<SDL_Surface*> ( this->get() ), flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_SDL_CANVAS
      std::cout << "ERR in nom::SDL_Canvas::setTransparent(): " << SDL_GetError() << std::endl << std::endl;
    #endif
    return false;
  }

  return true;
}

// As per libSDL docs, we must first initialize the video subsystem before using
// this method call, otherwise an access violation fault is sure to occur.
bool nom::SDL_Canvas::displayFormat ( void )
{
  void* converted_canvas = nullptr; // Better safe than sorry!

  converted_canvas = SDL_DisplayFormat ( static_cast<SDL_Surface*> ( this->get() ) );

  if ( converted_canvas == nullptr )
    return false;

  freeCanvas(); // Clean up our existing surface first to be safe

  // TODO: compare canvas_buffer == converted_buffer first?

  // Set the newly converted surface as our own
  this->setCanvas ( converted_canvas );

  return true;
}

// As per libSDL docs, we must first initialize the video subsystem before using
// this method call, otherwise an access violation fault is sure to occur.
bool nom::SDL_Canvas::displayFormatAlpha ( void )
{
  void* converted_canvas = nullptr; // Better safe than sorry!

  converted_canvas = SDL_DisplayFormatAlpha ( static_cast<SDL_Surface*> ( this->get() ) );

  if ( converted_canvas == nullptr )
    return false;

  freeCanvas(); // Clean up our existing surface first to be safe

  // TODO: compare canvas_buffer == converted_buffer first?

  // Set the newly converted surface as our own
  this->setCanvas ( converted_canvas );

  return true;
}

void nom::SDL_Canvas::clear ( const nom::Color& color )
{
  nom::Rectangle rect ( nom::Coords ( 0, 0, this->getCanvasWidth(), this->getCanvasHeight() ), color );

  rect.Draw ( this->canvas_buffer );

  // ...
}

bool nom::SDL_Canvas::mustLock ( void )
{
  if ( SDL_MUSTLOCK ( this->canvas_buffer ) )
    return true;
  else
    return false;
}

bool nom::SDL_Canvas::lockCanvas ( void )
{
  if ( this->mustLock() )
    SDL_LockSurface ( this->canvas_buffer );
  else
    return false;
  return true;
}

bool nom::SDL_Canvas::unlockCanvas ( void )
{
  SDL_UnlockSurface ( this->canvas_buffer );

  return true;
}
