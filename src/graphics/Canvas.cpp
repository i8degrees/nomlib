/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    scale2x Algorithm
1. Copyright (c) Andrea Mazzoleni

******************************************************************************/
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/graphics/scale2x/scale2x.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {

Canvas::Canvas ( void )  : canvas_buffer ( nullptr, priv::FreeTexture ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ) // only the width, height is used in source blitting
{
NOM_LOG_TRACE ( NOM );
}
/*
Canvas::Canvas ( SDL_Surface* video_buffer )  : canvas_buffer { video_buffer, nom::priv::FreeSurface }
{
NOM_LOG_TRACE ( NOM );

  this->offsets.setSize ( video_buffer->w, video_buffer->h );
}
*/

Canvas::Canvas ( SDL_Texture* video_buffer )  : canvas_buffer { video_buffer, priv::FreeTexture }
{
NOM_LOG_TRACE ( NOM );

  Coords size;

  SDL_QueryTexture ( this->canvas_buffer.get(), nullptr, nullptr, &size.width, &size.height );

  this->offsets.setSize ( size.width, size.height );
}

Canvas::Canvas ( const Canvas& other ) : canvas_buffer { other.canvas_buffer.get(), nom::priv::FreeTexture },
                                                          coords ( other.coords.x, other.coords.y ), offsets ( other.offsets.width, other.offsets.height )
{
NOM_LOG_TRACE ( NOM );
}

Canvas::Canvas ( int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, uint32 flags )
{
NOM_LOG_TRACE ( NOM );

  this->initialize ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask );
}

Canvas::Canvas ( void* pixels, int32 width, int32 height, int32 depth, uint16 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask )
{
NOM_LOG_TRACE ( NOM );

  //this->canvas_buffer.reset ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask ), nom::priv::FreeSurface );

  this->offsets.setSize ( width, height );
}

Canvas::~Canvas ( void )
{
NOM_LOG_TRACE ( NOM );
}

void Canvas::initialize ( uint32 flags, int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask )
{
NOM_LOG_TRACE ( NOM );

  //this->canvas_buffer.reset ( SDL_CreateRGBSurface ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask ), nom::priv::FreeSurface );
  this->offsets.setSize ( width, height );

  // If the video surface is marked for color keying transparency, we must do
  // so here.
  if ( flags & SDL_TRUE )//SDL_SRCCOLORKEY )
  {
    if ( this->setTransparent ( this->getCanvasColorKey(), SDL_RLEACCEL | SDL_TRUE ) == false )//SDL_SRCCOLORKEY ) == false )
    {
NOM_LOG_ERR ( NOM, "Could not create the video surface with color key transparency." );
    }
  }
}

Canvas::SharedPtr Canvas::clone ( void ) const
{
  return Canvas::SharedPtr ( new Canvas ( *this ) );
}

Canvas& Canvas::operator = ( const Canvas& other )
{
  this->canvas_buffer = other.canvas_buffer;
  this->coords = other.coords;
  this->offsets = other.offsets;

  return *this;
}

Canvas::RawPtr Canvas::get ( void ) const
{
  return this->canvas_buffer.get();
}

bool Canvas::valid ( void ) const
{
  if ( this->canvas_buffer.get() != nullptr )
    return true;
  else
    return false;
}

void Canvas::setCanvas ( const Canvas& surface )
{
  //this->canvas_buffer.reset ( SDL_ConvertSurface ( surface.canvas_buffer.get(), surface.getCanvasPixelsFormat(), surface.getCanvasFlags() ), nom::priv::FreeSurface );

  this->offsets.setSize ( surface.getCanvasWidth(), surface.getCanvasHeight() );
}

const Coords& Canvas::getPosition ( void ) const
{
  return this->coords;
}

void Canvas::setPosition ( const Coords& pos )
{
  this->coords.x = pos.x;
  this->coords.y = pos.y;
}

void Canvas::setOffsets ( const Coords& clip )
{
  this->offsets = clip;
}

const int32 Canvas::getCanvasWidth ( void ) const
{
  Coords size;

  SDL_QueryTexture ( this->canvas_buffer.get(), nullptr, nullptr, &size.width, &size.height );

  return size.width;
}

const int32 Canvas::getCanvasHeight ( void ) const
{
  Coords size;

  SDL_QueryTexture ( this->canvas_buffer.get(), nullptr, nullptr, &size.width, &size.height );

  return size.height;
}

uint32 Canvas::getCanvasFlags ( void ) const
{
  return 0;
  //return this->get()->flags;;
}

uint16 Canvas::getCanvasPitch ( void ) const
{
  return 0;
  //return this->get()->pitch;
}

void* Canvas::getCanvasPixels ( void ) const
{
  return nullptr;
  //return this->get()->pixels;
}

const uint8 Canvas::getCanvasBitsPerPixel ( void ) const
{
  return 0;
  //return this->canvas_buffer.get()->format->BitsPerPixel;
}

SDL_PixelFormat* Canvas::getCanvasPixelsFormat ( void ) const
{
  return nullptr;  
  //return this->canvas_buffer.get()->format;
}

const Color Canvas::getCanvasColorKey ( void ) const
{
  return Color::null;
/*
  uint32 transparent_color = 0; // holds me color for conversion
  Color colorkey; // native container

  transparent_color = RGBA::asInt32 ( this->getCanvasPixelsFormat(), colorkey );

  return colorkey;
*/
}

const uint8 Canvas::getCanvasAlphaValue ( void ) const
{
  return 0;
  //return this->getCanvasPixelsFormat()->alpha;
}

const uint32 Canvas::getCanvasRedMask ( void ) const
{
  return 0;
  //return this->getCanvasPixelsFormat()->Rmask;
}

const uint32 Canvas::getCanvasGreenMask ( void ) const
{
  return 0;
  //return this->getCanvasPixelsFormat()->Gmask;
}

const uint32 Canvas::getCanvasBlueMask ( void ) const
{
  return 0;
  //return this->getCanvasPixelsFormat()->Bmask;
}

const uint32 Canvas::getCanvasAlphaMask ( void ) const
{
  return 0;
  //return this->getCanvasPixelsFormat()->Amask;
}

const Coords Canvas::getCanvasBounds ( void ) const
{
  return Coords::null;
/*
  SDL_Rect clip_buffer; // temporary storage struct
  Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( this->canvas_buffer.get(), &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds = Coords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
*/
}

void Canvas::setCanvasBounds ( const Coords& clip_bounds )
{
/*
  SDL_Rect clip = IntRect::asSDLRect ( clip_bounds ); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( this->canvas_buffer.get(), &clip );
*/
}

int32 Canvas::getCanvasColorDepth ( void ) const
{
  return 0;
/*
  switch ( this->getCanvasPixelsFormat()->BytesPerPixel )
  {
    default: return -1; break; // Unsupported color depth

    case 1: return 8; break; // 8-bit
    case 2: return 16; break; // 16-bit
    case 3: return 24; break; // 24-bit
    case 4: return 32; break; // 32-bit
  }
*/
}

bool Canvas::getCanvasLock ( void ) const
{
  return false;
  //return this->canvas_buffer.get()->locked;
}

bool Canvas::mustLock ( void ) const
{
  return false;
/*
  if ( SDL_MUSTLOCK ( this->canvas_buffer.get() ) )
  {
  return true;
  }
  else
  {
    return false;
  }
*/
}

bool Canvas::lock ( void ) const
{
  return false;
/*
  if ( this->mustLock() == true )
  {
    if ( SDL_LockSurface ( this->canvas_buffer.get() ) == -1 )
    {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
      return false;
    }
  }
*/
  return true;
}

void Canvas::unlock ( void ) const
{
  //SDL_UnlockSurface ( this->canvas_buffer.get() );
}

bool Canvas::load ( const std::string& filename, const Color& colorkey,
                    bool use_cache, uint32 flags
                  )
{
  Image image;
/*
  // By default -- for peace of mind above all else -- we have caching turned
  // off
  if ( use_cache )
  {
    priv::ObjectCache cache;

    this->canvas_buffer = cache.getObject ( filename );

    if ( this->canvas_buffer == nullptr )
    {
      this->canvas_buffer = cache.addObject ( filename, image.load ( filename ) );
    }
  }
  else // Do not use the object cache
  {
*/
    this->canvas_buffer = image.load ( filename );
  //}

  // Validate our obtained data is good before further processing
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not load canvas image file: " + filename );
    return false;
  }

  // We produce a segmentation fault here if we do not have SDL's video
  // subsystem initialized before making the following calls -- transparency
  // and display format conversion.
NOM_ASSERT ( SDL_WasInit ( SDL_INIT_VIDEO) );

  if ( flags & SDL_TRUE )//SDL_SRCCOLORKEY )
    this->setTransparent ( colorkey, flags );

  /*if ( flags & SDL_SRCALPHA )
  {
    this->displayFormatAlpha(); // Optimized video surface with an alpha channel
  }
  else
  {*/
    this->displayFormat(); // Optimized video surface without an alpha channel
  //}

  // Update our canvas clipping bounds with the new source; not sure if we still
  // need to be doing this.
  this->offsets.setSize ( this->getCanvasWidth(), this->getCanvasHeight() );

  return true;
}

void Canvas::Draw ( SDL_Surface* video_buffer ) const
{
/*
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = IntRect::asSDLRect ( this->coords );
  SDL_Rect blit_offsets = IntRect::asSDLRect ( this->offsets );

  // Perhaps also check to see if video_buffer is nullptr?
  if ( this->valid() )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( this->canvas_buffer.get(), &blit_offsets, video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( NOM, SDL_GetError() );
        return;
    }
    else
    {
      if ( SDL_BlitSurface ( this->canvas_buffer.get(), nullptr, (SDL_SDL_Surface*) video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( NOM, SDL_GetError() );
        return;
    }
  }
*/
}

void Canvas::Update ( SDL_Renderer* video_target )
{
  SDL_RenderPresent ( video_target );
}

bool Canvas::setAlpha ( uint8_t opacity )
{
NOM_ASSERT ( ! ( opacity > SDL_ALPHA_OPAQUE ) || ( opacity < SDL_ALPHA_TRANSPARENT ) );
/*
  if ( SDL_SetAlpha ( this->canvas_buffer.get(), flags, static_cast<uint32>( opacity ) ) == -1 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
*/
/*
  if ( SDL_SetTextureAlphaMod ( this->canvas_buffer.get(), opacity ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
*/
  return true;
}

bool Canvas::setTransparent ( const Color& color, uint32_t flags )
{
/*
  uint32_t transparent_color = 0;

  // TODO: Alpha value needs testing
  transparent_color = RGBA::asInt32 ( this->getCanvasPixelsFormat(), color );

  if ( SDL_SetColorKey ( this->canvas_buffer.get(), flags, transparent_color ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
*/
  return true;
}

bool Canvas::displayFormat ( void )
{
  //this->canvas_buffer.reset ( SDL_DisplayFormat ( this->canvas_buffer.get() ), nom::priv::FreeSurface );

//NOM_ASSERT ( this->valid() );

  return true;
}

bool Canvas::displayFormatAlpha ( void )
{
  //this->canvas_buffer.reset ( SDL_DisplayFormatAlpha ( this->canvas_buffer.get()), nom::priv::FreeSurface );

//NOM_ASSERT ( this->valid() );

  return true;
}

uint32 Canvas::getPixel ( int32 x, int32 y )
{
  switch ( this->getCanvasColorDepth() )
  {
    default: return -1; break; // Unsupported

    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( this->getCanvasPixels() );

      return pixels[ ( y * this->getCanvasPitch() ) + x ];
    }
    break;

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( this->getCanvasPixels() );

      return pixels[ ( y * this->getCanvasPitch() / 2 ) + x ];
    }
    break;

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( this->getCanvasPixels() );

      return pixels[ ( y * this->getCanvasPitch() ) + x ];
    }
    break;

    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( this->getCanvasPixels() );

      return pixels[ ( y * this->getCanvasPitch()/4 ) + x ];
    }
    break;
  } // end switch
}

bool Canvas::resize ( enum ResizeAlgorithm scaling_algorithm )
{
/*
  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Current video surface flags state -- the destination buffer will be set
  // with these.
  uint32 flags = this->getCanvasFlags();

  // This is the target video surface object that is created from the existing
  // video surface, with the existing width & height recomputed to whatever the
  // chosen algorithm expects. The target buffer (upon success) becomes the new
  // video surface of this instance.
  Canvas destination_buffer;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  int32 scale_factor = getResizeScaleFactor ( scaling_algorithm );

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  uint32 alpha_mask = 0; // no alpha mask is default (works with color keying)

  // If the video surface does *NOT* have color keying set
  if ( ! ( flags & SDL_TRUE ) )//SDL_SRCCOLORKEY ) )
  {
    alpha_mask = this->getCanvasAlphaMask();
  }

  destination_buffer = Canvas (
                                this->getCanvasWidth() * scale_factor,
                                this->getCanvasHeight() * scale_factor,
                                this->getCanvasBitsPerPixel(),
                                this->getCanvasRedMask(),
                                this->getCanvasGreenMask(),
                                this->getCanvasBlueMask(),
                                alpha_mask,
                                flags
                              );

  // Ensure that our new video surface is sane before feeding
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
    return false;
  }

  switch ( scaling_algorithm )
  {
    default: this->unlock(); return false; break; // No resizing is applied

    case ResizeAlgorithm::scale2x:
    {
      if ( priv::scale2x  (
                            this->getCanvasPixels(),
                            destination_buffer.getCanvasPixels(),
                            this->getCanvasWidth(),
                            this->getCanvasHeight(),
                            this->getCanvasColorDepth(),
                            this->getCanvasPitch(),
                            destination_buffer.getCanvasPitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale2x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::scale3x:
    {
      if ( priv::scale3x  (
                            this->getCanvasPixels(),
                            destination_buffer.getCanvasPixels(),
                            this->getCanvasWidth(),
                            this->getCanvasHeight(),
                            this->getCanvasColorDepth(),
                            this->getCanvasPitch(),
                            destination_buffer.getCanvasPitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale3x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::scale4x:
    {
      if ( priv::scale2x  (
                            this->getCanvasPixels(),
                            destination_buffer.getCanvasPixels(),
                            this->getCanvasWidth(),
                            this->getCanvasHeight(),
                            this->getCanvasColorDepth(),
                            this->getCanvasPitch(),
                            destination_buffer.getCanvasPitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale4x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::hq2x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq2x_32 (
                      static_cast<uint32*> ( this->getCanvasPixels() ),
                      static_cast<uint32*> ( destination_buffer.getCanvasPixels() ),
                      this->getCanvasWidth(), this->getCanvasHeight()
                    );
    }
    break;

    case ResizeAlgorithm::hq3x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq3x_32 (
                      static_cast<uint32*> ( this->getCanvasPixels() ),
                      static_cast<uint32*> ( destination_buffer.getCanvasPixels() ),
                      this->getCanvasWidth(), this->getCanvasHeight()
                    );
    }
    break;

    case ResizeAlgorithm::hq4x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq4x_32 (
                      static_cast<uint32*> ( this->getCanvasPixels() ),
                      static_cast<uint32*> ( destination_buffer.getCanvasPixels() ),
                      this->getCanvasWidth(), this->getCanvasHeight()
                    );
    }
    break;
  } // end switch scaling_algorithm

  this->unlock(); // Relinquish our write lock

  // Do one more sanity check on our new video surface before do the transfer
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // Reset the video surface object's video memory to the rescaled pixel data.
  this->setCanvas ( destination_buffer );
*/
  return true;
}

bool Canvas::resize ( const Point2f& scale_factor )
{
/*
  // set ResizeAlgorithm::Stretch

  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Current video surface flags state -- the destination buffer will be set
  // with these.
  uint32 flags = this->getCanvasFlags();

  // This is the target video surface object that is created from the existing
  // video surface, with the existing width & height recomputed to whatever the
  // chosen algorithm expects. The target buffer (upon success) becomes the new
  // video surface of this instance.
  Canvas destination_buffer;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  //int32 scale_factor = getResizeScaleFactor ( scaling_algorithm );

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  uint32 alpha_mask = 0; // no alpha mask is default (works with color keying)

  // If the video surface does *NOT* have color keying set
  if ( ! ( flags & SDL_TRUE ) )//SDL_SRCCOLORKEY ) )
  {
    //alpha_mask = this->getCanvasAlphaMask();
  }

  destination_buffer = Canvas (
                                this->getCanvasWidth()*scale_factor.x,
                                this->getCanvasHeight()*scale_factor.y,
                                this->getCanvasBitsPerPixel(),
                                this->getCanvasRedMask(),
                                this->getCanvasGreenMask(),
                                this->getCanvasBlueMask(),
                                alpha_mask,
                                flags
                              );

  // Ensure that our new video surface is sane before feeding
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
    return false;
  }

  Vector2f r;
  Color color;
  Pixel pixels;

  double stretch_x = (double) this->getCanvasWidth() * scale_factor.x / (double) this->getCanvasWidth();
  double stretch_y = (double) this->getCanvasHeight() * scale_factor.y / (double) this->getCanvasHeight();

  for ( int y = 0; y < this->getCanvasHeight(); y++ )
  {
    for ( int x = 0; x < this->getCanvasWidth(); x++ )
    {
      for ( int sY = 0; sY < stretch_y; sY++ )
      {
        for ( int sX = 0; sX < stretch_x; sX++ )
        {
          uint32 pixel = this->getPixel ( x, y );
          RGBA::asRGB ( pixel, this->getCanvasPixelsFormat(), color );
          //NOM_DUMP_VAR(color);
          if ( color.red == 0 && color.green == 0 && color.blue == 0 ) continue;

          pixels = Pixel ( ( stretch_x * x ) + sX, ( stretch_y * y ) + sY, color );
          pixels.Draw ( destination_buffer.get() );
        }
      }
    }
  }

  this->unlock(); // Relinquish our write lock

  // Do one more sanity check on our new video surface before do the transfer
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Reset the video surface object's video memory to the rescaled pixel data.
  this->setCanvas ( destination_buffer );
*/
  return true;
}

int32 Canvas::getResizeScaleFactor ( enum ResizeAlgorithm scaling_algorithm )
{
  switch ( scaling_algorithm )
  {
    default: return 1; break;

    case ResizeAlgorithm::hq2x:
    case ResizeAlgorithm::scale2x:
      return 2;
    break;

    case ResizeAlgorithm::scale3x:
    case ResizeAlgorithm::hq3x:
      return 3;
    break;

    case ResizeAlgorithm::scale4x:
    case ResizeAlgorithm::hq4x:
      return 4;
    break;
  }
}


} // namespace nom
