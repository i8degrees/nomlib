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
#include "Template.hpp"

App::App ( nom::int32 args_count, char* args[] )
{
  NOM_LOG_TRACE ( NOM );

  nom::File dir;
  // Use this class to obtain the platform's directory delimiter
  nom::Path p;
  // Form the PWD (parent working directory); this must be an absolute file
  // path.
  std::string pwd = "\0";

  // Define these definitions at build time via header files or passing
  // manually to your compiler.
  #if defined ( OSXAPP ) // OS X Application Bundle; assumes Darwin platform
    // <app_name.app/Contents/Resources
    pwd = nom::getBundleResourcePath();
  #elif defined ( PLATFORM_WINDOWS )
    pwd = APP_INSTALL_PREFIX + APP_NAME + p.native() + APP_RESOURCES_DIR;
  #elif defined ( PLATFORM_POSIX ) // TODO / Assume POSIX-compatible platform
    pwd = APP_INSTALL_PREFIX + p.native() + "share" + p.native() + APP_NAME + p.native() + APP_RESOURCES_DIR;
  #else
    pwd = dir.path ( args[0] ) + p.native() + APP_RESOURCES_DIR;
  #endif

  // Change the working directory to whatever pwd has been set to.
  //
  // This should generally be done *after* processing command line
  // arguments!
  dir.setPath ( pwd );
  NOM_DUMP_VAR ( pwd );
}

App::~App ( void )
{
NOM_LOG_TRACE ( NOM );
}

bool App::onInit ( void )
{
  if ( this->context.create ( WINDOW_WIDTH, WINDOW_HEIGHT ) == false )
  {
    return false;
  }

  this->context.setWindowTitle ( APP_NAME );

  this->Running(); // If all is well, here goes nothing!

  return true;
}

void App::onKeyDown ( nom::int32 key, nom::int32 mod )
{
  switch ( key )
  {
    default: break;

    case SDLK_ESCAPE:
    case SDLK_q: this->onQuit(); break;

    case SDLK_f: this->toggleFPS(); break;
  }
}

nom::int32 App::Run ( void )
{
  this->update.start();
  this->fps.start();

  // 1. Events
  // 2. Logic
  // 3. Render
  while ( this->isRunning() == true )
  {
    while ( this->PollEvents ( &this->event ) )
    {
      this->onEvent ( &this->event );
    }

    this->context.Update();

    this->context.clear ( nom::Color::Gray ); // RGBA: 97, 97, 97, 255

    this->fps.update();

    // Refresh the frames per second at 1 second intervals
    if ( this->update.ticks() > 1000 )
    {
      if ( this->getShowFPS() == true )
      {
        this->context.setWindowTitle ( APP_NAME + " - " + this->fps.asString() + '\x20' + "fps" );
      }
      else
      {
        this->context.setWindowTitle ( APP_NAME );
      }
      this->update.restart();
    }
  } // end while isRunning() is true

  return NOM_EXIT_SUCCESS;
}

nom::int32 main ( nom::int32 argc, char* argv[] )
{
  App game ( argc, argv );

  if ( game.onInit() == false )
  {
NOM_LOG_ERR ( NOM_EXAMPLES, "Could not initialize game." );
    return NOM_EXIT_FAILURE;
  }

  return game.Run();

  // END OF EXECUTION; anything beyond this comment will never get executed.
}
