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
#include <Rocket/Controls.h>

#include "nomlib/librocket/ShellFileInterface.hpp"
#include "nomlib/librocket/RocketSDL2Renderer.hpp"
#include "nomlib/librocket/RocketSDL2SystemInterface.hpp"

/// \brief Base engine sanity test
///
/// \remarks Verbatim copied source, less and except where noted.
///
/// \see https://github.com/libRocket/libRocket/tree/master/Samples/basic/sdl2/src
TEST( libRocketTest, DISABLED_SDL2SamplesTest )
{
  SDL_Init( SDL_INIT_VIDEO );
  SDL_Window * screen = SDL_CreateWindow("LibRocket SDL2 test", 20, 20, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  // SDL_GLContext glcontext = SDL_GL_CreateContext(screen);
  int oglIdx = -1;
  int nRD = SDL_GetNumRenderDrivers();
  for(int i=0; i<nRD; i++)
  {
      SDL_RendererInfo info;
      if(!SDL_GetRenderDriverInfo(i, &info))
      {
          if(!strcmp(info.name, "opengl"))
          {
              oglIdx = i;
          }
      }
  }
  SDL_Renderer * renderer = SDL_CreateRenderer(screen, oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // GLEW doesn't compile on my system (Mac OS X v10.9.3), but fortunately, we
  // don't need it, anyway -- just the system distributed OpenGL header files.

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  glMatrixMode(GL_PROJECTION|GL_MODELVIEW);
  glLoadIdentity();
  glOrtho(0, 640, 480, 0, 0, 1);

  nom::RocketSDL2Renderer Renderer(renderer, screen);
  nom::RocketSDL2SystemInterface SystemInterface;

  // nom::init sets the working directory to this executable's directory path;
  // i.e.: build/tests/
  nom::ShellFileInterface FileInterface( "./Resources/librocket/SDL2SamplesTest/" );

  Rocket::Core::SetFileInterface( &FileInterface );
  Rocket::Core::SetRenderInterface( &Renderer );
  Rocket::Core::SetSystemInterface( &SystemInterface );

  if( ! Rocket::Core::Initialise() )
  {
    // return 1;
    FAIL(); // We are inside a unit test.
  }

  // Moved font resource loading to after Debugger initialization so we can
  // visually see the errors using said Debugger.

  Rocket::Core::Context *Context = Rocket::Core::CreateContext("default",
    Rocket::Core::Vector2i(640, 480));

  if( Rocket::Debugger::Initialise( Context ) == false )
  {
    FAIL(); // We are inside a unit test.
  }

  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Bold.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-BoldItalic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Italic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Roman.otf" );

  Rocket::Core::ElementDocument *Document = Context->LoadDocument("./demo.rml");

  if(Document)
  {
    Document->Show();
    Document->RemoveReference();
    fprintf(stdout, "\nDocument loaded\n");
  }
  else
  {
    fprintf(stdout, "\nDocument is NULL\n");
    FAIL(); // We are inside a unit test.
  }

    bool done = false;

  while(!done)
  {
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    Context->Render();
    SDL_RenderPresent(renderer);

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_MOUSEMOTION:
                Context->ProcessMouseMove(event.motion.x, event.motion.y, SystemInterface.GetKeyModifiers());
                break;

            case SDL_MOUSEBUTTONDOWN:
                Context->ProcessMouseButtonDown(SystemInterface.TranslateMouseButton(event.button.button), SystemInterface.GetKeyModifiers());
                break;

            case SDL_MOUSEBUTTONUP:
                Context->ProcessMouseButtonUp(SystemInterface.TranslateMouseButton(event.button.button), SystemInterface.GetKeyModifiers());
                break;

            case SDL_MOUSEWHEEL:
                Context->ProcessMouseWheel(event.wheel.y, SystemInterface.GetKeyModifiers());
                break;

            case SDL_KEYDOWN:
                Context->ProcessKeyDown(SystemInterface.TranslateKey(event.key.keysym.sym), SystemInterface.GetKeyModifiers());
                break;
            default:
                break;
        }
    }

    Context->Update();
  }

  Context->RemoveReference();
  Rocket::Core::Shutdown();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);

  // We must not shutdown SDL just yet, otherwise we lose logging facilities
  // in the next test... nom::quit will take care of this for us (see main).
  // SDL_Quit();

  // return 0
  SUCCEED();  // We are inside a unit test
}

/// \brief nomlib integration test.
TEST( libRocketTest, nomlibSamplesTest )
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

  // nom::init sets the working directory to this executable's directory path;
  // i.e.: build/tests
  //
  // FIXME: File organization needs to be dealt with somehow, for document
  // reloading feature... Copying file resources is not ideal during development
  // (only for release).
  // nom::ShellFileInterface FileInterface( "./Resources/librocket/nomlibTest/" );
  nom::ShellFileInterface FileInterface( "../../Resources/tests/gui/librocket/nomlibTest/" );

  Rocket::Core::SetFileInterface( &FileInterface );
  Rocket::Core::SetRenderInterface( &Renderer );
  Rocket::Core::SetSystemInterface( &SystemInterface );

  if( ! Rocket::Core::Initialise() )
  {
    FAIL();
  }

  Rocket::Core::Context* context = Rocket::Core::CreateContext("default",
    Rocket::Core::Vector2i( window.size().w, window.size().h ));

  // Initialize Debugger as early as possible, so we can visually see logging.
  if( Rocket::Debugger::Initialise( context ) == false )
  {
    FAIL();
  }

  // Necessary for form elements
  Rocket::Controls::Initialise();

  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Bold.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-BoldItalic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Italic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Roman.otf" );

  // Assumes base directory path of FileInterface (see above)
  Rocket::Core::ElementDocument* doc = context->LoadDocument( "./demo.rml" );

  // Test visual debugger logs
  Rocket::Core::Log::Message( Rocket::Core::Log::LT_INFO, "Hello, world!" );

  if( doc )
  {
    doc->Show();
    // NOM_DUMP( doc->GetReferenceCount() );
    doc->RemoveReference();
    // NOM_DUMP( doc->GetReferenceCount() );
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
            // Quit loop
            case SDLK_q:
            {
              done = true;
              break;
            }

            case SDLK_BACKQUOTE:
            {
              if( event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT )
              {
                Rocket::Debugger::SetVisible( ! Rocket::Debugger::IsVisible() );
                break;
              }
            }

            // Test showing and hiding documents
            case SDLK_h:
            {
              if( doc != nullptr )
              {
                if( doc->IsVisible() == true )
                {
                  doc->Hide();
                }
                else
                {
                  doc->Show();
                }
              }
              break;
            }

            // Reload document, and its dependencies (i.e.: templates and style
            // sheets) during run-time.
            case SDLK_r:
            {
              if( doc != nullptr )
              {
                doc->Close();
                // context->UnloadDocument( doc );
                Rocket::Core::Factory::ClearStyleSheetCache();
                Rocket::Core::Factory::ClearTemplateCache();
                doc = context->LoadDocument( "./demo.rml" );

                doc->Show();
                doc->RemoveReference();
                // NOM_DUMP( doc->GetReferenceCount() );
              }
              break;
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

  // Log all messages
  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
