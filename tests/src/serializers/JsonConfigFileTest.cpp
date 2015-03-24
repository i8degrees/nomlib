#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>
#include <nomlib/system/init.hpp>
#include <nomlib/serializers.hpp>

namespace nom {

class JsonConfigFileTest: public ::testing::Test
{
  public:
    JsonConfigFileTest( void ) :
      cfg{ new JsonConfigFile() }
    {
      // TTcards config copied to build/tests/Debug.
      cfg->set_filename( "Resources/json/config.json" );
    }

    ~JsonConfigFileTest( void )
    {
    }

  protected:
    IConfigFile::unique_ptr cfg;
};

/// \brief Load an existing configuration file and write two new properties to
/// that same file. This is the expected API usage of the class.
TEST_F( JsonConfigFileTest, LoadExistingConfigFile )
{
  // Automatically done for us
  // ASSERT_TRUE( cfg->load() );

  EXPECT_EQ( "icon.png", cfg->get_string( "APP_ICON", "" ) );
  EXPECT_EQ( "images/board.png", cfg->get_string( "BOARD_BACKGROUND", "" ) );
  EXPECT_EQ( "images/game-over_background.png", cfg->get_string( "GAMEOVER_BACKGROUND", "" ) );
  EXPECT_EQ( "cards.json", cfg->get_string( "CARDS_DB", "" ) );

  cfg->set_property( "WINDOW_WIDTH", 384 );
  cfg->set_property( "WINDOW_HEIGHT", 224 );
  cfg->set_property( "SCALE_FACTOR", 2 );

  ASSERT_TRUE( cfg->erase( "SCALE_FACTOR" ) );

  EXPECT_EQ( 384, cfg->get_int( "WINDOW_WIDTH", 384 ) );
  EXPECT_EQ( 224, cfg->get_int( "WINDOW_HEIGHT", 224 ) );

  // Should never be found (erased).
  EXPECT_EQ( -1, cfg->get_int( "SCALE_FACTOR", -1 ) );
}

/// \brief Load a new configuration file and write two new properties to the
/// file.
TEST_F( JsonConfigFileTest, LoadNewConfigFile )
{
  cfg->set_filename( "config_new.json" );
  EXPECT_EQ( "config_new.json", cfg->filename() );

  cfg->set_property( "APP_ICON", "icon.png" );
  cfg->set_property( "WINDOW_WIDTH", 384 );
  cfg->set_property( "WINDOW_HEIGHT", 224 );
  cfg->set_property( "SCALE_FACTOR", 2 );

  ASSERT_TRUE( cfg->erase( "SCALE_FACTOR" ) );

  EXPECT_EQ( "icon.png", cfg->get_string( "APP_ICON", "" ) );
  EXPECT_EQ( 384, cfg->get_int( "WINDOW_WIDTH", 384 ) );
  EXPECT_EQ( 224, cfg->get_int( "WINDOW_HEIGHT", 224 ) );

  // Should never be found (erased).
  EXPECT_EQ( -1, cfg->get_int( "SCALE_FACTOR", -1 ) );
}

/// \brief Save as new file support testing; the file should be output as
/// config2.json to the working directory of this executable.
TEST_F( JsonConfigFileTest, SaveLoadedConfigFile )
{
  // Output new file to working dir
  cfg->set_filename( "config2.json" );

  cfg->set_property( "WINDOW_WIDTH", 384 );

  ASSERT_TRUE( cfg->save() );
}

/// \brief Automatic save support testing; the file should be output as
/// config3.json upon destruction of cfg.
TEST_F( JsonConfigFileTest, SaveConfigFileOnDestruct )
{
  cfg->set_filename( "config3.json" ); // output to working directory
  cfg->set_property( "WINDOW_WIDTH", 384 );
  cfg->set_property( "WINDOW_HEIGHT", 224 );
  cfg->set_property( "APP_ICON", "icon.png" );

  EXPECT_EQ( 384, cfg->get_int( "WINDOW_WIDTH", -1 ) );
  EXPECT_EQ( 224, cfg->get_int( "WINDOW_HEIGHT", -1 ) );
  EXPECT_EQ( "icon.png", cfg->get_string( "APP_ICON", "" ) );
}

/// \brief Type safety tests
TEST_F( JsonConfigFileTest, IConfigFileSerializerType )
{
  JsonCppDeserializer* fp_loader = NOM_DYN_PTR_CAST( JsonCppDeserializer*, cfg->deserializer() );
  JsonCppSerializer* fp_saver = NOM_DYN_PTR_CAST( JsonCppSerializer*, cfg->serializer() );

  ASSERT_TRUE( fp_loader != nullptr );
  ASSERT_TRUE( fp_saver != nullptr );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false ) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  return RUN_ALL_TESTS();
}
