#include <iostream>
#include <string>
#include <map>

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

#include "nomlib/librocket/DecoratorPhotograph.hpp"

namespace nom {

/// \brief Helper method for automating the creation of a document window
// Rocket::Core::ElementDocument* load_window( Rocket::Core::Context* ctx, const std::string& filename, const Point2i& pos );

Rocket::Core::ElementDocument* load_window( Rocket::Core::Context* ctx, const std::string& filename, const nom::Point2i& pos )
{
  Rocket::Core::ElementDocument* doc = nullptr;

  doc = ctx->LoadDocument( filename.c_str() );

  if( doc )
  {
    doc->Show();
    // NOM_DUMP( doc->GetReferenceCount() );
    doc->RemoveReference();
    // NOM_DUMP( doc->GetReferenceCount() );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", doc->GetSourceURL().CString(), "is loaded." );
  }
  else
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", doc->GetSourceURL().CString(), "was NULL." );
    return nullptr;
  }

  // Absolute positioning of the window in global-space -- relative to the
  // rendering back-end (SDL2)
  if( pos != nom::Point2i::null )
  {
    doc->SetProperty( "left", Rocket::Core::Property( pos.x, Rocket::Core::Property::ABSOLUTE_UNIT ) );
    doc->SetProperty( "top", Rocket::Core::Property( pos.y, Rocket::Core::Property::ABSOLUTE_UNIT ) );
  }

  // Set the title of the window to that of the title element's text
  doc->GetElementById( "title" )->SetInnerRML( doc->GetTitle() );

  return doc;
}

class libRocketTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    libRocketTest() :
      running( true ),
      renderer( nullptr ),
      sys( nullptr ),
      filesystem( nullptr ),
      context( nullptr )
    {
      // NOM_LOG_TRACE( NOM );

      // Platform specific initialization of fonts (system, user, engine) that
      // are available to us at run-time.
      PlatformSettings::initialize();
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~libRocketTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE( NOM );

      // Required interface as per libRocket SDL2 implementation
      if( nom::set_hint( SDL_HINT_RENDER_DRIVER, "opengl" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not enable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not enable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not set scale quality to", "nearest" );
      }

      this->window.create( "nomlib & LibRocket integration tests", 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

      // Not used
      // SDL_GLContext glcontext = SDL_GL_CreateContext( this->window.window() );

      // TODO:
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
      // this->renderer.create( oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

      // Initialize OpenGL for libRocket engine
      SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
      glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
      glLoadIdentity();
      glOrtho( 0, this->window.size().w, this->window.size().h, 0, 0, 1 );

      this->renderer = new nom::RocketSDL2Renderer( this->window.renderer(), this->window.window(), &this->window );

      if( this->renderer == nullptr )
      {
        FAIL() << "Could not create SDL2 Renderer for libRocket.";
      }

      // nom::init sets the working directory to this executable's directory path;
      // i.e.: build/tests. Now all the documents (including their dependencies)
      // that are loaded in all use the same root source path.
      //
      // FIXME: File organization needs to be dealt with somehow, for document
      // reloading feature... Copying file resources is not ideal during development
      // (only for release).
      // nom::ShellFileInterface FileInterface( "./Resources/librocket/nomlibTest/" );
      this->filesystem = new nom::ShellFileInterface( "../../Resources/tests/gui/librocket/nomlibTest/" );

      if( this->filesystem == nullptr )
      {
        FAIL() << "Could not create Shell File Interface for libRocket.";
      }

      this->sys = new nom::RocketSDL2SystemInterface();

      if( this->sys == nullptr )
      {
        FAIL() << "Could not create System Interface for libRocket.";
      }

      Rocket::Core::SetFileInterface( this->filesystem );
      Rocket::Core::SetRenderInterface( this->renderer );
      Rocket::Core::SetSystemInterface( this->sys );

      if( ! Rocket::Core::Initialise() )
      {
        FAIL();
      }

      // Necessary for form elements
      Rocket::Controls::Initialise();

      this->context = Rocket::Core::CreateContext("default",
        Rocket::Core::Vector2i( window.size().w, window.size().h ));

      // Initialize Debugger as early as possible, so we can visually see logging.
      if( Rocket::Debugger::Initialise( this->context ) == false )
      {
        FAIL();
      }

      Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Bold.otf" );
      Rocket::Core::FontDatabase::LoadFontFace( "Delicious-BoldItalic.otf" );
      Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Italic.otf" );
      Rocket::Core::FontDatabase::LoadFontFace( "Delicious-Roman.otf" );

      // Additional fonts
      Rocket::Core::FontDatabase::LoadFontFace( nom::SystemFonts::cache().find_resource( "Arial" ).path().c_str() );
      Rocket::Core::FontDatabase::LoadFontFace( nom::SystemFonts::cache().find_resource( "OpenSans" ).path().c_str() );
      Rocket::Core::FontDatabase::LoadFontFace( nom::SystemFonts::cache().find_resource( "OpenSans-Bold" ).path().c_str() );

      // Load custom decorators for nomlib
      Rocket::Core::DecoratorInstancer* decorator0 = new nom::DecoratorInstancerFinalFantasyFrame();
      Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", decorator0 );
      decorator0->RemoveReference();

      // Load the default in-window cursor
      Rocket::Core::ElementDocument* cursor = this->context->LoadMouseCursor( "./cursor.rml" );

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

      // int pos_x = this->doc0->GetProperty<int>( "left" );
      // int pos_y = this->doc0->GetProperty<int>( "top" );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );

      this->context->RemoveReference();
      Rocket::Core::Shutdown();
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \brief Main loop
    virtual int on_run()
    {
      while( this->running )
      {
        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
          switch( event.type )
          {
            case SDL_QUIT:
            {
              this->running = false;
            }
            break;

            case SDL_MOUSEMOTION:
            {
              this->context->ProcessMouseMove( event.motion.x, event.motion.y, this->sys->GetKeyModifiers() );
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            {
              this->context->ProcessMouseButtonDown( this->sys->TranslateMouseButton( event.button.button ), this->sys->GetKeyModifiers() );
            }
            break;

            case SDL_MOUSEBUTTONUP:
            {
              context->ProcessMouseButtonUp( this->sys->TranslateMouseButton( event.button.button ), this->sys->GetKeyModifiers() );
            }
            break;

            case SDL_MOUSEWHEEL:
            {
              this->context->ProcessMouseWheel( this->sys->TranslateMouseWheel( event.wheel.y ), this->sys->GetKeyModifiers() );
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
                  this->running = false;
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
                // case SDLK_h:
                // {
                //   if( doc0 != nullptr )
                //   {
                //     if( this->doc0->IsVisible() == true )
                //     {
                //       this->doc0->Hide();
                //     }
                //     else
                //     {
                //       this->doc0->Show();
                //     }
                //   }
                //   break;
                // }

                // Reload document, and its dependencies (i.e.: templates and style
                // sheets) during run-time.
                case SDLK_r:
                {
                  for( auto itr = this->docs.begin(); itr != this->docs.end(); ++itr )
                  {
                    if( (*itr).second != nullptr )
                    {
                      (*itr).second->Close();
                      Rocket::Core::Factory::ClearStyleSheetCache();
                      Rocket::Core::Factory::ClearTemplateCache();
                      this->docs[ (*itr).first ] = this->context->LoadDocument( (*itr).first.c_str() );

                      if( (*itr).second != nullptr )
                      {
                        (*itr).second->Show();
                        (*itr).second->RemoveReference();
                        // NOM_DUMP( this->doc0->GetReferenceCount() );
                        NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", (*itr).second->GetSourceURL().CString(), "reloaded." );
                      }
                      else
                      {
                        NOM_LOG_CRIT(NOM_LOG_CATEGORY_GUI, "Document", (*itr).second->GetSourceURL().CString(), "was NULL.");
                        return NOM_EXIT_FAILURE;
                      }
                    }
                  }

                  break;
                }
              }

              this->context->ProcessKeyDown( this->sys->TranslateKey( event.key.keysym.sym ), this->sys->GetKeyModifiers() );
              break;
            }

            default:
            {
              break;
            }
          }
        }

        this->window.update();
        this->context->Update();

        this->window.fill( nom::Color4i::SkyBlue );
        this->context->Render();
      }

      // SUCCEED();
      return NOM_EXIT_SUCCESS;
    }

  protected:
    bool running;

    /// Wrapper for SDL Cursor
    nom::Cursor cursor_mgr;

    /// Wrapper for SDL Window && SDL Renderer
    nom::RenderWindow window;

    /// Rendering bridge between libRocket & nomlib
    nom::RocketSDL2Renderer* renderer;

    /// System interfacing (events, loop, ...) bridge between libRocket & nomlib
    nom::RocketSDL2SystemInterface* sys;

    /// Filesystem bridge between libRocket & nomlib
    nom::ShellFileInterface* filesystem;

    /// UI Desktop ('windows' container)
    Rocket::Core::Context* context;

    /// UI Windows; 'widgets' containers
    std::map<std::string, Rocket::Core::ElementDocument*> docs;
};

/// \brief nomlib & libRocket (using SDL2 back-end) interface sanity tests.
///
/// \see Derived from https://github.com/libRocket/libRocket/tree/master/Samples/basic/sdl2/src
TEST_F( libRocketTest, BaseIntegrationTest )
{
  // As per the positioning units used for nom::MessageBox ex0 in
  // examples/gui_messagebox.cpp
  Point2i pos( 38, 25 );
  std::string doc_file = "./demo.rml";

  // Assumes base directory path of FileInterface (see above)
  Rocket::Core::ElementDocument* doc = nom::load_window( this->context, doc_file, pos );

  // Test visual debugger logs
  Rocket::Core::Log::Message( Rocket::Core::Log::LT_INFO, "Hello, world!" );

  if( doc )
  {
    EXPECT_STREQ( "INFO.", doc->GetTitle().CString() )
    << "Document title should be the text of the title element: 'INFO.'";
  }
  else
  {
    FAIL()
    << "Document should not be NULL";
  }

  this->docs[doc_file] = doc;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( libRocketTest, RenderTwoDocumentWindows )
{
  Point2i pos0( 38, 25 );
  std::string doc_file0 = "./demo.rml";

  // As per the positioning units used for nom::QuestionDialogBox (ex2) in
  // examples/gui_messagebox.cpp
  nom::Point2i pos1 = nom::Point2i(38,141);

  std::string doc_file1 = "./questionbox.rml";

  Rocket::Core::ElementDocument* doc0 = load_window( this->context, doc_file0, pos0 );

  if( doc0 )
  {
    EXPECT_STREQ( "INFO.", doc0->GetTitle().CString() )
    << "Document title should be the text of the title element: 'INFO.'";
  }
  else
  {
    FAIL()
    << "Document should not be NULL";
  }

  Rocket::Core::ElementDocument* doc1 = load_window( this->context, doc_file1, pos1 );

  if( doc1 )
  {
    EXPECT_STREQ( "CHOICE", doc1->GetTitle().CString() )
    << "Document title should be the text of the title element: 'CHOICE'";
  }
  else
  {
    FAIL()
    << "Document should not be NULL";
  }

  this->docs[doc_file0] = doc0;
  this->docs[doc_file1] = doc1;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F( libRocketTest, EventListenerTest )
{
  // nom::MouseButtonEventListener evt;

  // As per the positioning units used for nom::QuestionDialogBox (ex2) in
  // examples/gui_messagebox.cpp
  nom::Point2i pos = nom::Point2i(38,141);
  std::string doc_file = "./questionbox.rml";

  Rocket::Core::Element* content_div = nullptr;

  Rocket::Core::ElementDocument* doc = load_window( this->context, doc_file, pos );

  if( doc )
  {
    EXPECT_STREQ( "CHOICE", doc->GetTitle().CString() )
    << "Document title should be the text of the title element: 'CHOICE'";
  }
  else
  {
    FAIL()
    << "Document should not be NULL";
  }

  // Resulting elements matching query
  Rocket::Core::ElementList tags;

  content_div = doc->GetElementById("content");
  if( content_div )
  {
    Rocket::Core::ElementUtilities::GetElementsByClassName( tags, content_div, "choice" );
  }
  else
  {
    FAIL()
    << "Could not find content element within "
    << doc->GetSourceURL().CString();
  }

  EXPECT_EQ( 2, tags.size() )
  << "I expected a total sum of two choices (YES and NO)!";

  for( auto itr = tags.begin(); itr != tags.end(); ++itr )
  {
    NOM_DUMP( (*itr)->GetTagName().CString() );

    // evt.RegisterEvent( *itr );
    nom::MouseButtonEventListener::RegisterEvent( *itr );
  }

  this->docs[doc_file] = doc;

  // Two different ways of simulating events:
  // content_div->Click();
  // Rocket::Core::Dictionary params;

  // params.Set("button","1");
  // content_div->DispatchEvent("mousedown", params);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );

  // content_div->RemoveEventListener( "mouseup", &evt );
}

} // namespace nom

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
