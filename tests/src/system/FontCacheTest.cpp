#include "gtest/gtest.h"

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

namespace nom {

class FontCacheTest: public ::testing::Test
{
  public:
    FontCacheTest( void )
    {
      // Enable function call tracing of engine initialization
      nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE_SYSTEM, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

      // Enable initialization status logging
      nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_SYSTEM, nom::LogPriority::NOM_LOG_PRIORITY_INFO );
    }

    ~FontCacheTest( void )
    {
      // ...
    }

  protected:
    // ...
};

/// \brief Non-static usage of fonts resource cache
TEST_F( FontCacheTest, CoreAPI )
{
  ResourceCache<Font> fonts_;
  File fp;
  Path p;

  // Internally, nom::PlatformSettings::enumerate_fonts does very much the same
  // thing as shown below (the appending of resources from known file paths):
  p = fp.resource_path( "org.i8degrees.nomlib" ) + p.native() + "fonts";

  #if defined( NOM_PLATFORM_OSX )
    Path sys( "/System/Library/Fonts" );
    Path lib( "/Library/Fonts" );
    ASSERT_TRUE( fonts_.append_resource( ResourceFile( "LucidaGrande", sys.prepend("LucidaGrande.ttc"), ResourceFile::Type::TrueTypeFont ) ) );
  #elif defined( NOM_PLATFORM_WINDOWS )
    Path sys( "C:\\Windows\\Fonts" );
    ASSERT_TRUE( fonts_.append_resource( ResourceFile( "Arial", sys.prepend("Arial.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  #endif

  ASSERT_TRUE( fonts_.append_resource( ResourceFile( "LiberationSans-Regular", p.prepend("LiberationSans-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  ASSERT_TRUE( fonts_.append_resource( ResourceFile( "LiberationSerif-Regular", p.prepend("LiberationSerif-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  ASSERT_TRUE( fonts_.append_resource( ResourceFile( "VIII", p.prepend("VIII.png"), ResourceFile::Type::BitmapFont ) ) );
  ASSERT_TRUE( fonts_.append_resource( ResourceFile( "VIII_small", p.prepend("VIII_small.png"), ResourceFile::Type::BitmapFont ) ) );

  // Should not exist
  ASSERT_FALSE( fonts_.append_resource( ResourceFile( "IX", p.prepend("IX.png") ) ) );

  // Should already exist.
  ASSERT_FALSE( fonts_.append_resource( ResourceFile( "VIII", p.prepend("VIII.png") ) ) );

  ResourceFile res;

  ASSERT_TRUE( res == ResourceFile::null );
  ASSERT_FALSE( res.exists() );

  res = fonts_.find_resource( "VIII" );
  ASSERT_TRUE( res.exists() );
  EXPECT_EQ( "VIII", res.name() );

  res = fonts_.find_resource( "VIII_small" );
  ASSERT_TRUE( res.exists() );
  EXPECT_EQ( "VIII_small", res.name() );

  // Should not exist
  res = fonts_.find_resource( "IX" );
  ASSERT_FALSE( res.exists() );
  EXPECT_EQ( "", res.name() );

  // NOTE: WindowsOS-specific error "unknown file : error : SEH exception with
  // code 0xc0000005 thrown in the test body." occurs here if the font cache
  // size does not match the expected value precisely.
  //
  // References:
  //
  // 1. https://www.assembla.com/spaces/OpenSurgSim/tickets/13#/activity/ticket:
  // 2. http://msdn.microsoft.com/library/vstudio/swezty51
  // 3. See also: tests/CMakeLists.txt FIXME note regarding err when using 'test'
  // target under Windows from the command line.
  EXPECT_EQ( 5, fonts_.size() );

  fonts_.clear();

  EXPECT_EQ( 0, fonts_.size() );
}

/// \brief Global (static) usage of fonts resource cache via the
/// nom::SystemFonts interface.
TEST_F( FontCacheTest, GlobalSystemFontsInterface )
{
  Path p;
  File fp;
  nom::RenderWindow window;
  nom::FontCache cache;

  // We first need to ensure that the SDL2_ttf extension is initialized,
  // otherwise we will receive err messages upon trying to load TrueType fonts.
  // nom::init_third_party( 0 );

  // (This should be done for us automatically):
  // nom::SystemFonts::initialize();

  p = fp.resource_path( "org.i8degrees.nomlib" ) + p.native() + "fonts";

  // Necessary for loading font resources
  ASSERT_TRUE( window.create( "FontCacheTest::GlobalSystemFontsInterface", 0, 0, SDL_WINDOW_HIDDEN ) == true )
  << "Could not create nom::RenderWindow object for loading font resources from";

  cache = nom::SystemFonts::cache();

  // Note that without the nom::PlatformSettings interface, we must add the font
  // resources before the cache can be used:
  cache.set_resource_handler( [&] ( const ResourceFile& res, Font& font ) { nom::create_font( res, font ); } );

  // Add two (2) bitmap fonts to the cache for testing use:
  ASSERT_TRUE( cache.append_resource( ResourceFile( "VIII", p.prepend("VIII.png"), ResourceFile::Type::BitmapFont ) ) )
  << "Could not insert BitmapFont resource VIII";
  ASSERT_TRUE( cache.append_resource( ResourceFile( "VIII_small", p.prepend("VIII_small.png"), ResourceFile::Type::BitmapFont ) ) )
  << "Could not insert BitmapFont resource VIII_small";

  // Add two (2) TrueType fonts to the cache for testing use:
  ASSERT_TRUE( cache.append_resource( ResourceFile( "LiberationSans", p.prepend("LiberationSans-Regular.ttf"), ResourceFile::Type::TrueTypeFont )  ) )
  << "Could not insert TrueType resource LiberationSans";
  ASSERT_TRUE( cache.append_resource( ResourceFile( "LiberationSerif", p.prepend("LiberationSerif-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) ) )
  << "Could not insert TrueType resource LiberationSerif";

  // Bitmap font tests:
  nom::Font bfont1 = *cache.load_resource( "VIII" );
  nom::Font bfont2 = *cache.load_resource( "VIII_small" );
  nom::Font bfont3 = *cache.load_resource( "VIII" );

  ASSERT_TRUE( bfont1.valid() == true )
  << "Font resource 1 should be valid";

  ASSERT_TRUE( bfont2.valid() == true )
  << "Font resource 2 should be valid";

  ASSERT_TRUE( bfont3.valid() == true )
  << "Font resource 3 should be valid";

  // NOTE: BitmapFont is never set sharable, as an optimization I chose to try,
  // arising out of the fact that we do not (yet!) have any variables that
  // ought to stay unique when passing around to multiple objects.
  //
  // See also: nom::create_font -- src/system/resource_handlers.cpp; this is
  // where the actual "sharing" logic is implemented.
  ASSERT_TRUE( bfont1 == bfont3 )
  << "Font resource 1 should be the same as font resource 3";

  // Ensure that nom::Font's copy-on-write functionality is working as
  // intended; the two resource instances should never be unique (cloned
  // copies).
  //
  // See above note on why; this is simply another way of testing the
  // above assertion.
  nom::Text label1;
  nom::Text label2;
  label1.set_font( bfont1 );
  label2.set_font( bfont3 );
  ASSERT_TRUE( label1.font() == label2.font() )
  << "Label 1 should be the same as label 2";

  // TrueType font tests:
  nom::Font bfont4 = *cache.load_resource( "LiberationSans" );
  nom::Font bfont5 = *cache.load_resource( "LiberationSans" );

  ASSERT_TRUE( bfont4.valid() == true )
  << "Font resource 4 should be valid";

  ASSERT_TRUE( bfont5.valid() == true )
  << "Font resource 5 should be valid";

  // Ensure that nom::Font's copy-on-write functionality is working as
  // intended; the two resource instances should not *yet* be unique (cloned
  // copies):
  ASSERT_TRUE( bfont4 == bfont5 )
  << "Font resource 4 should be the same as font resource 5";

  // Ensure that nom::Font's copy-on-write functionality is working as
  // intended; the two resource instances should always be unique (cloned
  // copies):
  nom::Text label3;
  nom::Text label4;
  label3.set_font( bfont4 );
  label4.set_font( bfont5 );
  ASSERT_FALSE( label3.font() == label4.font() )
  << "Label 3 should NOT be the same as label 4";
}

/// \brief Global (static) usage of fonts resource cache via the
/// nom::PlatformSettings interface.
TEST_F( FontCacheTest, GlobalPlatformSettingsInterface )
{
  Path p;
  File fp;
  nom::RenderWindow window;

  // We first need to ensure that the SDL2_ttf extension is initialized,
  // otherwise we will receive err messages upon trying to load TrueType fonts.
  // nom::init_third_party( 0 );

  // (This should be done for us automatically):
  // nom::PlatformSettings::initialize();

  // Necessary for loading font resources
  ASSERT_TRUE( window.create( "FontCacheTest::GlobalPlatformSettingsInterface", 0, 0, SDL_WINDOW_HIDDEN ) == true );

  // Note that we do *not* own these pointers!
  nom::Font* font1 = PlatformSettings::get_system_font( SystemFontType::VariableTrueType );
  nom::Font* font2 = PlatformSettings::get_system_font( SystemFontType::FixedTrueType );
  nom::Font* font3 = PlatformSettings::get_system_font( SystemFontType::FixedTrueType );

  ASSERT_FALSE( font1 == nullptr );
  ASSERT_FALSE( font2 == nullptr );
  ASSERT_FALSE( font3 == nullptr );

  // The variable TrueType font chosen for the platform should never be the same
  // as the fixed TrueType font.
  ASSERT_FALSE( font1 == font2 );

  // Ensure that nom::Font's copy-on-write functionality works; the two
  // resource instances should always be unique (cloned copies), except when
  // Font::sharable is explicitly set to false.
  ASSERT_FALSE( font1 == font3 );

  // TODO: Unit tests for nom::Font's sharable implementation -- copy-on-write
  // with reference counting. See also: FontCacheTest::GlobalSystemFontsInterface.
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
