/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "gtest/gtest.h"

#include "nomlib/config.hpp"
#include "nomlib/core.hpp"

using namespace nom;

TEST(StringTest, AllocateNullCString)
{
  const char* ret = nullptr;

  ret = nom::create_string(nullptr, 1);
  if(ret) {
    ret = "f";
    NOM_DUMP(ret);
  }

  ret = nom::create_string(nullptr);
  if(ret) {
    ret = "f";
    NOM_DUMP(ret);
  }
}

TEST(StringTest, CompareSensitiveCString)
{
  int ret = 0;

  ret = nom::compare_cstr_sensitive("opengl", "opengl");
  EXPECT_EQ(0, ret);

  ret = nom::compare_cstr_sensitive("opengl", "Opengl");
  EXPECT_NE(0, ret);

  ret = nom::compare_cstr_sensitive("opengl", "Opengl", 2);
  EXPECT_NE(0, ret);

  ret = nom::compare_cstr_sensitive("opengl", "opengl", 2);
  EXPECT_EQ(0, ret);
}

TEST(StringTest, CompareSensitiveString)
{
  int ret = 0;

  ret = nom::compare_string_sensitive("opengl", "Opengl");
  EXPECT_NE(0, ret);

  ret = nom::compare_string_sensitive("opengl", "Opengl", 1);
  EXPECT_NE(0, ret);
}

TEST(StringTest, CompareInsensitiveCString)
{
  int ret = 0;

  ret = nom::compare_cstr_insensitive("opengl", "openGL");
  EXPECT_EQ(0, ret);

  ret = nom::compare_cstr_insensitive("opengl", "openGL", 5);
  EXPECT_EQ(0, ret);

  ret = nom::compare_cstr_insensitive("ALC", "ALC_testme", 2);
  EXPECT_EQ(0, ret);
}

TEST(StringTest, CompareInsensitiveString)
{
  int ret = 0;

  ret = nom::compare_string_insensitive("opengl", "openGL", 5);
  EXPECT_EQ(0, ret);

  ret = nom::compare_string_insensitive("opengl", "openGL");
  EXPECT_EQ(0, ret);

  ret = nom::compare_string_insensitive("ALC", "ALC_testme", 3);
  EXPECT_EQ(0, ret);
}

TEST(StringTest, NullCStringToSignedInteger)
{
  int ret = 0;

  ret = nom::string_to_int(nullptr, 10);
  NOM_DUMP(ret);
  EXPECT_EQ(0, ret);

  ret = nom::string_to_int(nullptr);
  NOM_DUMP(ret);
  EXPECT_EQ(0, ret);
}

TEST(StringTest, NullCStringToUnsignedInteger)
{
  uint ret = 0;

  ret = nom::string_to_uint(nullptr, 10);
  NOM_DUMP(ret);
  EXPECT_EQ(0, ret);

  ret = nom::string_to_uint(nullptr);
  NOM_DUMP(ret);
  EXPECT_EQ(0, ret);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  // NOTE(jeff): There is no need to initialize nomlib -- nom::init -- before
  // using the string functions.

  return RUN_ALL_TESTS();
}
