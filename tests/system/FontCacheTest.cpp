#include "gtest/gtest.h"

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

namespace nom {

class FontCacheTest: public ::testing::Test
{
  public:
    FontCacheTest( void )
    {
      // ...
    }

    ~FontCacheTest( void )
    {
      // ...
    }

  protected:
    ResourceCache<Font> fonts_;
};

/// \brief Non-static usage of fonts resource cache
TEST_F( FontCacheTest, CoreAPI )
{
  File fp;
  Path p;

  p = fp.resource_path( "org.i8degrees.nomlib" ) + p.native() + "fonts";

  #if defined( NOM_PLATFORM_OSX )
    Path sys( "/System/Library/Fonts" );
    Path lib( "/Library/Fonts" );
    ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "LucidaGrande", sys.prepend("LucidaGrande.ttc"), ResourceFile::Type::TrueTypeFont ) ) );
  #elif defined( NOM_PLATFORM_WINDOWS )
    Path sys( "C:\\Windows\\Fonts" );
    ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "Arial", sys.prepend("Arial.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  #endif

  ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "LiberationSans-Regular", p.prepend("LiberationSans-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "LiberationSerif-Regular", p.prepend("LiberationSerif-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
  ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "VIII", p.prepend("VIII.png"), ResourceFile::Type::BitmapFont ) ) );
  ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "VIII_small", p.prepend("VIII_small.png"), ResourceFile::Type::BitmapFont ) ) );

  // Should not exist
  ASSERT_FALSE( this->fonts_.append_resource( ResourceFile( "IX", p.prepend("IX.png") ) ) );

  // Should already exist.
  ASSERT_FALSE( this->fonts_.append_resource( ResourceFile( "VIII", p.prepend("VIII.png") ) ) );

  ResourceFile res;

  ASSERT_TRUE( res == ResourceFile::null );
  ASSERT_FALSE( res.exists() );

  res = this->fonts_.find_resource( "VIII" );
  ASSERT_TRUE( res.exists() );
  EXPECT_EQ( "VIII", res.name() );

  res = this->fonts_.find_resource( "VIII_small" );
  ASSERT_TRUE( res.exists() );
  EXPECT_EQ( "VIII_small", res.name() );

  // Should not exist
  res = this->fonts_.find_resource( "IX" );
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
  EXPECT_EQ( 5, this->fonts_.size() );

  this->fonts_.clear();

  EXPECT_EQ( 0, this->fonts_.size() );
}

/// \brief Global (static) usage of fonts resource cache
TEST_F( FontCacheTest, StaticInterfaceAPI )
{
  nom::uint32 window_flags = SDL_WINDOW_HIDDEN;
  nom::RenderWindow window;

  nom::SystemFonts::initialize();
  nom::PlatformSettings::initialize();

  // Necessary for loading font resources
  if( window.create( "FontCacheTest::StaticInterfaceAPI", 0, 0, window_flags ) == false )
  {
    FAIL();
  }

  // FIXME:
  // nom::Font* font1 = PlatformSettings::get_system_font( SystemFontType::VariableTrueType );
  // nom::Font* font2 = PlatformSettings::get_system_font( SystemFontType::FixedTrueType );

  // ASSERT_FALSE( font1 == nullptr );
  // ASSERT_FALSE( font2 == nullptr );

  // ASSERT_TRUE( font1 != font2 );

  nom::FontCache cache = nom::SystemFonts::cache();

  nom::Font bfont = *cache.load_resource( "VIII" );

  ASSERT_TRUE( bfont.valid() == true );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
