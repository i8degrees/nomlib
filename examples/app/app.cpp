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

******************************************************************************/

// App class usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

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
    nom::Display display;
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
