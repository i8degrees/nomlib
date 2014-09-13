#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

#include <SDL.h>

#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Debugger/Debugger.h>

#include "nomlib/librocket/ShellFileInterface.hpp"
#include "nomlib/librocket/RocketSDL2Renderer.hpp"
#include "nomlib/librocket/RocketSDL2SystemInterface.hpp"

/// \see https://github.com/libRocket/libRocket/tree/master/Samples/basic/sdl2/src
TEST( libRocketTest, SDL2SamplesTest )
{
  // Wrapper for SDL Window
  nom::RenderWindow window;
  window.create( "LibRocket SDL2 test", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

  // Not used
  // SDL_GLContext glcontext = SDL_GL_CreateContext( window.window() );

  // Wrapper for SDL_Renderer

  // TOOD:
  int oglIdx = -1;
  int nRD = SDL_GetNumRenderDrivers();
  for( int i = 0; i < nRD; ++i )
  {
    SDL_RendererInfo info;
    if( ! SDL_GetRenderDriverInfo( i, &info ) )
    {
      NOM_DUMP(info.name);
      if( ! strcmp (info.name, "opengl" ) )
      {
          oglIdx = i;
      }
    }
  }

  // TODO: Wrapper for SDL Renderer
  // nom::Renderer renderer;
  // renderer.create( oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

  // Initialize OpenGL for libRocket engine
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
  glLoadIdentity();
  glOrtho( 0, window.size().w, window.size().h, 0, 0, 1 );

  nom::RocketSDL2Renderer Renderer( window.renderer(), window.window() );
  nom::RocketSDL2SystemInterface SystemInterface;

  // nom::init sets the working directory to this executable's directory path
  nom::ShellFileInterface FileInterface( "./Resources/librocket/" );

  Rocket::Core::SetFileInterface( &FileInterface );
  Rocket::Core::SetRenderInterface( &Renderer );
  Rocket::Core::SetSystemInterface( &SystemInterface );

  if( ! Rocket::Core::Initialise() )
  {
    FAIL();
  }

  Rocket::Core::Context* context = Rocket::Core::CreateContext("default",
    Rocket::Core::Vector2i( window.size().w, window.size().h ));

  if( Rocket::Debugger::Initialise( context ) == false )
  {
    FAIL();
  }

  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Bold.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-BoldItalic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Italic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Roman.otf" );

  Rocket::Core::ElementDocument* Document = context->LoadDocument( "demo.rml" );

  Rocket::Core::Log::Message( Rocket::Core::Log::LT_INFO, "hi" );

  if( Document )
  {
    Document->Show();
    Document->RemoveReference();
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document is loaded" );
  }
  else
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document was NULL." );
    FAIL();
  }

  bool done = false;

  while( !done )
  {
    SDL_Event event;
    while( SDL_PollEvent( &event ) )
    {
      switch( event.type )
      {
        case SDL_QUIT:
        {
          done = true;
        }
        break;

        case SDL_MOUSEMOTION:
        {
          context->ProcessMouseMove( event.motion.x, event.motion.y, SystemInterface.GetKeyModifiers() );
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
        {
          context->ProcessMouseButtonDown( SystemInterface.TranslateMouseButton( event.button.button ), SystemInterface.GetKeyModifiers() );
        }
        break;

        case SDL_MOUSEBUTTONUP:
        {
          context->ProcessMouseButtonUp( SystemInterface.TranslateMouseButton( event.button.button ), SystemInterface.GetKeyModifiers() );
        }
        break;

        case SDL_MOUSEWHEEL:
        {
          context->ProcessMouseWheel( SystemInterface.TranslateMouseWheel( event.wheel.y ), SystemInterface.GetKeyModifiers() );
        }
        break;

        case SDL_KEYDOWN:
        {
          // Check for a shift-~ to toggle the debugger.
          switch( event.key.keysym.sym )
          {
            // Note that the key is consumed
            case SDLK_BACKQUOTE:
            {
              if( event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT )
              {
                Rocket::Debugger::SetVisible( ! Rocket::Debugger::IsVisible() );
                break;
              }
            }
          }

          context->ProcessKeyDown( SystemInterface.TranslateKey( event.key.keysym.sym ), SystemInterface.GetKeyModifiers() );
          break;
        }

        default:
        {
          break;
        }
      }
    }

    window.update();
    context->Update();

    window.fill( nom::Color4i::SkyBlue );
    context->Render();
  }

  context->RemoveReference();
  Rocket::Core::Shutdown();

  SUCCEED();
}

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
