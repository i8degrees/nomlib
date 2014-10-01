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
#include <Rocket/Core/Input.h>
#include <Rocket/Debugger/Debugger.h>
#include <Rocket/Controls.h>
#include <Rocket/Controls/DataSource.h>

namespace nom {

using namespace Rocket::Core;

/// \brief A mock card model
///
/// \see TTcards::Card, TTcards::CardCollection
struct Card
{
  /// \remarks Invalid card object.
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

// class CardFormatter: public Rocket::Controls::DataFormatter
// {
//   public:
//     CardFormatter() :
//       Rocket::Controls::DataFormatter("card")
//     {
//     }

//     virtual ~CardFormatter() {}

//     void FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data)
//     {
//       // formatted_data = raw_data[0];
//       formatted_data = "<p class='card' onclick='" + raw_data[0] + "'>" + raw_data[0] + "</p>";
//     }
// };

/// \brief Mock model of the CardsMenu player's hand selection widget in TTcards
///
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Controls/DataGrid
/// \see http://librocket.com/wiki/documentation/tutorials/Datagrid
class CardsMenuModel: public Rocket::Controls::DataSource
{
  public:
    CardsMenuModel( const std::string& source ) :
      Rocket::Controls::DataSource{ source.c_str() }
    {
      // this->load_db();
    }

    /// \brief Destructor.
    ///
    /// \remarks libRocket internally handles deletion of the DataSource object
    /// instance for us.
    virtual ~CardsMenuModel()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \note Required interface implementation.
    virtual void GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns )
    {
      if( table == "cards" )
      {
        for( auto i = 0; i != columns.size(); ++i )
        {
          // TODO:
          if( columns[i] == "id" )
          {
            continue;
          }
          else if( columns[i] == "name" )
          {
            row.push_back( this->db_[row_index].name.c_str() );
          }
          else if( columns[i] == "num" )
          {
            row.push_back( this->db_[row_index].num.c_str() );
          }
        } // end for cols loop
      }
    } // end GetRow func

    /// \note Required interface implementation.
    virtual int GetNumRows( const Rocket::Core::String& table )
    {
      if( table == "cards" )
      {
        int idx = 0;
        for( auto itr = this->db_.begin(); itr != this->db_.end(); ++itr )
        {
          if( (*itr).id == -1 )
          {
            // Default value (not counted)
            return idx;
          }

          ++idx;
        } // end for cards loop

        return this->db_.size();
      }

      return 0;
    } // end GetNumRows func

    int size() const
    {
      return this->db_.size();
    }

    void dump()
    {
      for( auto itr = this->db_.begin(); itr != this->db_.end(); ++itr )
      {
        NOM_DUMP( (*itr).id );
        NOM_DUMP( (*itr).name );
        NOM_DUMP( (*itr).num );
      }
      NOM_DUMP( this->db_.size() );
    }

    void insert_card( int pos, const Card& card )
    {
      if( pos < this->db_.size() )
      {
        this->db_.at( pos ) = card;

        NotifyRowChange( "cards", pos, 1 );
      }
    }

    void append_card( const Card& card )
    {
      this->db_.push_back( card );

      NotifyRowAdd( "cards", this->db_.size(), 1 );
    }

    void append_cards( const std::vector<Card>& cards )
    {
      for( auto itr = cards.begin(); itr != cards.end(); ++itr )
      {
        this->db_.push_back( (*itr) );
      }

      NotifyRowAdd( "cards", this->db_.size(), cards.size() );
    }

    void load_db()
    {
      // pg 0
      this->cards_pg_.push_back(Card( 0,"Geezard",1));
      this->cards_pg_.push_back(Card( 1,"Fungar",1));
      this->cards_pg_.push_back(Card( 2,"Bite Bug",0));
      this->cards_pg_.push_back(Card( 3,"Red Bat",4));
      this->cards_pg_.push_back(Card( 4,"Blobra",2));
      this->cards_pg_.push_back(Card( 5,"Gayla",1));
      this->cards_pg_.push_back(Card( 6,"Gesper",3));
      this->cards_pg_.push_back(Card( 7,"Fastitocalon-F",0));
      this->cards_pg_.push_back(Card( 8,"Blood Soul",1));
      this->cards_pg_.push_back(Card( 9,"Caterchipillar",2));
      this->cards_pg_.push_back(Card( 10,"Cockatrice",3));

      // initial population
      this->append_cards( this->cards_pg_ );

      // pg 1
      this->cards_pg_.push_back(Card( 11,"Grat",1));
      this->cards_pg_.push_back(Card( 12,"Buel",1));
      this->cards_pg_.push_back(Card( 13,"Mesmerize",0));
      this->cards_pg_.push_back(Card( 14,"Glacial Eye",1));
      this->cards_pg_.push_back(Card( 15,"Belhelmel",1));
      this->cards_pg_.push_back(Card( 16,"Thrustaevis",1));
      this->cards_pg_.push_back(Card( 17,"Anacondaur",1));
      this->cards_pg_.push_back(Card( 18,"Creeps",0));
      this->cards_pg_.push_back(Card( 19,"Grendel",1));
      this->cards_pg_.push_back(Card( 20,"Jelleye",1));
      this->cards_pg_.push_back(Card( 21,"Grand Mantis",3));

      // pg 2
      this->cards_pg_.push_back(Card( 22,"Forbidden",1));
      this->cards_pg_.push_back(Card( 23,"Armadodo",1));
      this->cards_pg_.push_back(Card( 24,"TriFace",1));
      this->cards_pg_.push_back(Card( 25,"Fastitocalon",0));
      this->cards_pg_.push_back(Card( 26,"Snow Lion",1));
      this->cards_pg_.push_back(Card( 27,"Ochu",1));
      this->cards_pg_.push_back(Card( 28,"SAM08G",1));
      this->cards_pg_.push_back(Card( 29,"Death Claw",1));
      this->cards_pg_.push_back(Card( 30,"Cactaur",0));
      this->cards_pg_.push_back(Card( 31,"Tonberry",1));
      this->cards_pg_.push_back(Card( 32,"Abyss Worm",1));

      // pg 3
      this->cards_pg_.push_back(Card( 33,"Turtapod",1));
      this->cards_pg_.push_back(Card( 34,"Vysage",1));
      this->cards_pg_.push_back(Card( 35,"T-Rexaur",1));
      this->cards_pg_.push_back(Card( 36,"Bomb",1));
      this->cards_pg_.push_back(Card( 37,"Blitz",1));
      this->cards_pg_.push_back(Card( 38,"Wendigo",1));
      this->cards_pg_.push_back(Card( 39,"Torama",1));
      this->cards_pg_.push_back(Card( 40,"Imp",1));
      this->cards_pg_.push_back(Card( 41,"Blue Dragon",1));
      this->cards_pg_.push_back(Card( 42,"Adamantiose",1));
      this->cards_pg_.push_back(Card( 43,"Hexadragon",1));

      // pg 4
      this->cards_pg_.push_back(Card( 44,"Iron Giant",1));
      this->cards_pg_.push_back(Card( 45,"Behemoth",1));
      this->cards_pg_.push_back(Card( 46,"Chimera",1));
      this->cards_pg_.push_back(Card( 47,"PuPu",1));
      this->cards_pg_.push_back(Card( 48,"Elastoid",1));
      this->cards_pg_.push_back(Card( 49,"GIM47N",1));
      this->cards_pg_.push_back(Card( 50,"Malboro",1));
      this->cards_pg_.push_back(Card( 51,"Ruby Dragon",1));
      this->cards_pg_.push_back(Card( 52,"Elnoyle",1));
      this->cards_pg_.push_back(Card( 53,"Tonberry King",1));
      this->cards_pg_.push_back(Card( 54,"Wedge, Biggs",1));

      // Initial page
      // this->set_page(0);

      // NotifyRowChange("cards");
    }

    int id( const std::string& name ) const
    {
      for( auto itr = this->cards_pg_.begin(); itr != this->cards_pg_.end(); ++itr )
      {
        if( (*itr).name == name )
        {
          return (*itr).id;
        }
      }

      return nom::npos; // -1
    }

  private:
    std::vector<Card> db_;
  /// \fixme
  public:
    std::vector<Card> cards_pg_;
};


/// \see http://librocket.com/wiki/documentation/tutorials/Datagrid
class UIDataViewList: public nom::UIWidget
{
  public:
    /// \brief Default constructor.
    ///
    /// \see UIWidget::set_desktop, UIWidget::set_document_file,
    /// UIDataViewList::initialize.
    UIDataViewList( const Point2i& pos, const Size2i& dims ) :
      UIWidget( pos, dims ),
      data_source_( nullptr )
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

      this->_initialize();
    }

    virtual ~UIDataViewList()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

    virtual bool initialize()
    {
      this->_initialize();

      NOM_ASSERT( this->valid() != false );

      if( this->valid() == true )
      {
        this->data_source()->load_db();

        this->set_position( this->position() );

        this->set_document_size();

        this->show();

        return true;
      }

      return false;
    }

    CardsMenuModel* data_source() const
    {
      return NOM_SCAST( CardsMenuModel*, this->data_source_ );
      // return this->data_source_;
    }

    void set_data_source( CardsMenuModel* src )
    {
      this->data_source_ = src;
    }

    int size() const
    {
      return this->data_source()->size();
    }

    void dump()
    {
      this->data_source()->dump();
    }

    void insert_card( int pos, const Card& card )
    {
      this->data_source()->insert_card( pos, card );
    }

    void append_card( const Card& card )
    {
      this->data_source()->append_card( card );
    }

    void append_cards( const std::vector<Card>& cards )
    {
      this->data_source()->append_cards( cards );
    }

    void set_page( int pg )
    {
      // cards.begin() = id = per_page() * pg
      int start_id = this->per_page() * pg;

      // cards.end() = id + per_page()
      int end_id = start_id + this->per_page();

      // Sanity check
      if( start_id > this->data_source()->cards_pg_.size() ) return;

      int i = 0;
      for( auto idx = start_id; idx != end_id; ++idx )
      {
        this->data_source()->insert_card( i, this->data_source()->cards_pg_[idx] );
        ++i;
      }
    }

    // void load_db()
    // {
    //   // pg 0
    //   this->cards_pg_.push_back(Card( 0,"Geezard",1));
    //   this->cards_pg_.push_back(Card( 1,"Fungar",1));
    //   this->cards_pg_.push_back(Card( 2,"Bite Bug",0));
    //   this->cards_pg_.push_back(Card( 3,"Red Bat",4));
    //   this->cards_pg_.push_back(Card( 4,"Blobra",2));
    //   this->cards_pg_.push_back(Card( 5,"Gayla",1));
    //   this->cards_pg_.push_back(Card( 6,"Gesper",3));
    //   this->cards_pg_.push_back(Card( 7,"Fastitocalon-F",0));
    //   this->cards_pg_.push_back(Card( 8,"Blood Soul",1));
    //   this->cards_pg_.push_back(Card( 9,"Caterchipillar",2));
    //   this->cards_pg_.push_back(Card( 10,"Cockatrice",3));

    //   // initial population
    //   this->append_cards( this->cards_pg_ );

    //   // pg 1
    //   this->cards_pg_.push_back(Card( 11,"Grat",1));
    //   this->cards_pg_.push_back(Card( 12,"Buel",1));
    //   this->cards_pg_.push_back(Card( 13,"Mesmerize",0));
    //   this->cards_pg_.push_back(Card( 14,"Glacial Eye",1));
    //   this->cards_pg_.push_back(Card( 15,"Belhelmel",1));
    //   this->cards_pg_.push_back(Card( 16,"Thrustaevis",1));
    //   this->cards_pg_.push_back(Card( 17,"Anacondaur",1));
    //   this->cards_pg_.push_back(Card( 18,"Creeps",0));
    //   this->cards_pg_.push_back(Card( 19,"Grendel",1));
    //   this->cards_pg_.push_back(Card( 20,"Jelleye",1));
    //   this->cards_pg_.push_back(Card( 21,"Grand Mantis",3));

    //   // pg 2
    //   this->cards_pg_.push_back(Card( 22,"Forbidden",1));
    //   this->cards_pg_.push_back(Card( 23,"Armadodo",1));
    //   this->cards_pg_.push_back(Card( 24,"TriFace",1));
    //   this->cards_pg_.push_back(Card( 25,"Fastitocalon",0));
    //   this->cards_pg_.push_back(Card( 26,"Snow Lion",1));
    //   this->cards_pg_.push_back(Card( 27,"Ochu",1));
    //   this->cards_pg_.push_back(Card( 28,"SAM08G",1));
    //   this->cards_pg_.push_back(Card( 29,"Death Claw",1));
    //   this->cards_pg_.push_back(Card( 30,"Cactaur",0));
    //   this->cards_pg_.push_back(Card( 31,"Tonberry",1));
    //   this->cards_pg_.push_back(Card( 32,"Abyss Worm",1));

    //   // pg 3
    //   this->cards_pg_.push_back(Card( 33,"Turtapod",1));
    //   this->cards_pg_.push_back(Card( 34,"Vysage",1));
    //   this->cards_pg_.push_back(Card( 35,"T-Rexaur",1));
    //   this->cards_pg_.push_back(Card( 36,"Bomb",1));
    //   this->cards_pg_.push_back(Card( 37,"Blitz",1));
    //   this->cards_pg_.push_back(Card( 38,"Wendigo",1));
    //   this->cards_pg_.push_back(Card( 39,"Torama",1));
    //   this->cards_pg_.push_back(Card( 40,"Imp",1));
    //   this->cards_pg_.push_back(Card( 41,"Blue Dragon",1));
    //   this->cards_pg_.push_back(Card( 42,"Adamantiose",1));
    //   this->cards_pg_.push_back(Card( 43,"Hexadragon",1));

    //   // pg 4
    //   this->cards_pg_.push_back(Card( 44,"Iron Giant",1));
    //   this->cards_pg_.push_back(Card( 45,"Behemoth",1));
    //   this->cards_pg_.push_back(Card( 46,"Chimera",1));
    //   this->cards_pg_.push_back(Card( 47,"PuPu",1));
    //   this->cards_pg_.push_back(Card( 48,"Elastoid",1));
    //   this->cards_pg_.push_back(Card( 49,"GIM47N",1));
    //   this->cards_pg_.push_back(Card( 50,"Malboro",1));
    //   this->cards_pg_.push_back(Card( 51,"Ruby Dragon",1));
    //   this->cards_pg_.push_back(Card( 52,"Elnoyle",1));
    //   this->cards_pg_.push_back(Card( 53,"Tonberry King",1));
    //   this->cards_pg_.push_back(Card( 54,"Wedge, Biggs",1));

    //   // Initial page
    //   // this->set_page(0);

    //   // NotifyRowChange("cards");
    // }

    int current_page() const
    {
      return this->current_page_;
    }

    int total_pages() const
    {
      return this->total_pages_;
    }

    void set_total_pages( int num_pages )
    {
      this->total_pages_ = num_pages;
    }

    void set_current_page( int pg )
    {
      this->current_page_ = pg;
    }

    // int id( const std::string& name ) const
    // {
    //   for( auto itr = this->cards_pg_.begin(); itr != this->cards_pg_.end(); ++itr )
    //   {
    //     if( (*itr).name == name )
    //     {
    //       return (*itr).id;
    //     }
    //   }

    //   return nom::npos; // -1
    // }

    int id( const std::string& name ) const
    {
      return this->data_source()->id(name);
    }

    std::string title( Element* target, int col ) const
    {
      ElementList tags;
      NOM_ASSERT( target != nullptr );

      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, target, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );

      return tags[ col ]->GetInnerRML().CString();
    }

    void set_title( Element* target, int col, const std::string& name )
    {
      ElementList tags;
      NOM_ASSERT( target != nullptr );

      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, target, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );

      tags[ col ]->SetInnerRML( name.c_str() );
    }

    int per_page() const
    {
      return this->per_page_;
    }

    // current_page_ + 1 * id(selection_) - 1 = 5*11-1
    int selection() const
    {
      return this->selection_;
    }

    void set_selection( int idx )
    {
      this->selection_ = idx;
    }

  private:
    /// \brief Internal state initialization.
    void _initialize()
    {
      this->per_page_ = 11;
      // this->total_pages = db.size() / per_page();
      // this->total_pages = 10;
      this->set_total_pages( 5 );
      this->set_current_page( 0 );
      this->set_selection( 0 );
    }

    /// \remarks This object pointer is not owned by this class and should not
    /// be freed.
    Rocket::Controls::DataSource* data_source_;
    // DataSourceModel* data_source_;
    // CardsMenuModel* data_source_;

    /// \todo Remove
    // std::vector<Card> db_;
    /// \todo Remove
    // std::vector<Card> cards_pg_;

    /// \brief The total number of available card pages.
    int total_pages_;

    /// \brief The number of cards shown per page.
    int per_page_;

    /// \brief The currently selected page.
    int current_page_;

    /// \brief The currently selected card.
    ///
    /// \remarks This maps internally to the element's index position in
    /// the active cards database.
    int selection_;
};

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

      if( nom::RocketSDL2RenderInterface::gl_init( this->window_.size().w, this->window_.size().h ) == false )
      {
        // FAIL() << "Could not initialize OpenGL for libRocket.";
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

    // CardFormatter card_formatter;
};

void on_keydown( Rocket::Core::Event& ev, UIDataViewList* store )
{
  ASSERT_TRUE( store != nullptr );

  int pg = store->current_page();
  int num_pages = store->total_pages();

  int per_page = store->per_page();
  int selected_card = store->selection();

  Element* target = ev.GetTargetElement();

  if( target == nullptr )
  {
    FAIL()
    << "Event element target was NULL; this shouldn't have been so!";
  }

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

    store->set_page(pg);
    store->set_current_page(pg);
    store->set_selection(selected_card);

    // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "KEYDOWN event from", target->GetId().CString(), "value:", key );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Pg:", store->current_page() );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Selection:", store->selection() );
  } // end if keydown

  ASSERT_TRUE( store->document() != nullptr );
  store->set_title( store->document()->GetElementById("content"), 0, "CARDS P. " + std::to_string(pg+1) );

  EXPECT_EQ( pg, store->current_page() );
  EXPECT_EQ( 11, store->data_source()->GetNumRows("cards") );
} // end func on_keydown

void on_mouseup( Rocket::Core::Event& ev, UIDataViewList* store  )
{
  ASSERT_TRUE( store != nullptr );

  int selection = 0;
  Rocket::Core::Element* target = ev.GetTargetElement();

  if( target == nullptr )
  {
    FAIL()
    << "Event element target was NULL; this shouldn't have been so!";
  }

  if( ev == "mouseup" )
  {
    if( target->GetTagName() == "datagridcell" )
    {
      selection = store->id( target->GetInnerRML().CString() );
      store->set_selection( selection );

      NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card name:", target->GetInnerRML().CString() );
      NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Card ID:", store->selection() );

      EXPECT_EQ( selection, store->selection() );
      EXPECT_EQ( 11, store->data_source()->GetNumRows("cards") );

      // datagridrow
      // NOM_DUMP( target->GetParentNode()->GetTagName().CString() );
      // <datagridcell>Geezard</datagridcell><datagridcell>1</datagridcell>

      // 1
      // NOM_DUMP( target->GetParentNode()->GetLastChild()->GetInnerRML().CString() );

      // StringList out;
      // StringList cols;
      // cols.push_back( "name" );
      // cols.push_back( "num" );
      // store.GetRow( out, "cards", 0, cols );  // Geezard
      // for( auto itr = out.begin(); itr != out.end(); ++itr )
      // {
      //   NOM_DUMP( (*itr).CString() );
      // }

    } // end if target

  } // end if click
} // end func on_mouseup

void on_mousescroll( Rocket::Core::Event& ev, UIDataViewList* store )
{
  ASSERT_TRUE( store != nullptr );

  // int pg = store.current_page();
  // int num_pages = store.total_pages();
  int per_page = store->per_page();
  int selected_card = store->selection();

  Element* target = ev.GetTargetElement();

  if( target == nullptr )
  {
    FAIL()
    << "Event element target was NULL; this shouldn't have been so!";
  }

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

    // store.set_page(pg);
    // store.set_current_page(pg);
    store->set_selection(selected_card);

    // NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "WHEEL event from", target->GetId().CString(), "value:", wheel );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Pg:", store->current_page() );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Selection:", store->selection() );

  } // end if mousescroll
} // end func on_mousescroll

/// \todo Clean test up
// TEST_F( libRocketDataGridTest, BaseIntegration )
// {
//   // As per the positioning units used for nom::DataViewListTest (ex2) in
//   // tests/src/gui/DataViewListTest.cpp.
//   Point2i pos( 25, 25 );
//   std::string doc_file = "dataview.rml";

//   Rocket::Core::ElementDocument* doc = load_window( this->desktop.context(), doc_file, pos );

//   if( doc )
//   {
//     EXPECT_STREQ( "CARDS.", doc->GetTitle().CString() )
//     << "Document title should be the text of the title element: 'CARDS.'";
//   }
//   else
//   {
//     FAIL()
//     << "Document should not be NULL";
//   }

//   // store.dump();

//   // std::vector<Card> c;
//   // c.push_back( Card( 11, "Geezard", 999 ) );
//   // store.append_cards( c );

//   // store.append_card( Card( 12, "testme", 22 ) );
//   // // store.dump();

//   // store.insert_card( 11, Card( 12, "Geezard", 99 ) );
//   // store.dump();

//   EXPECT_EQ( 0, this->store.current_page() );
//   this->store.set_title( doc->GetElementById("content"), 0, "CARDS P. " + std::to_string(this->store.current_page() + 1) );

//   EXPECT_EQ( "CARDS P. 1", this->store.title(doc->GetElementById("content"), 0) );
//   EXPECT_EQ( "NUM.", this->store.title(doc->GetElementById("content"), 1) );

//   // evt.register_event_listener( doc, "click", new nom::UIEventListener( [&] ( Rocket::Core::Event& ev )
//   //   {
//   //     Rocket::Core::Element* event = ev.GetTargetElement();
//   //     if( ev == "click" && event && event->GetClassNames().Find("card") == 0 )
//   //     {
//           // Mouse coordinates
//           // Input::KeyIdentifier mouse_x = NOM_SCAST(Input::KeyIdentifier, ev.GetParameter<int>("mouse_x", -1) );
//           // Input::KeyIdentifier mouse_y = NOM_SCAST(Input::KeyIdentifier, ev.GetParameter<int>("mouse_y", -1) );
//           // NOM_DUMP(mouse_x);
//           // NOM_DUMP(mouse_y);

//   //       // datagridcell
//   //       NOM_DUMP( event->GetParentNode()->GetTagName().CString() );
//   //     }
//   //   }
//   // ));

//   EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
//   EXPECT_TRUE( this->compare() );
// /*
//   Rocket::Core::ElementList tags[6];
//   // Rocket::Core::Element* tag = nullptr;
//   Rocket::Core::Element* target = doc->GetElementById("content");
//   // Rocket::Core::Element* target = doc->GetElementById("content")->GetElementById("name");
//   NOM_ASSERT( target != nullptr );

//   // NOM_DUMP( target->GetTagName().CString() );

//   Rocket::Core::ElementUtilities::GetElementsByTagName( tags[0], target, "datagrid" );

//   NOM_DUMP(tags[0].size() );

//   for( auto itr = tags[0].begin(); itr != tags[0].end(); ++itr )
//   {
//     // if( (*itr)->GetTagName() )
//     {
//       // datagrid
//       NOM_DUMP( (*itr)->GetTagName().CString() );
//       // 2
//       NOM_DUMP( (*itr)->GetNumChildren() );

//       // datagridheader
//       NOM_DUMP( (*itr)->GetFirstChild()->GetTagName().CString() );
//       // 2
//       NOM_DUMP( (*itr)->GetFirstChild()->GetNumChildren() );
//       // datagridcolumn
//       NOM_DUMP( (*itr)->GetFirstChild()->GetFirstChild()->GetTagName().CString() );
//       // #text
//       NOM_DUMP( (*itr)->GetFirstChild()->GetFirstChild()->GetInnerRML().CString() );

//       // datagridbody
//       NOM_DUMP( (*itr)->GetLastChild()->GetTagName().CString() );

//       // if( (*itr)->GetLastChild()->GetTagName() == "datagridbody" )
//       // {
//       //   (*itr)->GetLastChild()->SetId("datagridbody-cards");
//       //   NOM_DUMP( (*itr)->GetLastChild()->GetId().CString() );
//       //   NOM_DUMP( (*itr)->GetLastChild()->GetTagName().CString() );
//       // }

//       // 0 (WTF?) ... should have 'datagridrow' children
//       NOM_DUMP( (*itr)->GetLastChild()->GetNumChildren() );
//     }
//   } // end for tags loop
// */
// }

TEST_F( libRocketDataGridTest, UIDataViewList )
{
  // As per the positioning units used for nom::DataViewListTest (ex2) in
  // tests/src/gui/DataViewListTest.cpp.
  Point2i pos1( 25, 25 );
  std::string doc_file1 = "dataview.rml";

  UIDataViewList store1( pos1, Size2i::null );
  EXPECT_EQ( true, store1.set_desktop( this->desktop.context() ) );
  EXPECT_EQ( true, store1.set_document_file( doc_file1 ) );
  store1.set_data_source( new CardsMenuModel("cards_db") );

  if( store1.initialize() == false )
  {
    FAIL()
    << this->test_set() << " object should not be invalid; is the context and document file valid?";
  }

  store1.set_title( store1.document()->GetElementById("content"), 0, "CARDS P. " + std::to_string(store1.current_page() + 1) );

  EXPECT_EQ( true, store1.visible() );

  EXPECT_EQ( pos1, store1.position() );

  // Default values
  EXPECT_EQ( 0, store1.selection() );
  EXPECT_EQ( 11, store1.per_page() );
  EXPECT_EQ( 0, store1.current_page() );

  EXPECT_EQ( "CARDS P. 1", store1.title( store1.document()->GetElementById("content"), 0) );
  EXPECT_EQ( "NUM.", store1.title( store1.document()->GetElementById("content"), 1) );

  store1.register_event_listener( store1.document(), "keydown", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_keydown( ev, &store1 ); }
  ));

  store1.register_event_listener( store1.document(), "mouseup", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_mouseup( ev, &store1 ); }
  ));

  store1.register_event_listener( store1.document(), "mousescroll", new nom::UIEventListener( [&]
    ( Rocket::Core::Event& ev ) { on_mousescroll( ev, &store1 ); }
  ));

  // store1.dump();

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

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
