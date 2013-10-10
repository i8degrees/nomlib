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
#include "App.hpp"

App::App ( nom::int32 args_count, char* args[] )
{
  NOM_LOG_TRACE ( NOM );

  nom::File dir;
  // Use this class to obtain the platform's directory delimiter
  nom::Path p;
  // Form the PWD (parent working directory); this must be an absolute file
  // path.
  std::string pwd = "\0";

  pwd = dir.path ( args[0] ) + p.native() + APP_RESOURCES_DIR;

  // Change the working directory to whatever pwd has been set to.
  //
  // This should generally be done *after* processing command line
  // arguments!
  dir.setPath ( pwd );
}

App::~App ( void )
{
NOM_LOG_TRACE ( NOM );
}

bool App::onInit ( void )
{
  nom::uint32 window_flags = SDL_WINDOW_RESIZABLE;

  for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
  {
    if ( this->window[idx].create ( APP_NAME, WINDOW_WIDTH/2, WINDOW_HEIGHT, window_flags ) == false )
    {
      return false;
    }

    this->window[idx].set_position ( 0+(WINDOW_WIDTH/2) * idx, WINDOW_HEIGHT/2 );

    if ( this->window[idx].set_window_icon ( RESOURCE_ICON ) == false )
    {
      return false;
    }
  }

  this->ui_frame = nom::ui::GrayFrame ( 50, 50, 200, 200 );
  this->linear.setSize ( 200 - 4, 200 - 4 );
  this->linear.setMargins ( 4, 4 );
  this->linear.setFillDirection ( nom::Gradient::FillDirection::Top );

  this->Running(); // If all is well, here goes nothing!
  return true;
}

void App::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
{
  switch ( key )
  {
    default: break;

    // Use inherited SDL_App::onQuit() method -- you may also provide your own
    // event handler for this.
    case SDLK_ESCAPE:
    case SDLK_q: this->onQuit(); break;

    case SDLK_BACKSLASH: this->toggleFPS(); break;

    case SDLK_f:
    {
      if ( this->isFullScreen() == true )
      {
        this->window[0].fullscreen ( 0 );
        this->setFullScreen ( false );
      }
      else
      {
        this->window[0].fullscreen ( SDL_WINDOW_FULLSCREEN_DESKTOP );
        this->setFullScreen ( true );
      }
    }
    break;
  } // end switch key
}

nom::int32 App::Run ( void )
{
  for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
  {
    this->update[idx].start();
    this->fps[idx].start();
  }

  // 1. Events
  // 2. Logic
  // 3. Render
  while ( this->isRunning() == true )
  {
    while ( this->PollEvents ( &this->event ) )
    {
      this->onEvent ( &this->event );
    }

    this->window[0].fill ( nom::Color::NomSecondaryColorKey );
    this->window[1].fill ( nom::Color::NomPrimaryColorKey );
    this->window[2].fill ( nom::Color::Black );

    for ( auto idx = 3; idx < MAXIMUM_WINDOWS; idx++ )
    {
      nom::int32 random_color = nom::rand ( 1, 11 );

      switch ( random_color )
      {
        default: this->window[idx].fill ( nom::Color::Black ); break;

        case 1: this->window[idx].fill ( nom::Color::White ); break;
        case 2: this->window[idx].fill ( nom::Color::Red ); break;
        case 3: this->window[idx].fill ( nom::Color::Green ); break;
        case 4: this->window[idx].fill ( nom::Color::Blue ); break;
        case 5: this->window[idx].fill ( nom::Color::Yellow ); break;
        case 6: this->window[idx].fill ( nom::Color::Magenta ); break;
        case 7: this->window[idx].fill ( nom::Color::Cyan ); break;
        case 8: this->window[idx].fill ( nom::Color::LightGray ); break;
        case 9: this->window[idx].fill ( nom::Color::Gray ); break;
        case 10: this->window[idx].fill ( nom::Color::NomPrimaryColorKey ); break;
        case 11: this->window[idx].fill ( nom::Color::NomSecondaryColorKey ); break;
      }
    }

    nom::Line line1 = nom::Line ( ((WINDOW_WIDTH-176)*2)/2, 194*2, 176*2, 24*2, nom::Color( 133, 133, 133 ) );
    line1.draw ( this->window[0].renderer() );

    nom::Point pixel = nom::Point ( 500, 245, nom::Color::White );
    pixel.draw ( this->window[2].renderer() );

    this->linear.setStartColor ( nom::Color ( 251, 222, 232 ) );
    this->linear.setEndColor ( nom::Color ( 114, 66, 66 ) );
    this->linear.setPosition ( 50, 50 );
    this->linear.update();
    this->linear.draw( this->window[2].renderer() );
    this->ui_frame.draw ( this->window[2].renderer() );

    nom::Line line2 = nom::Line ( 100, 100, 250, 250, nom::Color( 133, 133, 133 ) );
    line2.draw ( this->window[1].renderer() );

    nom::Rectangle rectangle = nom::Rectangle ( 100, 100, 200, 200, nom::Color::Gray );
    rectangle.draw ( this->window[0].renderer() );

    if ( this->isFullScreen() == true )
    {
      this->window[0].fill ( nom::Color::NomPrimaryColorKey );
      this->window[1].fill ( nom::Color::NomSecondaryColorKey );
    }

    for ( auto idx = 0; idx < MAXIMUM_WINDOWS; idx++ )
    {
      this->window[idx].update();
      this->fps[idx].update();
      this->ui_frame.update();

      // Refresh the frames per second at 1 second intervals
      if ( this->update[idx].ticks() > 1000 )
      {
        if ( this->getShowFPS() == true )
        {
          this->window[idx].set_window_title ( APP_NAME + " " + std::to_string(this->window[idx].window_id()) + " - " + this->fps[idx].asString() + '\x20' + "fps" );
        }
        else
        {
          this->window[idx].set_window_title ( APP_NAME + " " + std::to_string(this->window[idx].window_id()) + " - " + "Display" + " " + std::to_string ( this->window[idx].window_display_id() ) );
        }

        this->update[idx].restart();
      } // end refresh cycle
    } // end for MAXIMUM_WINDOWS update loop
  } // end while isRunning() is true

  return EXIT_SUCCESS;
}

nom::int32 main ( nom::int32 argc, char* argv[] )
{
  App game ( argc, argv );

  if ( game.onInit() == false )
  {
NOM_LOG_ERR ( NOM_EXAMPLES, "Could not initialize game." );
    return EXIT_FAILURE;
  }

  return game.Run();

  // END OF EXECUTION; anything beyond this comment will never get executed!
}
