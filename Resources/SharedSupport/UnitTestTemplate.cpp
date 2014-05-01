#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/config.hpp>

namespace nom {

class Template: public ::testing::Test
{
  public:
    Template( void )
    {
      // ...
    }

    ~Template( void )
    {
      // ...
    }

  protected:
    // Member methods & variables
};

TEST_F( Template, TemplateTestDesc )
{
  EXPECT_EQ( -1, -1 );
  ASSERT_TRUE( true );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
