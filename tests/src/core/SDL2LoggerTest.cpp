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
#include "gtest/gtest.h"

#include "nomlib/config.hpp"
#include "nomlib/core.hpp"
#include "nomlib/math.hpp"

using namespace nom;

/// \brief Customized log output function for SDL_LogMessage.
///
/// \remarks This function is used only in the
/// SDL2LoggerTest::CustomLogOutputFunction unit test.
void log_message( void* ptr, int cat, SDL_LogPriority prio, const char* msg )
{
  if( prio < NOM_NUM_LOG_PRIORITIES && prio > 0 )
  {
    std::cout << nom::SDL2Logger::priority_prefixes_[prio] << ": ";
  }

  std::cout << msg << std::endl;
}

/// \remarks For use with NOM_LOG_TRACE functionality testing
struct FunctionTraceTest
{
  FunctionTraceTest( int cat, nom::LogPriority prio ) :
    cat_{ cat },
    priority_{ prio }
  {
    NOM_LOG_TRACE( this->cat_ );
  }

  ~FunctionTraceTest( void )
  {
    NOM_LOG_TRACE( this->cat_ );
  }

  int cat_;
  nom::LogPriority priority_;
};

/// \remarks Helper method for verifying test conditions
void expected_logger_state( int cat, nom::LogPriority prio, const std::string& msg, bool log_output_state )
{
  // This is the control state logic that SDL uses to determine whether or not
  // to output a logged message. If this logic is false, the log message should
  // never be seen.
  if( log_output_state )
  {
    EXPECT_FALSE( prio < SDL_LogGetPriority(cat) )
    << "Our log priority level should not be less than SDL's log priority level; "
    << "this log message should be output to the console.";
  }
  else
  {
    EXPECT_TRUE( prio < SDL_LogGetPriority(cat) )
    << "Our log priority level should be less than SDL's log priority level; "
    << "this log message should not be output to the console.";
  }
}

TEST( SDL2LoggerTest, LogVerbose )
{
  int cat = NOM;
  nom::LogPriority prio = NOM_LOG_PRIORITY_VERBOSE;
  std::string out = "Verbose message";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_VERBOSE(cat, out);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_LOG_VERBOSE(cat, out);

  SDL2Logger::set_logging_priority( cat, LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  expected_logger_state( cat, prio, out, false );
}

TEST( SDL2LoggerTest, LogDebug )
{
  int cat = NOM;
  nom::LogPriority prio = NOM_LOG_PRIORITY_DEBUG;
  std::string out = "Debug message";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_DEBUG(cat, out);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_LOG_DEBUG(cat, out);

  SDL2Logger::set_logging_priority( cat, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  expected_logger_state( cat, prio, out, false );
}

TEST( SDL2LoggerTest, LogInfo )
{
  int cat = NOM;
  nom::LogPriority prio = NOM_LOG_PRIORITY_INFO;
  std::string out = "Could not initialize something totally not important.";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_INFO(cat, out);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_LOG_INFO(cat, out);

  SDL2Logger::set_logging_priority( cat, LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  expected_logger_state( cat, prio, out, false );
}

TEST( SDL2LoggerTest, LogWarn )
{
  int cat = NOM;
  nom::LogPriority prio = NOM_LOG_PRIORITY_WARN;
  std::string out = "Font cache was not initialized. Initializing...";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_WARN(cat, out);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_LOG_WARN(cat, out);

  SDL2Logger::set_logging_priority( cat, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  expected_logger_state( cat, prio, out, false );
}

TEST( SDL2LoggerTest, LogError )
{
  int cat = NOM;
  nom::LogPriority prio = nom::LogPriority::NOM_LOG_PRIORITY_ERROR;
  std::string out = "Could not load image resources.";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_ERR(NOM, out);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_LOG_ERR(NOM, out);

  SDL2Logger::set_logging_priority( cat, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  expected_logger_state( cat, prio, out, false );
}

TEST( SDL2LoggerTest, LogCritical )
{
  int cat = NOM;
  nom::LogPriority prio = nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL;
  std::string out = "Could not initialize application. Exiting...";

  // Should see this logged message
  SDL2Logger::set_logging_priorities( prio );

  expected_logger_state( cat, prio, out, true );

  NOM_LOG_CRIT(cat, out);

  // SDL2Logger::set_logging_priority( cat, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  // expected_logger_state( cat, prio, out, true );
}

TEST( SDL2LoggerTest, LogVariable )
{
  int cat = NOM;
  nom::LogPriority prio = nom::LogPriority::NOM_LOG_PRIORITY_DEBUG;
  std::string out = "pos: 25, 25";
  Point2i pos(25,25);

  // Should see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

  expected_logger_state( cat, prio, out, true );

  NOM_DUMP_VAR(cat, pos);

  // Should not see this logged message
  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  expected_logger_state( cat, prio, out, false );

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, cat );
  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "pos:", Point2i::null );
}

TEST( SDL2LoggerTest, LogFunctionTraceNomCategory )
{
  int cat = NOM;
  nom::LogPriority p = nom::LogPriority::NOM_LOG_PRIORITY_DEBUG;

  SDL2Logger::set_logging_priorities( p );

  // Should see this function trace logged
  FunctionTraceTest t( cat, p );
}

TEST( SDL2LoggerTest, LogFunctionTraceDisabledCategory )
{
  int cat = NOM_LOG_CATEGORY_TRACE_RENDER;
  nom::LogPriority p = nom::LogPriority::NOM_LOG_PRIORITY_DEBUG;

  // Disable category logging except for critical
  SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  // Enable category logging of function call traces
  SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE_RENDER, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

  // Should see this function trace logged
  FunctionTraceTest t1( cat, p );

  EXPECT_FALSE( p < SDL_LogGetPriority( cat ) )
  << "Our log priority level should not be less than SDL's log priority level; "
  << "this log message should be output to the console.";

  // Should not see this function trace logged
  FunctionTraceTest t2( NOM_LOG_CATEGORY_TRACE, p );

  EXPECT_TRUE( p < SDL_LogGetPriority( NOM_LOG_CATEGORY_TRACE ) )
  << "Our log priority level should be less than SDL's log priority level; "
  << "this log message should not be output to the console.";
}

TEST( SDL2LoggerTest, LogStubbedFunction )
{
  NOM_STUBBED( NOM );
}

TEST( SDL2LoggerTest, CustomLogOutputFunction )
{
  int cat = NOM;
  SDL_LogPriority prio = nom::SDL2Logger::SDL_priority( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );
  std::string out = "Could not initialize application. Exiting...";
  void ( *cb )( void*, int, SDL_LogPriority, const char* ) = log_message;

  SDL_LogSetOutputFunction( cb, nullptr );

  SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  // Should see this logged message
  SDL2Logger::set_logging_priorities( nom::SDL2Logger::priority( prio ) );

  expected_logger_state( cat, nom::SDL2Logger::priority( prio ), out, true );

  NOM_LOG_CRIT(cat, out);
}

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Note that SDL2 does not need to be initialized before using the logging
  // facilities!

  return RUN_ALL_TESTS();
}
