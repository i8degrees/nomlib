#include "serializers/common.hpp"

// Private headers
#include "gtest/gtest.h"

namespace nom {

Value sanity2_out( void )
{
  Value o;

  o["object1"]["response1"] = "Yeah buddy!";
  o["object1"]["boolean1"] = false;
  o["object1"]["null"] = Value::null;

  o["object2"]["response2"] = "Light weight!";
  o["object2"]["boolean2"] = true;
  o["object2"]["array2"][0] = 0;
  o["object2"]["array2"][1] = 2;
  o["object2"]["array2"][2] = 3;
  o["object2"]["array2"][3] = "hax";
  o["object2"]["deep_object"]["h"]["boolean"] = false;
  o["object2"]["deep_object"]["h"]["deep_deep_deep_object"]["deep_deep_deep_array"][0] = "string_1";
  o["object2"]["deep_object"]["h"]["deep_deep_deep_object"]["deep_deep_deep_array"][1] = "string_2";
  o["object2"]["deep_object"]["h"]["deep_deep_deep_object"]["h"] = 240;
  o["object2"]["deep_object"]["h"]["deep_deep_deep_object"]["w"] = 420;
  o["object2"]["deep_object"]["h"]["key"] = "pair";
  o["object2"]["deep_object"]["h"]["null"] = Value::null;
  o["object2"]["deep_object"]["w"] = -4;

  o["object3"][0]["cmd"][0] = "hax";
  o["object3"][0]["cmd"][1] = "gibson";
  o["object3"][0]["deep_object"]["h"]["boolean"] = false;
  o["object3"][0]["deep_object"]["h"]["deep_deep_deep_object"]["deep_deep_deep_array"][0] = "string_1";
  o["object3"][0]["deep_object"]["h"]["deep_deep_deep_object"]["deep_deep_deep_array"][1] = "string_2";
  o["object3"][0]["deep_object"]["h"]["deep_deep_deep_object"]["h"] = 240;
  o["object3"][0]["deep_object"]["h"]["deep_deep_deep_object"]["w"] = 420;
  o["object3"][0]["deep_object"]["h"]["key"] = "pair";
  o["object3"][0]["deep_object"]["h"]["null"] = Value::null;
  o["object3"][0]["deep_object"]["w"] = -4;

  o["object5"][0] = "hello, there!";
  o["object5"][1] = -1;
  o["object5"][2] = false;

  return o;
}

// The idea with these (un-used, but tested) methods is to centralize as much of
// the unit testing code as we can, so that we can greatly simplify testing,
// but we ought to probably wait until we catch back up on the XML
// processing side of the project before we commit to moving all of the
// redundant code over, just in case! (For starters, XML has no concept of data
// types, unlike JSON -- everything is a string!)

void CheckObjectValues_NullValue( /*const*/ Value& o )
{
  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( true, o["obj"]["key"].null_type() );
}

void CheckObjectValues_IntegerValue( /*const*/ Value& o )
{
  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( -1, o["obj"]["key"].get_int() );
}

} // namespace nom
