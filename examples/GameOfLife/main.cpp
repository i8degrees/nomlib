//
//  main.c
//  GameOfLife
//
//  Created by Fielding Johnston on 3/11/13.
//
//

#include <iostream>
#include <time.h>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "timer.h"
#include "texture.h"

#ifndef __MINGW32__
//#include "SDL/SDL_rotozoom.h"
#else
#include "Windows.h"
#endif

#ifdef EMSCRIPTEN
#include "emscripten.h"
#include "SDL_rotozoom.h"
#endif

using namespace std;

#define DEBUGINFO 0
#define TEXTURES 1

#define CELL_SIZE 4         // length and width (in pixels) for the square cells
#define BOARD_SIZE 1600     // Length and width for the square viewing area
#define SCREEN_BPP 32       // Screen bits per-pixels

#ifdef APP
#define IMG_CELL "GameOfLife.app/Contents/Resources/img/pink.png"
#define BG_LIGHT "GameOfLife.app/Contents/Resources/img/bglight.png"
#define BG_DARK "GameOfLife.app/Contents/Resources/img/bgdark.png"
#define FONT_COMFORTAA "GameOfLife.app/Contents/Resources/font/Comfortaa-Regular.ttf"
#elif __MINGW32__
#define IMG_CELL "pink.png"
#define BG_LIGHT "bglight.png"
#define BG_DARK "bgdark.png"
#define FONT_COMFORTAA "Comfortaa-Regular.ttf"

#else
#define IMG_CELL "assets/img/pink.png"
#define BG_LIGHT "assets/img/bglight.png"
#define BG_DARK "assets/img/bgdark.png"
#define FONT_COMFORTAA "assets/font/Comfortaa-Regular.ttf"

#endif

bool running = true;
bool showFPS = false;
// Generations/updates per second
int genPerSec = 12;
int grid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];
int bufferGrid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE];
int generation = 0;
int framecount = 0;

bool init();
void quit();

bool loadAssets();

void spawn();

void handleEvents();
void update();
void draw(bool textures = false);

void mainloop(); // main loop wrapper for Emscripten

int checkNeighbors( int x, int y );
void drawCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
SDL_Texture* loadTexture(std::string path);

// SDL Objects
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
TTF_Font *font = NULL;
SDL_Color textColor = { 255, 255, 255 };

Texture cell;
Texture bg_light;
Texture bg_dark;
Texture fpsTextTexture;
Timer fpsTimer;
Timer frameticks;

bool init()
{
  bool initialized = true;
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf( "SDL failed to initialize! SDL_Error: %s\n", SDL_GetError() );
    initialized = false;
  }
  else
  {
    if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
      printf( "Warning: Linear texture filtering not enabled!" );
    }
    window = SDL_CreateWindow( "Conway's Game of Life by Fielding", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE, BOARD_SIZE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if ( window == NULL )
    {
      printf( "SDL window could not be created! SDL_Error: %s\n", SDL_GetError() );
      initialized = false;
    }
    else
    {
      renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
      if ( renderer == NULL )
      {
        printf( "SDL renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        initialized = false;
      }
      else
      {
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
          initialized = false;
        }

        if( TTF_Init() == -1 )
        {
          printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
          initialized = false;
        }
      }
    }
  }
  return initialized;
}

void quit()
{
  cell.free();
  bg_light.free();
  bg_dark.free();

  TTF_CloseFont( font );
  font = NULL;

  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  renderer = NULL;
  window = NULL;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

bool loadAssets()
{
  bool loaded = true;

  if ( !cell.loadFromFile( IMG_CELL, renderer ) )
  {
    printf( "Failed to load %s\n", IMG_CELL );
    loaded = false;
  }

  if ( !bg_light.loadFromFile( BG_LIGHT, renderer ) )
  {
    printf( "Failed to load %s\n", BG_LIGHT );
    loaded = false;
  }

  if ( !bg_dark.loadFromFile( BG_DARK, renderer ) )
  {
    printf( "Failed to load %s\n", BG_DARK );
    loaded = false;
  }

  font = TTF_OpenFont( FONT_COMFORTAA, 28 );
  if ( font == NULL )
  {
    printf( "Failed to laod comfortaa font! SDL_ttf Error: %s\n", TTF_GetError() );
    loaded = false;
  }
  return loaded;
}

void spawn()
{
  // fill the array with random cells
  int rando = 0;
  //#ifndef __MINGW32__
  srand(( unsigned )time( NULL ));
  //#else
  //  srand( ( unsigned ) GetSystemTime() );
  //#endif

  for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
  {
    for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
    {
      rando = rand() % 2;
      grid[x][y] = rando;
      if ( DEBUGINFO ){ cout<<"Setting square at "<<x<<","<<y<<" to "<<grid[x][y]<<"\n"; }
    }
  }
}

void mainloop()
{
  float avgFPS = framecount / ( fpsTimer.getTicks() / 1000.f );
  if ( avgFPS > 2000000)
  {
    avgFPS = 0;
  }

  fpsTextTexture.loadFromRenderedText( to_string(avgFPS), textColor, font, renderer );
  handleEvents();
  update();
  draw(TEXTURES);
}

void handleEvents()
{
    while ( SDL_PollEvent( &event ) )
    {
      if ( event.type == SDL_KEYDOWN )   // If a Key was pressed
      {
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:   // If key pressed was escape
            running = false;      // exit the program
            break;

          case SDLK_UP:       // If key pressed was up arrow
            break;

          case SDLK_DOWN:     // If key pressed was down arrow
            break;

          case SDLK_f:
            if ( showFPS )
            {
              showFPS = false;
            }
            else
            {
              showFPS = true;
            }
            break;
          default:            // default for undefined keys
            break;
        }
      } else if ( event.type == SDL_QUIT )
      {
        running = false;    // exit the program
      }
    }

}

void update()
{
    // Rules:
    // 1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overcrowding.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction

    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            int neighbors = checkNeighbors( x, y );

            if ( neighbors < 2 && grid[x][y] == 1 )
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from under-population!\n"; }
                bufferGrid[x][y] = 0;
            } else if ( (neighbors == 2 || neighbors == 3) && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has a healthy amount of neighbors and lives on!\n"; }
                bufferGrid[x][y] = 1;
            } else if ( neighbors > 3 && grid[x][y] == 1)
            {
                if( DEBUGINFO ){ cout<<x<<","<<y<<" has died from overcrowding!\n"; }
                bufferGrid[x][y] = 0;

            } else if ( neighbors == 3 && grid[x][y] == 0)
            {
                if( DEBUGINFO ){ cout<<"A new cell has been born at "<<x<<","<<y<<"!\n"; }
                bufferGrid[x][y] = 1;
            }
        }
    }

    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
            grid[x][y] = bufferGrid[x][y];
        }
    }
  ++generation;
}

void draw(bool textures)
{
  SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderClear( renderer );

  if (textures)
  {

    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
      for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
      {
        if ( grid[x][y] == 1)
        {
          cell.render(x * CELL_SIZE, y * CELL_SIZE, renderer);
        }
        else
        {
          if ( y % 2 != 0 && x % 2 == 0 )
          {
            bg_dark.render(x * CELL_SIZE, y * CELL_SIZE, renderer);
          }
          else
          {
            bg_light.render(x * CELL_SIZE, y * CELL_SIZE, renderer);
          }
        }
      }
    }
  }
  else
  {


    for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
    {
      for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
      {
        if ( grid[x][y] == 1 )
        {
          drawCell(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, 0xFF, 0x00, 0x00, 0xFF);
        }
      }
    }
  }
  if (showFPS)
  {
    fpsTextTexture.render( ( BOARD_SIZE - fpsTextTexture.getWidth() ) /  2, (BOARD_SIZE - fpsTextTexture.getHeight() ) / 2, renderer);
  }

  SDL_RenderPresent( renderer );
  framecount++;
}

int checkNeighbors( int x, int y )
{
    int ncount = 0;
    if ( DEBUGINFO ){ cout<<"Checking neighbor cells for " << x << "," << y << ".\n"; }
    for ( int a = x - 1; a <= x + 1; a++ )
    {
        for ( int b = y - 1; b <= y + 1; b++)
        {
            if ( grid[a][b] == 1 && a >= 0 && a <= ( BOARD_SIZE / CELL_SIZE - 1 ) && b >= 0 && b <= ( BOARD_SIZE / CELL_SIZE - 1 ) )
            {
                ncount++;
            }
        }
    }

    if ( grid[x][y] ) { ncount--; } // subtract self from neighbor count if currently living
    if ( DEBUGINFO ){ cout<<x<<","<<y<<" has "<< ncount << " current neighbor cells.\n"; }

    return ncount;
}

void drawCell( Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{

  SDL_Rect rect = {x, y, w, h};
  SDL_SetRenderDrawColor( renderer, r, g, b, a );
  SDL_RenderFillRect( renderer, &rect );

}

int main ( int argc, char **argv )
{
  if ( !init() )
  {
    printf( "Failed to initialize!\n" );
  }
  else
  {
    if ( !loadAssets() )
    {
      printf ( "Failed to load assets!\n" );
    }
    else
    {
      spawn();;
      std::stringstream fpsText;
      fpsTimer.start();
      framecount = 0;

#ifdef EMSCRIPTEN
      emscripten_set_main_loop(mainloop, 10, 1);;
#else
      while ( running )
      {
        frameticks.start();
        mainloop();
      }
#endif
    }
  }
  quit();
  return 0;
}
