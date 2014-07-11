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
/// \remarks By default: NOM_LOG_CATEGORY_APPLICATION is enabled at the
/// SDL_LOG_PRIORITY_INFO priority level; SDL_LOG_CATEGORY_ASSERT is enabled at
/// the SDL_LOG_PRIORITY_WARN priority level, SDL_LOG_CATEGORY_TEST is enabled
/// at the SDL_LOG_PRIORITY_VERBOSE priority level and everything else is
/// SDL_LOG_PRIORITY_CRITICAL priority level.
///
/// \note This enumeration declaration is intentionally outside of the nom
/// namespace, for the sake of both preserving legacy code and for convenience.
enum
{
  /// \brief Default log category
  ///
  /// \deprecated This identifier exists only to preserve legacy code, and
  /// isn't likely to disappear anytime soon. New code should use one of the
  /// logging categories that best describes their purpose (or add a new one).
  NOM = SDL_LOG_CATEGORY_CUSTOM,

  /// \brief Application logging category
  NOM_LOG_CATEGORY_APPLICATION,

  /// \brief Error logging category
  NOM_LOG_CATEGORY_ERROR,

  /// \brief Assertion logging category
  NOM_LOG_CATEGORY_ASSERT,

  /// \brief System subsystem logging category
  NOM_LOG_CATEGORY_SYSTEM,

  /// \brief Audio subsystem logging category
  NOM_LOG_CATEGORY_AUDIO,

  /// \brief Video subsystem logging category
  NOM_LOG_CATEGORY_VIDEO,

  /// \brief Rendering subsystem logging category
  NOM_LOG_CATEGORY_RENDER,

  /// \brief User inputs logging category
  NOM_LOG_CATEGORY_INPUT,

  /// \brief Test logging category
  NOM_LOG_CATEGORY_TEST,

  /// \brief GUI subsystem logging category
  NOM_LOG_CATEGORY_GUI,

  NOM_LOG_CATEGORY_TRACE,
  NOM_LOG_CATEGORY_TRACE_AUDIO,
  NOM_LOG_CATEGORY_TRACE_FONTS,
  NOM_LOG_CATEGORY_TRACE_VIDEO,
  NOM_LOG_CATEGORY_TRACE_RENDER,
  NOM_LOG_CATEGORY_TRACE_GUI,
  NOM_LOG_CATEGORY_TRACE_EVENTS,
  NOM_LOG_CATEGORY_TRACE_STATES,
  NOM_LOG_CATEGORY_TRACE_SYSTEM,

  /// \brief Custom logging category that is reserved for application-level use.
  /// For example:
  ///
  /// \code
  /// enum
  /// {
  ///   MYAPP_LOG_CATEGORY_AWESOME1 = NOM_LOG_CATEGORY_CUSTOM,
  ///   MYAPP_LOG_CATEGORY_AWESOME2,
  ///   MYAPP_LOG_CATEGORY_AWESOME3
  ///   // ...and so on
  /// };
  /// \endcode
  NOM_LOG_CATEGORY_CUSTOM
};

namespace nom {

/// \brief Helper method for nom::SDL2Logger.
template<typename Type>
void write_debug_output( std::ostream& out, const Type& f )
{
  out << f;
}

/// \brief Category & priority message logging API
class SDL2Logger
{
  public:
    typedef SDL2Logger self_type;

    /// \brief Get initialization status of the default logging category
    /// priorities.
    static bool initialized( void );

    /// \brief Set the default logging category priority levels.
    ///
    /// \see NOM_LOG_CATEGORY_* enumeration.
    static void initialize( void );

    /// \brief Default constructor; initialize the log category to NOM and
    /// the log priority to SDL_LOG_PRIORITY_INFO.
    SDL2Logger( void );

    /// \brief Construct a logging object.
    ///
    /// \param cat The logging category to assign.
    ///
    /// \param prio A SDL_LogPriority enumeration value to use as the logging
    /// priority.
    ///
    /// \see ::write.
    SDL2Logger( int cat, SDL_LogPriority prio );

    /// \brief Destructor; the logging category, logging priority level and
    /// output stream is sent to the underlying implementation at this time.
    /// Care must be taken when using this interface outside the provided
    /// macros, as the message is not output until this point.
    ///
    /// If you wish to set a custom log output function for logging, you should
    /// do so before destruction occurs.
    ~SDL2Logger( void );

    /// \brief Write one or more log messages to the output stream.
    ///
    /// \param rest Variable list of std::ostringstream compatible arguments.
    template <typename Type, typename ... Args>
    void write( const Type& f, const Args& ... rest )
    {
      nom::write_debug_output( this->output_stream(), f );

      this->write( " " );

      this->write( rest... );
    }

    /// \brief Write a log message to the output stream.
    ///
    /// \remarks This method supports writing to the output buffer with a
    /// single string literal / object.
    ///
    /// \see ::write( const Type&, const Args& )
    template <typename Type>
    void write( const Type& f )
    {
      nom::write_debug_output( this->output_stream(), f );
    }

    /// \brief Write a log message to the output stream.
    ///
    /// \remarks This is an intentional method overload when the desire is to
    /// not write anything.
    ///
    /// \see ::write( const Type&, const Args& )
    void write( void );

    /// \brief Get the logging category.
    int category( void ) const;

    /// \brief Get the logging priority level.
    SDL_LogPriority priority( void ) const;

    /// \brief Get the log message.
    ///
    /// \returns A reference to the logger's C++ output stream.
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

    /// \brief Set the logging priority for all logging categories.
    ///
    /// \remarks This is a wrapper for SDL_SetAllPriority.
    static void set_logging_priorities( SDL_LogPriority prio );

    /// \brief Set the logging priority for a log category.
    ///
    /// \remarks This is a wrapper for SDL_SetPriority.
    static void set_logging_priority( int cat, SDL_LogPriority prio );

  private:
    /// \brief Copy constructor.
    ///
    /// \remarks This object cannot be copied.
    ///
    /// \note The object cannot be copied because of the usage of
    /// std::ostringstream.
    SDL2Logger( const self_type& rhs ) = delete;

    /// \brief Copy assignment constructor.
    ///
    /// \remarks This object cannot be copy assigned.
    ///
    /// \note The object cannot be copied because of the usage of
    /// std::ostringstream.
    self_type& operator =( const self_type& rhs ) = delete;

    /// \brief The logging category of the log message.
    int category_;

    /// \brief The logging priority of the log message.
    SDL_LogPriority priority_;

    /// \brief The log message stream.
    std::ostringstream os_;

    /// \brief Ensure one-time initialization of the setting of logging category
    /// priorities.
    ///
    /// \see NOM_LOG_CATEGORY_* enumeration.
    static bool initialized_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::SDL2Logger
/// \ingroup system
///
/// A simple category & priority based logging facility.
///
/// If you are debugging nomlib, you might want to call:
///
/// \code
/// nom::SDL2Logger::set_logging_priorities( SDL_LOG_PRIORITY_WARN );
/// \endcode
///
/// \remarks It is a good idea to ensure that SDL2 has been initialized before
/// using any functionality from this API; the first logged message not being
/// has been observed when SDL2 has not been yet been initialized.
///
/// \note This implementation is a C++ wrapper for SDL2's logging facilities,
/// and is intended to be fully compatible & consistent with except where
/// explicitly stated otherwise.
///
/// \see [SDL2 Logging API](https://wiki.libsdl.org/CategoryLog)
///
/// \see This implementation was inspired by a [Q&A blog](http://mortoray.com/2013/11/01/bridging-__va_args__-to-c-variadic-templates/)
/// and by a [SDL2 C++ wrapper](https://gist.github.com/aggsol/6339637).
///
/// \see Convenience logging macros -- NOM_LOG_* -- for the primary usage API.
///
