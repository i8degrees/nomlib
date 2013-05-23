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
  this->running = false;

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
  // cleanup all of the states
  while ( !states.empty() )
    states.pop_back();

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

bool Gfx::setAlpha (  SDL_Surface *video_buffer, unsigned char opacity,
                      unsigned int flags )
{
  #ifdef DEBUG_GFX
    if ( opacity > 255 || opacity < 1 )
      std::cout << "ERR in Gfx::setAlpha(): " << "opacity value is out of bounds." << std::endl;
  #endif

  SDL_SetAlpha ( video_buffer, flags, ( unsigned int ) opacity );

  return true;
}

bool Gfx::setTransparent (  SDL_Surface *video_buffer, GColor color,
                            unsigned int flags )
{
  unsigned int transparent_color = 0;

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  // TODO: Alpha value needs testing
  if ( color.getAlpha() != -1 )
    transparent_color = color.mapRGBA ( video_buffer->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() );
  else
    transparent_color = color.mapRGB ( video_buffer->format, color.getRed(), color.getGreen(), color.getBlue() );

  if ( SDL_SetColorKey ( video_buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

// TODO: Alpha value needs testing
SDL_Surface *Gfx::LoadImage ( std::string filename, GColor colorkey, unsigned int flags )
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

  if ( colorkey.getAlpha() != -1 )
    video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );
  else
    video_buffer = SDL_DisplayFormat ( temp_buffer );


  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
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

  video_buffer = SDL_DisplayFormat ( temp_buffer );

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

bool Gfx::UpdateScreen ( SDL_Surface *video_buffer )
{
  if ( SDL_Flip ( video_buffer ) != 0 )
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

  //if ( color.getAlpha() != -1 )
    //rectangle_color = color.mapRGBA ( this->screen->format, r.getRed(), g.getGreen(), b.getBlue(), color.getAlpha() );
  //else
    rectangle_color = GColor::mapRGB ( this->screen->format, r, g, b );

  if ( SDL_FillRect ( this->screen, &rectangle, rectangle_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawRectangle(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::DrawRectangle ( SDL_Surface *video_buffer, unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int r, unsigned int g, unsigned int b )
{
  SDL_Rect rectangle = { 0, 0, 0, 0 };
  unsigned int rectangle_color = 0;

  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  //if ( color.getAlpha() != -1 )
    //rectangle_color = color.mapRGBA ( this->screen->format, r.getRed(), g.getGreen(), b.getBlue(), color.getAlpha() );
  //else
    rectangle_color = GColor::mapRGB ( video_buffer->format, r, g, b );

  if ( SDL_FillRect ( video_buffer, &rectangle, rectangle_color ) != 0 )
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
bool Gfx::setIcon ( std::string app_icon, GColor color, unsigned int flags )
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

  if ( Gfx::setTransparent ( icon_buffer, color, flags ) == false )
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

// 32-bit bpp
unsigned int Gfx::getPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y )
{
  //Convert the pixels to 32 bit
  unsigned int *pixels = (unsigned int *) video_buffer->pixels;

  //Get the pixel requested
  return pixels[ ( y * video_buffer->w ) + x ];
}

// 32-bit bpp
void Gfx::setPixel ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, GColor color )
{
  unsigned char * pixel = (unsigned char *)video_buffer->pixels;

  pixel += (y * video_buffer->pitch) + (x * sizeof(unsigned int));

  *((unsigned int *)pixel) = GColor::mapRGB ( video_buffer->format, color.getRed(), color.getGreen(), color.getBlue() );
}

void Gfx::drawLine ( SDL_Surface *video_buffer, float x1, float y1, float x2, float y2, GColor color )
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

bool Gfx::lockSurface ( SDL_Surface *video_buffer )
{
  if ( SDL_MUSTLOCK ( video_buffer ) )
  {
    SDL_LockSurface ( video_buffer );
  }
  else
    return false;

  return true;
}

bool Gfx::unlockSurface ( SDL_Surface *video_buffer )
{
  if ( SDL_MUSTLOCK ( video_buffer ) )
  {
    SDL_UnlockSurface ( video_buffer );
  }
  else
    return false;

  return true;
}

void Gfx::ChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  // store the new state
  states.push_back( std::move( state ) );
}

void Gfx::PushState( std::unique_ptr<GameState> state )
{
  // pause current state
  if ( !states.empty() )
    states.back()->Pause();

  // store the new state
  states.push_back( std::move( state ) );
}

void Gfx::PopState ( void )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  // resume previous state
  if ( !states.empty () )
    states.back()->Resume();
}

void Gfx::PopStateThenChangeState( std::unique_ptr<GameState> state )
{
  // cleanup the current state
  if ( !states.empty() )
    states.pop_back();

  if ( !states.empty () )
  {
    Gfx::ChangeState( std::move( state ) );
    //states.back()->ChangeState( this, std::move( state ) );
  }
}

void Gfx::HandleInput ( void )
{
  // let the state handle events
  states.back()->HandleInput ();
}

void Gfx::Update ( void )
{
  // let the state update the scene
  states.back()->Update();
}

void Gfx::Draw( void )
{
  // let the state draw the scene
  states.back()->Draw();
}

bool Gfx::isRunning ( void )
{
  if ( this->running == true )
    return true;
  else
    return false;
}

void Gfx::Run ( void )
{
  this->running = true;
}

void Gfx::Quit ( void )
{
  this->running = false;
}
