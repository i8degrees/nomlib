/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

******************************************************************************/

//
//  GameOfLife
//
//  Created by Fielding Johnston on 3/11/13.
//
// https://github.com/justfielding/GameOfLife
//
//

#include <iostream>
#include <string>
#include <random> // FIXME

// Pubic nomlib interface headers
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

#define GAMEOFLIFE_DEBUG 0

/// File path name of the resources directory; this must be a relative file path.
const std::string APP_RESOURCES_DIR = "Resources";

/// \brief  Relative file path name of our resource example
const nom::Path p;
const std::string RESOURCE_ICON = APP_RESOURCES_DIR + p.native() + "icon.png";

/// \brief Relative filename path to saved screen shot example
///
/// Default path should resolve to the same directory as the app example
/// executable
const std::string OUTPUT_SCREENSHOT_FILENAME = "screenshot.png";

/// \brief Name of our application.
const std::string APP_NAME = "GameOfLife";

/// \brief Length and width for the square viewing area
///
/// \remarks WINDOW_WIDTH & WINDOW_HEIGHT
const nom::int32 BOARD_SIZE = 800;

/// \brief length and width (in pixels) for the square cells
const nom::sint CELL_SIZE = 16;

/// \brief Maximum number of active windows we will attempt to spawn in this
/// example.
const nom::int32 MAXIMUM_WINDOWS = 1;

const std::string IMG_CREEP = APP_RESOURCES_DIR + p.native() + "creep.png";
const std::string BG_LIGHT = APP_RESOURCES_DIR + p.native() + "bglight.png";
const std::string BG_DARK = APP_RESOURCES_DIR + p.native() + "bgdark.png";

/// \brief Usage example
class App: public nom::SDLApp
{
  public:
    App( nom::int32 argc, char* argv[] )
    {
      NOM_LOG_TRACE( NOM );

      // Fatal error; if we are not able to complete this step, it means that
      // we probably cannot rely on our resource paths!
      if( nom::init( argc, argv ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize nomlib." );
        exit( NOM_EXIT_FAILURE );
      }

      atexit( nom::quit );
    } // end App

    ~App( void )
    {
      NOM_LOG_TRACE( NOM );
    } // end ~App

    bool on_init( void )
    {
      nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        if ( this->window[idx].create( APP_NAME, BOARD_SIZE, BOARD_SIZE, window_flags ) == false )
        {
          return false;
        }

        if( this->window[idx].set_window_icon( RESOURCE_ICON ) == false )
        {
          nom::DialogMessageBox( APP_NAME, "ERROR: Could not load window icon: " + RESOURCE_ICON );
          return false;
        }

        this->window_size[idx] = this->window[idx].size();

        // Scale window contents up by the new width & height
        this->window[idx].set_logical_size( this->window_size[idx].x, this->window_size[idx].y );
      }

      if( this->creep.load( IMG_CREEP ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not load resource file: " + IMG_CREEP );
        return false;
      }

      if( this->bg_light.load( BG_LIGHT ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not load resource file: " + BG_LIGHT );
        return false;
      }

      if( this->bg_dark.load( BG_DARK ) == false )
      {
        nom::DialogMessageBox( APP_NAME, "ERROR: Could not load resource file: " + BG_DARK );
        return false;
      }

      this->spawn();

      return true;
    } // end on_init

    void on_update( float )
    {
      // Rules:
      // 1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
      // 2. Any live cell with two or three live neighbours lives on to the next generation.
      // 3. Any live cell with more than three live neighbours dies, as if by overcrowding.
      // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
      {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
          int neighbors = checkNeighbors( x, y );

          if ( neighbors < 2 && grid[x][y] == 1 )
          {
            if( GAMEOFLIFE_DEBUG ){ std::cout<<x<<","<<y<<" has died from under-population!\n"; }
            bufferGrid[x][y] = 0;
          } else if ( (neighbors == 2 || neighbors == 3) && grid[x][y] == 1)
          {
            if( GAMEOFLIFE_DEBUG ){ std::cout<<x<<","<<y<<" has a healthy amount of neighbors and lives on!\n"; }
            bufferGrid[x][y] = 1;
          } else if ( neighbors > 3 && grid[x][y] == 1)
          {
            if( GAMEOFLIFE_DEBUG ){ std::cout<<x<<","<<y<<" has died from overcrowding!\n"; }
            bufferGrid[x][y] = 0;

          } else if ( neighbors == 3 && grid[x][y] == 0)
          {
            if( GAMEOFLIFE_DEBUG ){ std::cout<<"A new cell has been born at "<<x<<","<<y<<"!\n"; }
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
    }

    void on_draw( nom::IDrawable::RenderTarget& target )
    {
      for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
      {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
          nom::Point2i offset;
          offset.x = x * CELL_SIZE;
          offset.y = y * CELL_SIZE;

          if ( grid[x][y] == 1 )
          {
            this->creep.set_position( offset );
            this->creep.draw( target );
          }
          else
          {
            if ( y % 2 != 0 && x % 2 == 0 )
            {
              this->bg_dark.set_position( offset );
              this->bg_dark.draw( target );
            }
            else
            {
              this->bg_light.set_position( offset );
              this->bg_light.draw( target );
            }
          }
        }
      }
    }

    nom::sint Run( void )
    {
      for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
      {
        this->fps_update[idx].start();
        this->fps[idx].start();
      }

      // 1. Events
      // 2. Logic
      // 3. Render
      while( this->running() == true )
      {
        while( this->poll_event( &this->event ) )
        {
          this->on_event( &this->event );
        }

        for( auto idx = 0; idx < MAXIMUM_WINDOWS; ++idx )
        {
          this->window[idx].update();
          this->fps[idx].update();

          // Refresh the frames per second at 1 second intervals
          if ( this->fps_update[idx].ticks() > 1000 )
          {
            if ( this->show_fps() == true )
            {
              this->window[idx].set_window_title( APP_NAME + " - " + this->fps[idx].asString() + ' ' + "fps" );
            }
            else
            {
              this->window[idx].set_window_title( APP_NAME + " [" + std::to_string(this->window[idx].window_id()) + "]" + " - " + "Display" + ' ' + std::to_string ( this->window[idx].window_display_id() ) );
            }

            this->fps_update[idx].restart();
          } // end refresh cycle
        } // end for MAXIMUM_WINDOWS update loop

        this->on_update( nom::ticks() );

        this->window[0].fill( nom::Color4i::Black );
        this->on_draw( this->window[0] );

        nom::sleep( 500 ); // wait .5 seconds

      } // end while SDLApp::running() is true

      return NOM_EXIT_SUCCESS;
    } // end Run()

  private:
    /// \brief Event handler for key down actions
    ///
    /// \remarks Implements nom::Input::on_key_down
    void on_key_down( const nom::Event& ev )
    {
      switch( ev.key.sym )
      {
        default: break;

        // Use inherited SDLApp::on_app_quit method -- you may also provide your
        // own event handler for this.
        case SDLK_ESCAPE:
        case SDLK_q: this->on_app_quit( ev ); break;

        case SDLK_BACKSLASH:
        {
          if ( this->toggle_fps() )
          {
            // Stub for doing something cool here
          }
          else
          {
            // Stub for doing something cool here
          }
          break;
        }

        case SDLK_F1:
        {
          if( this->window[ev.key.window_id - 1].window_id() == ev.key.window_id )
          {
            if( this->window[ev.key.window_id - 1].save_screenshot( OUTPUT_SCREENSHOT_FILENAME ) == false )
            {
              nom::DialogMessageBox( APP_NAME, "ERROR: Could not save screen-shot");
              break;
            } // end save_screenshot err check
          } // end window_id check
          break;
        }

        // Toggle full-screen
        case SDLK_f:
        {
          if ( this->window[ev.key.window_id - 1].window_id() == ev.key.window_id )
          {
            this->window[ev.key.window_id - 1].toggle_fullscreen();
          } // end window_id match
          break;
        } // end SDLK_f
      } // end switch key
    } // end on_key_down

    void spawn()
    {
      // fill the array with random cells
      int rando = 0;
      srand( NOM_SCAST(uint, time( nullptr ) ) ); // FIXME

      for ( int x = 0; x < BOARD_SIZE / CELL_SIZE; x++ )
      {
        for ( int y = 0; y < BOARD_SIZE / CELL_SIZE; y++ )
        {
          rando = rand() % 2;
          grid[x][y] = rando;
          if ( GAMEOFLIFE_DEBUG ){ std::cout<<"Setting square at "<<x<<","<<y<<" to "<<grid[x][y]<<"\n"; }
        }
      }
    }

    int checkNeighbors( int x, int y )
    {
      int ncount = 0;
      if ( GAMEOFLIFE_DEBUG ){ std::cout<<"Checking neighbor cells for " << x << "," << y << ".\n"; }
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
      if ( GAMEOFLIFE_DEBUG ){ std::cout<<x<<","<<y<<" has "<< ncount << " current neighbor cells.\n"; }

      return ncount;
    }

    // Create a grid based on total board size and each cell size
    nom::sint grid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE] = {{ 0 }};
    nom::sint bufferGrid[BOARD_SIZE / CELL_SIZE][BOARD_SIZE / CELL_SIZE] = {{ 0 }};

    /// \brief Window handles
    ///
    /// \TODO Use std::vector?
    nom::RenderWindow window[MAXIMUM_WINDOWS];

    nom::Point2i window_size[MAXIMUM_WINDOWS];

    /// \brief Interval at which we refresh the frames per second counter
    nom::Timer fps_update[MAXIMUM_WINDOWS];

    /// \brief Timer for tracking frames per second
    nom::FPS fps[MAXIMUM_WINDOWS];

    nom::Texture creep;
    nom::Texture bg_light;
    nom::Texture bg_dark;
}; // end class App

nom::sint main( nom::int32 argc, char* argv[] )
{
  App game( argc, argv );

  if( game.on_init() == false )
  {
    nom::DialogMessageBox( APP_NAME, "ERROR: Could not initialize application." );
    return NOM_EXIT_FAILURE;
  }

  return game.Run();

  // ...Goodbye cruel world!
}
