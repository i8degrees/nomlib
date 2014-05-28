#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

namespace nom {

/// \note These unit tests target the non-typical usage of the FontCache class --
/// the non-static variant.
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
    Path p;
    File fp;

    // ResourceCache<IFont::shared_ptr> fonts_;
    ResourceCache<Font> fonts_;
};

/// \fixme This test is failing on WindowsOS (Unknown file error: SEH exception).
TEST_F( FontCacheTest, CoreAPI )
{
  p = Path( fp.resource_path( "org.i8degrees.nomlib" ) + p.native() + "fonts" );

  #if defined( NOM_PLATFORM_OSX )
    Path sys( "/System/Library/Fonts" );
    Path lib( "/Library/Fonts" );
    ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "LucidaGrande", sys.prepend("LucidaGrande.ttc"), ResourceFile::Type::TrueTypeFont ) ) );
  #elif defined( NOM_PLATFORM_WINDOWS )
    Path sys( "C:\\Windows\\Fonts" );
    ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "Arial", sys.prepend("Arial.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
    ASSERT_TRUE( this->fonts_.append_resource( ResourceFile( "TimesNewRoman", sys.prepend("times.ttf"), ResourceFile::Type::TrueTypeFont ) ) );
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

  EXPECT_EQ( 5, this->fonts_.size() );

  this->fonts_.clear();

  EXPECT_EQ( 0, this->fonts_.size() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
