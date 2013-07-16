/******************************************************************************

    nomlib App Demo

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "nomlib/graphics.hpp"
#include "nomlib/system.hpp"

#define dump_var(var) NOMLIB_DUMP_VAR(var)
const std::string APP_NAME = "nomlib App Demo";

class App: public nom::SDL_App
{
  public:
    App ( void )
    {
      this->display.createWindow ( 720, 480, 32 );
    }

    ~App ( void )
    {
      // ...
    }

    void onKeyDown ( int32_t key, int32_t mod )
    {
      switch ( key )
      {
        default: break;

        case SDLK_ESCAPE:
        case SDLK_q: this->onQuit(); break;
        case SDLK_f: this->toggleFPS(); break;
      }
    }

    int32_t Run ( void )
    {
      this->fps.Start();

      this->Running(); // ...here we go!

      while ( this->isRunning() == true )
      {
        while ( this->PollEvents ( &this->event ) )
        {
          this->onEvent ( &this->event );
        }

        if ( this->getShowFPS() )
          this->display.setWindowTitle ( APP_NAME + " " + "-" + " " + std::to_string ( this->fps.getFPS() ) + " " + "fps" );
        else
          this->display.setWindowTitle ( APP_NAME );

        this->fps.Update();
        } // isRunning() == true

        return EXIT_SUCCESS;
      }
  private:
    /// display context
    nom::SDL_Display display;
    /// timer for tracking frames per second
    nom::FPS fps;
    /// Input events
    SDL_Event event;
};

int main ( int argc, char* argv[] )
{
  App engine;

  return engine.Run();
}
