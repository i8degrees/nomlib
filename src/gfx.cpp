/******************************************************************************
    gfx.cpp

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "gfx.h"

Gfx::Gfx ( unsigned int sdl_flags, unsigned int img_flags )
{
  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::Gfx(): Hello, world!" << "\n" << std::endl;
  #endif

  this->screen = NULL;

  if ( SDL_Init ( sdl_flags ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Gfx() at SDL_Init(): " << SDL_GetError() << std::endl;
    #endif

    exit ( EXIT_FAILURE );
  }

  if ( IMG_Init ( img_flags ) != img_flags )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Gfx() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif

    exit ( EXIT_FAILURE );
  }
}

Gfx::~Gfx ( void )
{
  if ( this->screen != NULL )
  {
    SDL_FreeSurface ( this->screen );
    this->screen = NULL;
  }

  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::~Gfx(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();

  SDL_Quit ();
}

bool Gfx::SetVideoMode (  unsigned int screen_width,
                          unsigned int screen_height,
                          unsigned int screen_bpp,
                          unsigned int video_flags
                        )
{
  this->screen = SDL_SetVideoMode ( screen_width, screen_height, screen_bpp, video_flags );

  if ( this->screen == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetVideoMode(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

bool Gfx::setTransparent (  SDL_Surface *video_buffer,
                            unsigned int r, unsigned int g, unsigned int b,
                            unsigned int flags )
{
  unsigned int transparent_color = 0;

  transparent_color = SDL_MapRGB ( video_buffer->format, r, g, b );

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  if ( SDL_SetColorKey ( video_buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

SDL_Surface *Gfx::LoadImage ( std::string filename, unsigned int flags )
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

  {
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  }
  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

bool Gfx::DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y )
{
  SDL_Rect coords;

  coords.x = x;
  coords.y = y;

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  if ( SDL_BlitSurface ( video_buffer, NULL, this->screen, &coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                        unsigned int x_offset, unsigned int y_offset,
                        unsigned int width_offset, unsigned int height_offset )
{
  SDL_Rect coords, offsets;

  coords.x = x;
  coords.y = y;

  offsets.x = x_offset;
  offsets.y = y_offset;
  offsets.w = width_offset;
  offsets.h = height_offset;

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  if ( SDL_BlitSurface ( video_buffer, &offsets, this->screen, &coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface() at SDL_BlitSurface(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::UpdateScreen ( void )
{
  if ( SDL_Flip ( this->screen ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::DrawRectangle ( unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int r, unsigned int g, unsigned int b )
{
  SDL_Rect rectangle = { 0, 0, 0, 0 };
  unsigned int rectangle_color = 0;

  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  rectangle_color = SDL_MapRGB ( this->screen->format, r, g, b );

  if ( SDL_FillRect ( this->screen, &rectangle, rectangle_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawRectangle(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void Gfx::setTitle ( std::string app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), NULL );
}

// NOTE: *MUST* be called before the first call to SDL_SetVideoMode is made
bool Gfx::setIcon ( std::string app_icon,
                    unsigned int r, unsigned int g, unsigned int b,
                    unsigned int flags )
{
  SDL_Surface *icon_buffer = NULL;

  if ( screen != NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << "SDL_SetVideoMode() has already been called." << std::endl;
    #endif
    return false;
  }

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( Gfx::setTransparent ( icon_buffer, r, g, b, flags ) == false )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;

  return true;
}

unsigned int Gfx::getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y )
{
    //Convert the pixels to 32 bit
    unsigned int *pixels = (unsigned int *) video_buffer->pixels;

    //Get the pixel requested
    return pixels[ ( y * video_buffer->w ) + x ];
}
