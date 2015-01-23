/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include <string>

#include <gtest/gtest.h>

#include <nomlib/config.hpp>
#include <nomlib/core.hpp>
#include <nomlib/system.hpp>

using namespace nom;

class TimerTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    TimerTest()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

      // Disable verbose, debug output
      nom::SDL2Logger::set_logging_priority(  NOM_LOG_CATEGORY_TEST,
                                              NOM_LOG_PRIORITY_WARN );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~TimerTest()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
    }

  protected:
    HighResolutionTimer duration_timer;
    HighResolutionTimer timer;
    uint64 t = 0;
};

TEST_F(TimerTest, ConversionToMilliseconds)
{
  const uint32 DURATION = 2000;

  timer.start();
  duration_timer.start();
  while( HighResolutionTimer::to_milliseconds(t) < DURATION ) {
    t = duration_timer.ticks();
  }

  uint64 ret = HighResolutionTimer::to_milliseconds(t);
  EXPECT_GE(ret, DURATION);
}

TEST_F(TimerTest, ConversionToSeconds)
{
  const uint32 DURATION = 2;

  timer.start();
  duration_timer.start();
  while( HighResolutionTimer::to_seconds(t) < DURATION ) {
    t = duration_timer.ticks();
  }

  uint64 ret = HighResolutionTimer::to_seconds(t);
  EXPECT_GE(ret, DURATION);
}

TEST_F(TimerTest, HighResolutionTimer)
{
  const uint32 DURATION = 2000;

  timer.start();
  duration_timer.start();
  while( HighResolutionTimer::to_milliseconds(t) < DURATION ) {
    t = duration_timer.ticks();
  }

  uint64 ret = HighResolutionTimer::to_milliseconds(t);
  EXPECT_GE(ret, DURATION);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init(argc, argv) == false )
  {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  return RUN_ALL_TESTS();
}
