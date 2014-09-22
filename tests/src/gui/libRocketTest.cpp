#include <iostream>
#include <string>
#include <map>

#include <SDL.h>

#include "gtest/gtest.h"

// nom::UnitTest framework
#include "nomlib/tests/common.hpp"

#include <nomlib/config.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/librocket.hpp>

#include <Rocket/Core.h>
#include <Rocket/Core/Types.h>
#include <Rocket/Core/Input.h>
#include <Rocket/Debugger/Debugger.h>
#include <Rocket/Controls.h>
#include <Rocket/Controls/DataSource.h>

#if defined( NOM_USE_LIBROCKET_LUA )
  #include <Rocket/Core/Lua/Interpreter.h>
  #include <Rocket/Controls/Lua/Controls.h>
#endif

namespace nom {

// 11 per page for 10 pages for a grand total of 110 cards
const int NUM_CARDS = 11;

/// \see http://librocket.com/wiki/documentation/tutorials/Datagrid
class CardsMenu: public Rocket::Controls::DataSource
{
  public:
    CardsMenu() :
      Rocket::Controls::DataSource("cards_db")
    {
      for( auto i = 0; i != NUM_CARDS; ++i )
      {
        this->cards[i].id = -1;
      }

      this->LoadCards();
    }

    virtual ~CardsMenu()
    {
      // NOM_LOG_TRACE( NOM );
    }

    void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns)
    {
      if( table == "cards" )
      {
        for( auto i = 0; i != columns.size(); ++i )
        {
          if( columns[i] == "name" )
          {
            row.push_back( cards[row_index].name.c_str() );
          }
          else if( columns[i] == "num" )
          {
            row.push_back( cards[row_index].num.c_str() );
          }
        } // end for cols loop
      }
    } // end GetRow func

    int GetNumRows(const Rocket::Core::String& table)
    {
      if( table == "cards" )
      {
        for( auto i = 0; i != NUM_CARDS; ++i )
        {
          if( cards[i].id == -1 )
          {
            // Default value (not counted)
            return i;
          }
        } // end for cards loop

        return NUM_CARDS;
      }

      return 0;
    } // end GetNumRows func

    void dump()
    {
      for( auto i = 0; i != NUM_CARDS; ++i )
      {
        NOM_DUMP(this->cards[i].id);
        NOM_DUMP(this->cards[i].name);
        NOM_DUMP(this->cards[i].num);
      }
    }

  private:
    void SubmitCard( int idx, int id, const std::string& name, int num )
    {
      this->cards[idx] = Card(id, name, num);

      NotifyRowAdd( "cards", idx, 1 );
    }

    void LoadCards()
    {
      SubmitCard(0,0,"Geezard",1);
      SubmitCard(1,1,"Fungar",1);
      SubmitCard(2,2,"Bite Bug",0);
      SubmitCard(3,3,"Red Bat",4);
      SubmitCard(4,4,"Blobra",2);
      SubmitCard(5,5,"Gayla",1);
      SubmitCard(6,6,"Gesper",3);
      SubmitCard(7,7,"Fastitocalon-F",0);
      SubmitCard(8,8,"Blood Soul",1);
      SubmitCard(9,9,"Caterchipillar",2);
      SubmitCard(10,10,"Cockatrice",3);
    }

    // Mock cards store
    struct Card
    {
      Card() {}

      Card( int id, const std::string& name, int num ) :
        id( id ),
        name( name ),
        num{ std::to_string( num ) }
      {
        // ...
      }

      int id = -1;
      std::string name = "\0";
      std::string num = "1";
    };

    Card cards[NUM_CARDS];
};

/// \brief Helper method for automating the creation of a document window
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

/// \brief Helper method for automating the creation of a mouse cursor
Rocket::Core::ElementDocument* load_cursor( Rocket::Core::Context* ctx, const std::string& filename )
{
  Rocket::Core::ElementDocument* cursor = ctx->LoadMouseCursor( filename.c_str() );

  if( cursor )
  {
    cursor->Show();
    cursor->RemoveReference();
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Cursor", cursor->GetSourceURL().CString(), "is loaded." );
  }
  else
  {
    return nullptr;
  }

  return cursor;
}

class libRocketTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    libRocketTest()
    {
      // NOM_LOG_TRACE( NOM );

      // Platform specific initialization of fonts (system, user, engine) that
      // are available to us at run-time.
      PlatformSettings::initialize();

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~libRocketTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks Initialization callback for VisualUnitTest to act on, instead
    /// of its default rendering setup. A bit of additional setup is
    /// required for plumbing in libRocket into our setup.
    void init_rendering()
    {
      // Required interface as per libRocket SDL2 implementation
      if( nom::set_hint( SDL_HINT_RENDER_DRIVER, "opengl" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not enable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
      {
        NOM_LOG_INFO ( NOM, "Could not disable vertical refresh." );
      }

      if( nom::set_hint( SDL_HINT_RENDER_SCALE_QUALITY, "nearest" ) == false )
      {
        NOM_LOG_INFO( NOM, "Could not set scale quality to", "nearest" );
      }

      // Try to force the use of the OpenGL rendering driver; this is required
      // as per the SDL2 implementation for libRocket.
      int oglIdx = -1;
      int nRD = SDL_GetNumRenderDrivers();
      for( auto i = 0; i < nRD; ++i )
      {
        SDL_RendererInfo info;
        if( ! SDL_GetRenderDriverInfo( i, &info ) )
        {
          if( ! strcmp( info.name, "opengl" ) )
          {
            oglIdx = i;
          }
        }
      }

      // Use default resolution, provided by nom::VisualUnitTest
      this->window_.create( "nomlib & LibRocket integration tests", this->resolution().w, this->resolution().h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

      if( nom::RocketSDL2Renderer::gl_init( this->window_.size().w, this->window_.size().h ) == false )
      {
        FAIL() << "Could not initialize OpenGL for libRocket.";
      }

      // I don't understand *why*, but when we enable this, we can forgo the use
      // of the glUseProgramObjectARB call in nom::RocketSDL2Renderer::RenderGeometry
      // until we go full-screen "desktop" mode, in both Windows and OSX.
      // There seems to be a severe drop in FPS (~50%) under OSX, whereas the
      // FPS appears unaffected on my Windows setup...
      // SDL_GLContext glcontext = SDL_GL_CreateContext( this->window_.window() );
      // if( glcontext == nullptr )
      // {
      //   FAIL() << "Could not create OpenGL Context.";
      // }

      // Experimental support for emulating SDL2's independent resolution
      // scaling feature via a "logical viewport" -- this is important to us
      // because TTcards depends on it somewhat at the moment.
      if( nom::set_hint( "NOM_LIBROCKET_EMULATE_SDL2_LOGICAL_VIEWPORT", "1" ) == false )
      {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Could not enable emulated SDL2 independent resolution scaling." );
      }
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      nom::SearchPath resources;
      std::string ext = ".json";

      // NOM_LOG_TRACE( NOM );

      // We must set the rendering init callback before calling ::SetUp()
      this->set_init_rendering_callback( [&] () { this->init_rendering(); } );

      // VisualUnitTest environment init...
      VisualUnitTest::SetUp();

      // nom::init sets the working directory to this executable's directory
      // path; i.e.: build/tests or build/tests/Debug depending on build
      // environment. Everything is relative from here on out.

      // Determine our resources path based on several possible locations --
      // also dependent upon the build environment
      if( resources.load_file( nom::UnitTest::test_set() + ext, "resources" ) == false )
      {
        FAIL()
        << "Could not determine the resource path for "
        << nom::UnitTest::test_set() + ext;
      }

      // Initialize the core of libRocket; these are the core dependencies that
      // libRocket depends on for successful initialization.
      Rocket::Core::FileInterface* fs =
        new nom::RocketFileInterface( resources.path() );

      Rocket::Core::SystemInterface* sys =
        new RocketSDL2SystemInterface();

      if( nom::init_librocket( fs, sys ) == false )
      {
        FAIL()
        << "Could not initialize libRocket.";
      }

      Rocket::Core::RenderInterface* renderer =
        new nom::RocketSDL2Renderer( &this->window_ );

      // Initialize libRocket's debugger as early as possible, so we get visual
      // logging
      this->desktop.enable_debugger();
      if( this->desktop.create_context( "default", this->resolution(), renderer ) == false )
      {
        FAIL()
        << "Could not initialize libRocket context.";
      }

      if( this->desktop.load_font( "Delicious-Bold.otf" ) == false )
      {
        FAIL() << "Could not load font file: Delicious-Bold.otf";
      }

      if( this->desktop.load_font( "OpenSans-Regular.ttf" ) == false )
      {
        FAIL() << "Could not load font file: OpenSans-Regular.ttf";
      }

      if( this->desktop.load_font( "OpenSans-Bold.ttf" ) == false )
      {
        FAIL() << "Could not load font file: OpenSans-Bold.ttf";
      }

      // Load custom decorators for nomlib
      Rocket::Core::DecoratorInstancer* decorator0 = new nom::DecoratorInstancerFinalFantasyFrame();
      Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", decorator0 );
      decorator0->RemoveReference();

      // Load the default in-window cursor
      Rocket::Core::ElementDocument* cur = load_cursor( this->desktop.context(), "arrow-cursor.rml" );

      if( cur )
      {
        EXPECT_STREQ( "arrow", cur->GetTitle().CString() )
        << "Document title should be the text of the title element: 'arrow'";
      }
      else
      {
        FAIL() << "Cursor was NULL.";
      }

      // Resize window cursor
      cur = load_cursor( this->desktop.context(), "diag-cursor.rml" );

      if( cur )
      {
        EXPECT_STREQ( "diag-resize", cur->GetTitle().CString() )
        << "Document title should be the text of the title element: 'diag-resize'";
      }
      else
      {
        FAIL() << "Cursor was NULL.";
      }

      // Let libRocket handle the rendering of the platform's window cursor.
      this->cursor_mgr.show_cursor( false );

      // Register input bindings
      InputActionMapper state;

      // EXPERIMENTAL: Reload document, and its dependencies (i.e.: templates
      // and style sheets) during run-time.
      EventCallback reload_docs( [&] ( const Event& evt )
      {
        this->reload_docs( evt );
      } );

      state.insert  ( "reload_docs",
                       nom::KeyboardAction( SDL_KEYDOWN,
                                            SDLK_r ),
                       reload_docs );

      // Additional input bindings for VisualUnitTest's event loop.
      this->input_mapper_.insert( "reload_docs", state, true );

      /// Put our event polling within the main event's loop
      this->append_event_callback( [&] ( const Event ev ) { this->desktop.process_event( ev ); } );

      // Register GUI updates onto our main loop (::on_run).
      this->append_update_callback( [&] ( float delta ) { this->desktop.update(); } );

      // Register GUI rendering onto our main loop (::on_run).
      this->append_render_callback( [&] ( const RenderWindow& win ) { this->desktop.draw(); } );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );

      for( auto itr = docs.begin(); itr != docs.end(); ++itr )
      {
        // TODO: Re-enable once everything is using common base
        // (*itr).second->RemoveReference();
        (*itr).second->Close();
      }

      this->desktop.shutdown();

      nom::shutdown_librocket();
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

    void on_click( Rocket::Core::Event& event )
    {
      if( event == "mouseup" )
      {
        Rocket::Core::Input::KeyIdentifier button = (Rocket::Core::Input::KeyIdentifier) event.GetParameter<int>("button", 3);
        // Rocket::Core::Input::KeyIdentifier button_x = (Rocket::Core::Input::KeyIdentifier) event.GetParameter<int>("mouse_x", -1);
        // Rocket::Core::Input::KeyIdentifier button_y = (Rocket::Core::Input::KeyIdentifier) event.GetParameter<int>("mouse_y", -1);

        // Rocket::Core::Element* ev = event.GetCurrentElement();
        Rocket::Core::Element* ev = event.GetTargetElement();

        if( button == 0 )
        {
          NOM_DUMP_VAR(NOM_LOG_CATEGORY_GUI, "LEFT button click");
          // NOM_DUMP( ev->GetClassNames().CString() );

          if( ev->GetClassNames().Find( "choice" ) == 0 )
          {
            // question
            NOM_DUMP( ev->GetParentNode()->GetId().CString() );

            // Are you Sure?
            NOM_DUMP( ev->GetOwnerDocument()->GetElementById("message")->GetInnerRML().CString() );

            if( ev->GetClassNames().Find( "choice selected" ) == nom::npos )
            {
              ev->SetClassNames( "choice selected" );
              NOM_DUMP_VAR( NOM, "ON", ev->GetClassNames().CString() );
            }
            else
            {
              ev->SetClassNames( "choice" );
              NOM_DUMP_VAR( NOM, "OFF", ev->GetClassNames().CString() );
            }
          }

          // std::string id = ev->GetId().CString();
          // if( id == "choice1" )
          // {
          //   Rocket::Core::Element* c = ev->GetParentNode()->GetElementById("choice2");
          //   c->SetClassNames( "choice" );
          // }
          // else
          // {
          //   Rocket::Core::Element* c = ev->GetParentNode()->GetElementById("choice1");
          //   c->SetClassNames( "choice" );
          // }
        }
        else if( button == 1 )
        {
          NOM_DUMP_VAR(NOM_LOG_CATEGORY_GUI, "RIGHT button click");
        }
        else if( button == 2 )
        {
          NOM_DUMP_VAR(NOM_LOG_CATEGORY_GUI, "MIDDLE button click");
        }
        else if( button == 3 )
        {
          NOM_DUMP_VAR(NOM_LOG_CATEGORY_GUI, "UNDEFINED button click");
        }
      }
    } // end on_click func

    void on_wheel( Rocket::Core::Event& event )
    {
      if( event == "mousescroll" )
      {
        // Rocket::Core::Input::KeyIdentifier button = (Rocket::Core::Input::KeyIdentifier) event.GetParameter<int>("button", 3);
        int wheel_delta = event.GetParameter<int>( "wheel_delta", 0 );

        NOM_DUMP( wheel_delta );
      }
    } // end on_wheel func

    void on_keydown( Rocket::Core::Event& event )
    {
      if( event == "keydown" )
      {
        Rocket::Core::Input::KeyIdentifier key = NOM_SCAST(Rocket::Core::Input::KeyIdentifier, event.GetParameter<int>("key_identifier", 0) );

        NOM_DUMP( key );
      }
    } // end on_wheel func

    void reload_docs( const Event& evt )
    {
      if( evt.type != SDL_KEYDOWN ) return;

      for( auto itr = this->docs.begin(); itr != this->docs.end(); ++itr )
      {
        if( (*itr).second != nullptr )
        {
          (*itr).second->Close();
          Rocket::Core::Factory::ClearStyleSheetCache();
          Rocket::Core::Factory::ClearTemplateCache();
          this->docs[ (*itr).first ] = this->desktop.context()->LoadDocument( (*itr).first.c_str() );

          if( (*itr).second != nullptr )
          {
            (*itr).second->Show();
            (*itr).second->RemoveReference();
            // NOM_DUMP( this->doc0->GetReferenceCount() );
            NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Document", (*itr).second->GetSourceURL().CString(), "reloaded." );
          }
          else
          {
            NOM_LOG_CRIT(NOM_LOG_CATEGORY_GUI, "Document was NULL.");
            // return NOM_EXIT_FAILURE;
          }
        }
      }
    }

  protected:
    /// Wrapper for SDL Cursor
    nom::Cursor cursor_mgr;

    /// UI Windows; 'widgets' containers
    std::map<std::string, Rocket::Core::ElementDocument*> docs;

    /// \remarks This object must be global, otherwise crashes will often
    /// result.
    // nom::UIEventListener evt;
    nom::UIEventDispatcher evt;

    nom::UIContext desktop;

    /// \remarks This object must be global, otherwise crashes will often
    /// result (UIEventListener related?).
    CardsMenu store;
};

/// \brief nomlib & libRocket (using SDL2 back-end) interface sanity tests.
///
/// \see Derived from https://github.com/libRocket/libRocket/tree/master/Samples/basic/sdl2/src
TEST_F( libRocketTest, BaseIntegrationTest )
{
  // As per the positioning units used for nom::MessageBox ex0 in
  // examples/gui_messagebox.cpp
  Point2i pos( 38, 25 );
  std::string doc_file = "messagebox.rml";

  // Assumes base directory path of FileInterface (see above)
  Rocket::Core::ElementDocument* doc = nom::load_window( this->desktop.context(), doc_file, pos );

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
  EXPECT_TRUE( this->compare() );
}

TEST_F( libRocketTest, RenderTwoDocumentWindows )
{
  Point2i pos0( 38, 25 );
  std::string doc_file0 = "messagebox.rml";

  // As per the positioning units used for nom::QuestionDialogBox (ex2) in
  // examples/gui_messagebox.cpp
  nom::Point2i pos1 = nom::Point2i(38,141);

  std::string doc_file1 = "questionbox.rml";

  Rocket::Core::ElementDocument* doc0 = load_window( this->desktop.context(), doc_file0, pos0 );

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

  Rocket::Core::ElementDocument* doc1 = load_window( this->desktop.context(), doc_file1, pos1 );

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
  EXPECT_TRUE( this->compare() );
}

TEST_F( libRocketTest, EventListenerTest )
{
  // As per the positioning units used for nom::QuestionDialogBox (ex2) in
  // examples/gui_messagebox.cpp
  nom::Point2i pos = nom::Point2i(38,141);
  std::string doc_file = "questionbox.rml";

  Rocket::Core::Element* content_div = nullptr;

  Rocket::Core::ElementDocument* doc = load_window( this->desktop.context(), doc_file, pos );

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

    evt.register_event_listener( *itr, "mousescroll", new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) { this->on_wheel( ev ); } ) );
    evt.register_event_listener( *itr, "keydown", new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) { this->on_keydown( ev ); } ) );
    evt.register_event_listener( *itr, "mouseup", new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) { this->on_click( ev ); } ) );
  }

  this->docs[doc_file] = doc;

  // Two different ways of simulating events:
  // content_div->Click();
  // Rocket::Core::Dictionary params;

  // params.Set("button","1");
  // content_div->DispatchEvent("mousedown", params);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

/// \remarks Models the Cards Menu state of TTcards
TEST_F( libRocketTest, DataGridStore )
{
  // As per the positioning units used for nom::DataViewListTest (ex2) in
  // tests/src/gui/DataViewListTest.cpp.
  Point2i pos( 25, 25 );
  std::string doc_file = "dataview.rml";

  store.dump();

  Rocket::Core::ElementDocument* doc = load_window( this->desktop.context(), doc_file, pos );

  if( doc )
  {
    EXPECT_STREQ( "CARDS.", doc->GetTitle().CString() )
    << "Document title should be the text of the title element: 'CARDS.'";
  }
  else
  {
    FAIL()
    << "Document should not be NULL";
  }

  this->docs[doc_file] = doc;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F( libRocketTest, UIMessageBox )
{
  // As per the positioning units used for nom::MessageBox ex1 in
  // examples/gui_messagebox.cpp
  Point2i pos( 38, 83 );
  Size2i dims( 300, 48 );
  std::string doc_file = "messagebox.rml";

  nom::UIMessageBox mbox( pos, dims );

  EXPECT_EQ( true, mbox.set_desktop( this->desktop.context() ) );
  EXPECT_EQ( true, mbox.set_document_file( doc_file ) );

  if( mbox.initialize() == false )
  {
    FAIL()
    << "UIMessageBox should not be invalid; is the context and document file valid?";
  }

  mbox.set_title_text("INFO.");
  mbox.set_message_text("Diablos");

  EXPECT_EQ( "title", mbox.title_id() );
  EXPECT_EQ( "message", mbox.message_id() );

  EXPECT_EQ( true, mbox.enabled() );

  EXPECT_EQ( "INFO.", mbox.title_text() );
  EXPECT_EQ( "Diablos", mbox.message_text() );

  EXPECT_EQ( nom::Point2i(38, 83), mbox.position() );
  EXPECT_EQ( nom::Size2i(300, 48), mbox.size() );

  EXPECT_EQ( nom::IntRect(43,78,25,16), mbox.title_bounds() );
  EXPECT_EQ( nom::IntRect(53,98,50,23), mbox.message_bounds() );

  EXPECT_EQ( Anchor::TopLeft, mbox.title_alignment() );

  nom::UIEventListener* on_click = new nom::UIEventListener(
    [&] ( Rocket::Core::Event& ev ) { this->on_click( ev ); } );

  mbox.register_event_listener( mbox.document()->GetElementById("message"),
                                "mouseup",
                                on_click );

  // WTF? Why does it think it is MiddleRight?
  // EXPECT_EQ( Anchor::MiddleCenter, mbox.message_alignment() );

  // Rocket::Core::FontDatabase::LoadFontFace( "/Library/Fonts/Arial.ttf" );
  // mbox.set_title_font( "Arial" );
  // mbox.set_title_font_size( 24 );

  // FIXME: Alignment is broken; it's being set, just not taking effect
  // mbox.set_title_alignment( Anchor::TopCenter );

  // mbox.set_message_font( "Delicious" );
  // mbox.set_message_font_size( 24 );

  // FIXME: Alignment is broken; it's being set, just not taking effect
  // mbox.set_message_alignment( Anchor::MiddleRight );

  // FIXME: Reloading RML & RCSS is broken without this (double free issue)
  // this->docs[doc_file] = mbox.document();

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

#if defined( NOM_USE_LIBROCKET_LUA )
TEST_F( libRocketTest, LuaIntegrationTest )
{
  // As per the positioning units used for nom::MessageBox ex0 in
  // examples/gui_messagebox.cpp
  Point2i pos( 38, 25 );
  std::string doc_file = "lua.rml";

  Rocket::Core::Lua::Interpreter::Initialise();

  // Required for forms
  Rocket::Controls::Lua::RegisterTypes( Rocket::Core::Lua::Interpreter::GetLuaState() );

  // Assumes base directory path of FileInterface (see above)
  Rocket::Core::ElementDocument* doc = nom::load_window( this->desktop.context(), doc_file, pos );

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

  // NOM_DUMP( doc->GetElementById("p")->GetInnerRML().CString() );
  evt.register_event_listener( doc->GetElementById("p"), "testme_ev", new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) { NOM_LOG_INFO( NOM, "BINGO!" ); } ) );
  // doc->GetElementById("p")->DispatchEvent("testme_ev", Rocket::Core::Dictionary() );

  this->docs[doc_file] = doc;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );

  Rocket::Core::Lua::Interpreter::Shutdown();
}
#endif // defined NOM_USE_LIBROCKET_LUA

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  // nom::UnitTest framework integration
  nom::init_test( argc, argv );

  // Log all messages
  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
