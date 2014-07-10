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
#ifndef NOMLIB_SYSTEM_SDL2_LOGGER_HPP
#define NOMLIB_SYSTEM_SDL2_LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include "SDL.h"

#include "nomlib/system/clock.hpp"

/// \brief The predefined logging categories.
///
/// \remarks By default the application category is enabled at the INFO level,
/// the assert category is enabled at the WARN level, test is enabled at the
/// VERBOSE level and all other categories are enabled at the CRITICAL level.
///
/// \note This enumeration declaration is intentionally outside of the nom
/// namespace, for the sake of preserving legacy code and for convenience.
enum
{
  /// \brief Default log category
  ///
  /// \deprecated This identifier exists only to preserve legacy code, and
  /// isn't likely to disappear anytime soon. New code should use one of the
  /// logging categories that best describes their purpose (or add a new one).
  NOM = SDL_LOG_CATEGORY_CUSTOM,

  NOM_LOG_CATEGORY_APPLICATION,
  NOM_LOG_CATEGORY_ERROR,
  NOM_LOG_CATEGORY_ASSERT,
  NOM_LOG_CATEGORY_SYSTEM,
  NOM_LOG_CATEGORY_AUDIO,
  NOM_LOG_CATEGORY_VIDEO,
  NOM_LOG_CATEGORY_RENDER,
  NOM_LOG_CATEGORY_INPUT,
  NOM_LOG_CATEGORY_TEST,
  NOM_LOG_CATEGORY_GUI,

  /// \brief Function call trace -- entry and exit points
  NOM_LOG_CATEGORY_TRACE,
  NOM_LOG_CATEGORY_TRACE_AUDIO,
  NOM_LOG_CATEGORY_TRACE_GRAPHICS,
  NOM_LOG_CATEGORY_TRACE_FONTS,
  NOM_LOG_CATEGORY_TRACE_GUI,
  NOM_LOG_CATEGORY_TRACE_EVENTS,
  NOM_LOG_CATEGORY_TRACE_MATH,
};

namespace nom {

/// \brief Helper method for nom::SDL2Logger.
template<typename Type>
void write_debug_output( std::ostream& out, const Type& f )
{
  out << f;
}

/// \brief High-level logging API
///
/// \remarks This implementation is a C++ wrapper for SDL2's logging facilities.
///
/// \see NOM_LOG macros; they are the principle user of this class interface.
///
/// \see This implementation was inspired by a [Q&A](http://mortoray.com/2013/11/01/bridging-__va_args__-to-c-variadic-templates/)
/// and by a [SDL2 C++ wrapper](https://gist.github.com/aggsol/6339637). Thank
/// you!
class SDL2Logger
{
  public:
    typedef SDL2Logger self_type;

    /// \brief Default constructor; initialize the log category to NOM and
    /// the log priority to SDL_LOG_PRIORITY_INFO.
    SDL2Logger( void );

    /// \brief Construct an object for logging.
    ///
    /// \param cat The logging category to assign.
    ///
    /// \param prio A SDL_LogPriority enumeration value to use as the logging
    /// priority.
    SDL2Logger( int cat, SDL_LogPriority prio );

    /// \brief Destructor; the buffered output for logging is flushed to the
    /// underlying implementation at this time.
    ~SDL2Logger( void );

    /// \brief Write to the output buffer.
    ///
    /// \remarks This method supports the passing of multiple string objects /
    /// literals (via C++11 variadic templates).
    ///
    /// \note The output buffer is not flushed until the destruction of the
    /// object occurs.
    template <typename Type, typename ... Args>
    void write( const Type& f, const Args& ... rest )
    {
      nom::write_debug_output( this->output_stream(), f );

      this->write( " " );

      this->write( rest... );
    }

    /// \brief Write to the output buffer.
    ///
    /// \remarks This method supports writing to the output buffer with a
    /// single string literal / object.
    ///
    /// \note The output buffer is not flushed until the destruction of the
    /// object occurs.
    ///
    /// \see ::write( const Type&, const Args& )
    template <typename Type>
    void write( const Type& f )
    {
      nom::write_debug_output( this->output_stream(), f );
    }

    /// \brief Set the output buffer.
    ///
    /// \remarks This is a method intended for combining with macros for
    /// supporting the condition where you wish to write nothing (i.e.: disable
    /// the logging macro when building a release target).
    ///
    /// \note The output buffer is not flushed until the destruction of the
    /// object occurs.
    ///
    /// \see ::write( const Type&, const Args& )
    void write( void );

    // /// \brief Get the category this object was constructed with.
    int category( void ) const;

    /// \brief Get the logging priority this object was constructed with.
    SDL_LogPriority priority( void ) const;

    /// \brief Get the buffered output as a C++ output stream.
    ///
    /// \returns The message to be logged.
    std::ostringstream& output_stream( void );

    /// \brief Get the buffered output as a C++ string (std::string).
    ///
    /// \returns The message to be logged.
    std::string output_string( void ) const;

    /// \brief Get the buffered output as a C-style string.
    ///
    /// \remarks This is a compatibility method for using with C library APIs,
    /// such as SDL2.
    const char* output_cstring( void );

    // void set_category( int cat );

    // void set_priority( SDL_LogPriority p );

    static void set_logging_priorities( SDL_LogPriority prio );

    static void set_logging_priority( int cat, SDL_LogPriority prio );

  private:
    /// \brief Copy constructor.
    ///
    /// \remarks This object is non copyable.
    ///
    /// \note This object cannot be copied because of our internal
    /// std::ostringstream object member.
    SDL2Logger( const self_type& rhs ) = delete;

    /// \brief Copy assignment constructor.
    ///
    /// \remarks This object is non copy assignable.
    ///
    /// \note This object cannot be copied because of our internal
    /// std::ostringstream object member.
    self_type& operator =( const self_type& rhs ) = delete;

    /// \brief The logging category this buffered output is in.
    ///
    /// \see [SDL2 logging functions](https://wiki.libsdl.org/CategoryLog).
    int category_;

    /// \brief The logging priority this buffered output has.
    ///
    /// \see [SDL2 logging functions](https://wiki.libsdl.org/CategoryLog).
    SDL_LogPriority priority_;

    /// \brief The buffered output.
    ///
    /// \note This object is not flushed -- written to the underlying logging
    /// interface -- until the destruction of this object occurs.
    std::ostringstream os_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Logger
/// \ingroup system
///
///
/// \see http://mortoray.com/2013/11/01/bridging-__va_args__-to-c-variadic-templates/
///
