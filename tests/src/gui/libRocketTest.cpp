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

/// \brief nomlib & libRocket (using SDL2 back-end) interface sanity tests.
///
/// \see Derived from https://github.com/libRocket/libRocket/tree/master/Samples/basic/sdl2/src
///
/// \todo Rename to BaseIntegrationTest
///
/// \todo Split up
TEST( libRocketTest, nomlibSamplesTest )
{
  // Wrapper for SDL Cursor
  nom::Cursor cursor_mgr;

  if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
  {
    NOM_LOG_INFO ( NOM, "Could not enable vertical refresh." );
  }

  if( nom::set_hint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" ) == false )
  {
    NOM_LOG_INFO( NOM, "Could not set scale quality to", "nearest" );
  }

  // Wrapper for SDL Window && SDL Renderer
  nom::RenderWindow window;
  window.create( "LibRocket SDL2 test", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

  // Not used
  // SDL_GLContext glcontext = SDL_GL_CreateContext( window.window() );

  // int oglIdx = -1;
  // int nRD = SDL_GetNumRenderDrivers();
  // for( auto i = 0; i < nRD; ++i )
  // {
  //   SDL_RendererInfo info;
  //   if( ! SDL_GetRenderDriverInfo( i, &info ) )
  //   {
  //     if( ! strcmp( info.name, "opengl" ) )
  //     {
  //       oglIdx = i;
  //     }
  //   }
  // }

  // TODO: Wrapper for SDL Renderer?
  // nom::Renderer renderer;
  // renderer.create( oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

  // Initialize OpenGL for libRocket engine
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
  glLoadIdentity();
  glOrtho( 0, window.size().w, window.size().h, 0, 0, 1 );

  nom::RocketSDL2Renderer Renderer( window.renderer(), window.window(), &window );
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

  // Necessary for form elements
  Rocket::Controls::Initialise();

  Rocket::Core::Context* context = Rocket::Core::CreateContext("default",
    Rocket::Core::Vector2i( window.size().w, window.size().h ));

  // Initialize Debugger as early as possible, so we can visually see logging.
  if( Rocket::Debugger::Initialise( context ) == false )
  {
    FAIL();
  }

  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Bold.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-BoldItalic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Italic.otf" );
  Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Roman.otf" );

  Rocket::Core::DecoratorInstancer* instancer = new nom::DecoratorInstancerFinalFantasyFrame();
  Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", instancer);
  instancer->RemoveReference();

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
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", doc->GetSourceURL().CString(), "is loaded." );

    EXPECT_STREQ( "INFO.", doc->GetTitle().CString() )
    << "Document title should be the text of the title element: 'INFO.'";
  }
  else
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", doc->GetSourceURL().CString(), "was NULL." );
    FAIL();
  }

  // Set the title of the window to that of the title element's text
  doc->GetElementById( "title" )->SetInnerRML( doc->GetTitle() );

  // Load the default in-window cursor
  Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor( "./cursor.rml" );

  if( cursor )
  {
    cursor->Show();
    cursor->RemoveReference();
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Cursor", cursor->GetSourceURL().CString(), "is loaded." );

    EXPECT_STREQ( "default", cursor->GetTitle().CString() )
    << "Document title should be the text of the title element: 'default'";
  }
  else
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Cursor", cursor->GetSourceURL().CString(), "was NULL." );
    FAIL();
  }

  // Let libRocket handle the rendering of the in-window cursor
  cursor_mgr.show_cursor( false );

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

                if( doc != nullptr )
                {
                  doc->Show();
                  doc->RemoveReference();
                  // NOM_DUMP( doc->GetReferenceCount() );
                  NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", doc->GetSourceURL().CString(), "reloaded." );
                }
                else
                {
                  FAIL()
                  << "Document " << doc->GetSourceURL().CString()
                  << " was NULL.";
                }
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
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
