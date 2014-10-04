#include <iostream>
#include <string>
#include <map>

#include <SDL.h>

#include "gtest/gtest.h"

#include <Rocket/Core.h>
#include <Rocket/Core/Input.h>

// nom::UnitTest framework
#include "nomlib/tests/common.hpp"

#include <nomlib/config.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/librocket.hpp>

#include "nomlib/tests/gui/librocket/CardsDataSource.hpp"

/// \brief Disable NOM_ASSERT macros so that they do not interfere with tests
/// that check for failure conditions; i.e.: libRocketDataGridTest
/// ::DataSourceModelSanity.
#if defined( NOM_ASSERT )
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr)
#endif // defined NOM_ASSERT

using namespace Rocket::Core;

namespace nom {

void on_keydown( Rocket::Core::Event& ev, UIDataViewList* store )
{
  EXPECT_TRUE( store != nullptr );

  int pg = store->current_page();
  int num_pages = store->total_pages();

  int per_page = store->per_page();
  int selected_card = store->selection();

  EXPECT_TRUE( store->document() != nullptr );

  CardsDataSource* model = store->data_source();
  EXPECT_TRUE( model != nullptr );

  if( ev == "keydown" )
  {
    Input::KeyIdentifier key = NOM_SCAST(Input::KeyIdentifier, ev.GetParameter<int>("key_identifier", 0) );

    // Handle paging (left && right)
    if( key == Input::KI_LEFT && pg > 0 )
    {
      --pg;
    }
    else if( key == Input::KI_RIGHT && ( pg < num_pages - 1 ) )
    {
      ++pg;
    }

    // Handle cursor (up && down)
    else if( key == Input::KI_UP && selected_card > 0 )
    {
      --selected_card;
    }
    else if( key == Input::KI_DOWN && ( selected_card < per_page - 1 ) )
    {
      ++selected_card;
    }

    store->set_current_page(pg);
    store->set_selection(selected_card);

    Element* page_left = store->document()->GetElementById("page-left");
    NOM_ASSERT( page_left != nullptr );
    Element* page_right = store->document()->GetElementById("page-right");
    NOM_ASSERT( page_right != nullptr );

    page_left->SetClassNames("");
    page_right->SetClassNames("");

    // Handling logic visual page indicators
    if( store->current_page() > 0 ) {
      if( page_left ) {
        page_left->SetClassNames("page-left");
      }
    }

    if( store->current_page() >= 0 && (store->current_page() < store->total_pages() - 1) ) {
      if( page_right ) {
        page_right->SetClassNames("page-right");
      }
    }

    // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "KEYDOWN event from", target->GetId().CString(), "value:", key );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Pg:", store->current_page() );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Selection:", store->selection() );
  } // end if keydown

  EXPECT_TRUE( store->set_column_title(1, "CARDS P. " + std::to_string(pg+1) ) );

  EXPECT_EQ( 11, model->num_rows() );
  EXPECT_EQ( pg, store->current_page() );
  EXPECT_EQ( selected_card, store->selection() );

} // end func on_keydown

void on_mouseup( Rocket::Core::Event& ev, UIDataViewList* store  )
{
  nom::StringList row;

  ASSERT_TRUE( store != nullptr );

  int selection = 0;
  Rocket::Core::Element* target = ev.GetTargetElement();

  CardsDataSource* model = store->data_source();
  ASSERT_TRUE( model != nullptr );

  CardCollection* db = store->database();
  ASSERT_TRUE( db != nullptr );

  if( ev == "mouseup" )
  {
    if( target->GetTagName() == "card" )
    {
      Card c = db->lookup_by_name( target->GetInnerRML().CString() );
      selection = c.id();
      store->set_selection( selection );

      NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card name:", target->GetInnerRML().CString() );
      NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card ID:", store->selection() );

      EXPECT_EQ( 11, model->num_rows() );
      EXPECT_EQ( selection, store->selection() );

      if( target->IsClassSet("unavailable-card") )
      {
        target->SetClassNames("available-card");
      }
      else
      {
        target->SetClassNames("unavailable-card");

        // Card card = db->lookup_by_id( store->selection() );

        // if( card.num() > 0 )
        // {
        //   card.set_num( card.num() - 1 );
        // }

        // model->insert_card( selection, card );

        // target = store->document()->GetElementById( card.name().c_str() );
        // NOM_ASSERT( target != nullptr );
        // if( target )
        // {
        //   target->SetClassNames("unavailable-card");
        // }
      }

      // TODO: The store's selection ID needs to be translated
      // model->row( store->selection(), "name", row );
      // model->row( store->selection(), "num", row );
      // if( row.size() == 2 )
      // {
        // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card name (from row):", row.at(0) );
        // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card number (from row):", row.at(1) );
      // }
    } // end if target

  } // end if click
} // end func on_mouseup

void on_mousescroll( Rocket::Core::Event& ev, UIDataViewList* store )
{
  ASSERT_TRUE( store != nullptr );

  int per_page = store->per_page();
  int selected_card = store->selection();

  // Element* target = ev.GetTargetElement();

  CardsDataSource* model = store->data_source();
  ASSERT_TRUE( model != nullptr );

  if( ev == "mousescroll" )
  {
    Input::KeyIdentifier wheel = NOM_SCAST(Input::KeyIdentifier, ev.GetParameter<int>("wheel_delta", -1) );

    // Handle cursor (up && down)
    if( wheel < 0 && selected_card > 0 )
    {
      --selected_card;
    }
    else if( wheel > 0 && ( selected_card < per_page - 1 ) )
    {
      ++selected_card;
    }

    store->set_selection(selected_card);

    // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "WHEEL event from", target->GetId().CString(), "value:", wheel );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Pg:", store->current_page() );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Selection:", store->selection() );

    EXPECT_EQ( 11, model->num_rows() );
    // EXPECT_EQ( pg, store->current_page() );
    EXPECT_EQ( selected_card, store->selection() );

  } // end if mousescroll
} // end func on_mousescroll

class libRocketDataGridTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    libRocketDataGridTest()
    {
      // NOM_LOG_TRACE( NOM );

      // Platform specific initialization of fonts (system, user, engine) that
      // are available to us at run-time.
      // PlatformSettings::initialize();

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~libRocketDataGridTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks Initialization callback for VisualUnitTest to act on, instead
    /// of its default rendering setup. A bit of additional setup is
    /// required for plumbing in libRocket into our setup.
    bool init_rendering()
    {
      int render_driver = -1;
      uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
      uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

      // Required interface as per libRocket SDL2 implementation
      if( nom::set_hint( SDL_HINT_RENDER_DRIVER, "opengl" ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not enable the OpenGL rendering driver." );
        return false;
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
      int nRD = SDL_GetNumRenderDrivers();
      for( auto i = 0; i < nRD; ++i )
      {
        SDL_RendererInfo info;
        if( ! SDL_GetRenderDriverInfo( i, &info ) )
        {
          if( ! strcmp( info.name, "opengl" ) )
          {
            render_driver = i;
          }
        }
      }

      if( this->window_.create( this->test_case(),
                                this->resolution(),
                                window_flags,
                                render_driver,
                                render_flags ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize rendering context and window." );
        return false;
      }

      if( nom::RocketSDL2RenderInterface::gl_init( this->window_.size().w, this->window_.size().h ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize OpenGL for libRocket." );
        return false;
      }

      // I don't understand *why*, but when we enable this, we can forgo the use
      // of the glUseProgramObjectARB call in nom::RocketSDL2RenderInterface::RenderGeometry
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

      return true;
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      nom::SearchPath resources;
      std::string ext = ".json";

      // NOM_LOG_TRACE( NOM );

      // VisualUnitTest environment init
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
        new nom::RocketSDL2RenderInterface( &this->window_ );

      // Initialize libRocket's debugger as early as possible, so we get visual
      // logging
      this->desktop.enable_debugger();
      if( this->desktop.create_context( "default", this->resolution(), renderer ) == false )
      {
        FAIL()
        << "Could not initialize libRocket context.";
      }

      EXPECT_EQ( this->resolution(), this->desktop.size() );

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

      DecoratorInstancerSprite* decorator1 = new nom::DecoratorInstancerSprite();
      Rocket::Core::Factory::RegisterDecoratorInstancer("sprite-sheet", decorator1 );
      decorator1->RemoveReference();

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

  protected:
    nom::UIContext desktop;
    CardStatusFormatter card_status_formatter;
    CardFormatter card_formatter;
};

/// \note This unit test requires that the usage of NOM_ASSERT is disabled.
///
/// If asserts are not disabled, the test *should* fail around the point when
/// an invalid erase is tried; since we are expecting a result from the attempt,
/// an assertion interrupts program flow, ergo the test failure.
///
/// \todo Split up into smaller testing units
TEST_F( libRocketDataGridTest, DataSourceModel )
{
  // Buffer objects used for verifying storage model results
  nom::StringList row;
  nom::StringList cols;
  cols.push_back( "name" );
  cols.push_back( "num" );

  CardsDataSource* store = new CardsDataSource("cards_db");

  // Baseline sanity tests

  ASSERT_TRUE( store != nullptr );

  EXPECT_EQ( "cards", store->table_name() )
  << "Table name string should be the default initialized value.";

  EXPECT_EQ( true, store->empty() )
  << "Resulting storage size should be empty (zero).";

  EXPECT_EQ( nom::npos, store->erase_card( 0 ) )
  << "Element insertion position should be invalid.";

  EXPECT_EQ( nom::npos, store->erase_cards( 1, 99 ) )
  << "Element insertion position should be invalid.";
  // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, store->dump() );

  // Creation tests

  CardList cards;
  cards.push_back( Card( 0, "Geezard", 999 ) );
  EXPECT_EQ( 1, store->append_cards( cards ) )
  << "Resulting storage size should be one item.";

  EXPECT_EQ( 2, store->append_card( Card( 89, "testme", 22 ) ) )
  << "Resulting storage size should be two items.";

  EXPECT_EQ( nom::npos, store->insert_card( 11, Card( 0, "Geezard", 99 ) ) )
  << "Element insertion position should be invalid.";

  // Overwrite 'testme' card
  EXPECT_EQ( 2, store->insert_card( 1, Card( 1, "Fungar", 777 ) ) )
  << "Resulting storage size should remain two.";

  EXPECT_EQ( false, store->empty() )
  << "Storage should not be empty.";

  // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, store->dump() );

  // Destruction tests

  // Destroy 'Fungar' card
  EXPECT_EQ( 1, store->erase_card( 1 ) )
  << "Resulting storage size should be one.";

  // ...and verify the results
  row.clear();
  store->row(row, 0, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );

  cards.clear();
  cards.push_back( Card( 2, "Bite Bug", 2 ) );
  cards.push_back( Card( 3, "Red Bat", 5 ) );
  cards.push_back( Card( 89, "Diablos", 666 ) );

  // Overwrite all but the first card in the model, shifting said first card to
  // the end of the container.
  EXPECT_EQ( 4, store->insert_cards( 0, cards ) )
  << "Resulting storage size should be four items.";

  // Ensure that the first card is at the end of the container
  row.clear();
  store->row( row, store->num_rows() - 1, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );
  // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, store->dump() );

  // Destroy all but 'Geezard' card
  EXPECT_EQ( 1, store->erase_cards( 0, 3 ) )
  << "The Geezard card should be the only item remaining.";

  // ...and verify the results
  row.clear();
  store->row(row, 0, cols );
  EXPECT_EQ( "Geezard", row.at(0) );
  EXPECT_EQ( "999", row.at(1) );

  store->erase_cards();
  EXPECT_EQ( true, store->empty() )
  << "Resulting storage size should be emptied (zero).";

  row.clear();
  store->erase_cards();

  NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, store->dump() );
}

/// \todo Add unit test for rendering two UIDataViewList objects; I'm curious
/// about performance offsets, etc.
TEST_F( libRocketDataGridTest, UIDataViewList )
{
  CardsDataSource* model = nullptr;

  // As per the positioning units used for nom::DataViewListTest (ex2) in
  // tests/src/gui/DataViewListTest.cpp.
  Point2i pos1( 25, 25 );
  std::string doc_file1 = "dataview.rml";

  UIDataViewList store1( pos1, Size2i::null );
  EXPECT_EQ( true, store1.set_desktop( this->desktop.context() ) );
  EXPECT_EQ( true, store1.set_document_file( doc_file1 ) );

  store1.set_data_source( new CardsDataSource("cards_db") );
  store1.set_database( new CardCollection() );

  CardCollection* db = store1.database();
  NOM_ASSERT( db != nullptr );

  EXPECT_EQ( true, db->load_db() )
  << "Could not initialize nom::CardsDataSource data interface.";

  // Complete collection of cards as loaded from the CardCollection interface
  CardList cards = db->cards();

  // Initial cards population -- page 0
  CardList pop;

  for( auto itr = cards.begin(); itr != cards.begin() + store1.per_page(); ++itr )
  {
    pop.push_back( *itr );
  }

  model = store1.data_source();
  NOM_ASSERT( model != nullptr );
  if( model )
  {
    model->append_cards( pop );
  }

  if( store1.initialize() == false )
  {
    FAIL()
    << this->test_set() << " object should not be invalid; is the context and document file valid?";
  }

  EXPECT_TRUE( store1.set_column_title(1, "CARDS P. " + std::to_string(store1.current_page() + 1) ) );

  EXPECT_EQ( true, store1.visible() );
  EXPECT_EQ( pos1, store1.position() );

  // Default values sanity

  EXPECT_EQ( "cards", model->table_name() );

  EXPECT_EQ( 0, store1.selection() );
  EXPECT_EQ( 11, store1.per_page() );
  EXPECT_EQ( 0, store1.current_page() );

  EXPECT_EQ( "CARDS P. 1", store1.column_title(1) );
  EXPECT_EQ( "NUM.", store1.column_title(2) );

  store1.register_event_listener( store1.document(), "keydown", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_keydown( ev, &store1 ); }
  ));

  store1.register_event_listener( store1.document(), "mouseup", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_mouseup( ev, &store1 ); }
  ));

  store1.register_event_listener( store1.document(), "mousescroll", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_mousescroll( ev, &store1 ); }
  ));

  // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, store1.dump() );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize nomlib.", nom::MessageBoxType::NOM_DIALOG_ERROR );
    return NOM_EXIT_FAILURE;
  }
  atexit( nom::quit );

  // nom::UnitTest framework integration
  nom::init_test( argc, argv );

  // Log all messages
  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
