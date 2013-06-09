/******************************************************************************
    gfx.cpp

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#include "gfx.h"

using namespace nom;

bool Gfx::setAlpha (  void* video_buffer, unsigned char opacity,
                      unsigned int flags )
{
  #ifdef DEBUG_GFX
    if ( opacity > 255 || opacity < 1 )
      std::cout << "ERR in Gfx::setAlpha(): " << "opacity value is out of bounds." << std::endl;
  #endif

  SDL_SetAlpha ( (SDL_Surface*) video_buffer, flags, ( unsigned int ) opacity );

  return true;
}

bool Gfx::setTransparent (  void* video_buffer, const nom::Color& color,
                            unsigned int flags )
{
  SDL_Surface *buffer = (SDL_Surface*) video_buffer; // FIXME

  unsigned int transparent_color = 0;

  if ( buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  // TODO: Alpha value needs testing
  transparent_color = color.getColorAsInt ( buffer->format );

  if ( SDL_SetColorKey ( buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

///
/// TODO: Alpha value needs testing
///
/// Must be called after video init due to SDL_DisplayFormat call being made
///
void* Gfx::LoadImage ( std::string filename, const nom::Color& colorkey, unsigned int flags )
{
  SDL_Surface *temp_buffer = NULL;
  SDL_Surface *video_buffer = NULL;

  temp_buffer = IMG_Load ( filename.c_str() );

  if ( temp_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif

    return NULL;
  }

  setTransparent ( temp_buffer, colorkey, flags );

  // Add check in for if SDL_SRCALPHA flag is set
  if ( colorkey.getAlpha() != -1 )
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  else
    video_buffer = SDL_DisplayFormat ( temp_buffer );

  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

bool Gfx::DrawSurface ( void* source_buffer, void* video_buffer,
                        const nom::Coords& coords, const nom::Coords& offsets
                      )
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = coords.getSDL_Rect();
  SDL_Rect blit_offsets = offsets.getSDL_Rect();

  if ( (SDL_Surface*) source_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): NULL surface" << std::endl;
    #endif

    return false;
  }

  if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
  {
    if ( SDL_BlitSurface ( (SDL_Surface*) source_buffer, &blit_offsets, (SDL_Surface*)video_buffer, &blit_coords ) != 0 )
    {
      #ifdef DEBUG_GFX
        std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
      #endif
      return false;
    }
  }
  else
    SDL_BlitSurface ( (SDL_Surface*) source_buffer, NULL, (SDL_Surface*)video_buffer, &blit_coords );

  return true;
}

bool Gfx::updateSurface ( void* video_buffer )
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

// 32-bit bpp
unsigned int Gfx::getPixel ( void* video_buffer, unsigned int x, unsigned int y )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  //Convert the pixels to 32 bit
  unsigned int *pixels = (unsigned int *) buffer->pixels;

  //Get the pixel requested
  return pixels[ ( y * buffer->w ) + x ];
}

// 32-bit bpp
void Gfx::setPixel ( void* video_buffer, unsigned int x, unsigned int y, nom::Color color )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  unsigned char * pixel = (unsigned char *)buffer->pixels;

  pixel += (y * buffer->pitch) + (x * sizeof(unsigned int));

  *((unsigned int *)pixel) = color.getColorAsInt ( buffer->format );
}

void Gfx::drawLine ( void* video_buffer, float x1, float y1, float x2, float y2, nom::Color color )
{
    // Bresenham's line algorithm
    bool steep = ( fabs ( y2 - y1 ) > fabs ( x2 - x1 ) );

    if ( steep )
    {
        std::swap ( x1, y1 );
        std::swap ( x2, y2 );
    }

    if ( x1 > x2 )
    {
        std::swap ( x1, x2 );
        std::swap ( y1, y2 );
    }

    float dx = x2 - x1;
    float dy = fabs ( y2 - y1 );

    float error = dx / 2.0f;
    int ystep = ( y1 < y2 ) ? 1 : -1;
    int y = (int)y1;

    int maxX = (int)x2;

    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            Gfx::setPixel ( video_buffer, y, x, color );
        }
        else
        {
            Gfx::setPixel ( video_buffer, x, y, color);
        }

        error -= dy;
        if(error < 0)
        {
          y += ystep;
          error += dx;
        }
    }
}

bool Gfx::lockSurface ( void* video_buffer )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  if ( SDL_MUSTLOCK ( buffer ) )
  {
    SDL_LockSurface ( buffer );
  }
  else
    return false;

  return true;
}

bool Gfx::unlockSurface ( void* video_buffer )
{
  SDL_Surface* buffer = (SDL_Surface*) video_buffer;

  if ( SDL_MUSTLOCK ( buffer ) )
  {
    SDL_UnlockSurface ( buffer );
  }
  else
    return false;

  return true;
}
