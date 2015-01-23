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
#ifndef NOMLIB_CORE_SDL2_LOGGER_HPP
#define NOMLIB_CORE_SDL2_LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <SDL.h>

#include "nomlib/core/clock.hpp"

/// \brief The predefined logging categories.
///
/// \remarks By default: NOM_LOG_CATEGORY_APPLICATION is enabled at the
/// LogPriority::NOM_LOG_PRIORITY_INFO priority level;
/// LogPriority::NOM_LOG_CATEGORY_ASSERT is enabled at the
/// LogPriority::NOM_LOG_PRIORITY_WARN priority level,
/// LogPriority::NOM_LOG_CATEGORY_TEST is enabled
/// at the LogPriority::NOM_LOG_PRIORITY_VERBOSE priority level and everything
/// else is LogPriority::NOM_LOG_PRIORITY_CRITICAL priority level. These
/// priority levels intend to mimic the defaults provided by SDL2's logging
/// subsystem.
/// Additionally, NOM_LOG_CATEGORY_RENDER is enabled at NOM_LOG_PRIORITY_WARN
/// priority level when nomlib is compiled with debugging (NOM_DEBUG).
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

  /// \brief Animation action objects
  NOM_LOG_CATEGORY_ANIMATION,
  NOM_LOG_CATEGORY_ACTION_PLAYER,
  NOM_LOG_CATEGORY_ACTION_QUEUE,

  /// \brief Events logging category
  NOM_LOG_CATEGORY_EVENT,

  /// \brief Test logging category
  NOM_LOG_CATEGORY_TEST,

  /// \brief GUI subsystem logging category
  NOM_LOG_CATEGORY_GUI,

  /// \brief Memory allocation & deallocations
  NOM_LOG_CATEGORY_MEMORY,

  /// \brief Totals (user-defined) of memory allocation & deallocations
  NOM_LOG_CATEGORY_MEMORY_TOTALS,

  /// \brief Game states debugging
  NOM_LOG_CATEGORY_STATES,

  NOM_LOG_CATEGORY_TRACE,
  NOM_LOG_CATEGORY_TRACE_AUDIO,

  /// \todo Rename to NOM_LOG_CATEGORY_TRACE_FONT
  NOM_LOG_CATEGORY_TRACE_FONTS,

  NOM_LOG_CATEGORY_TRACE_VIDEO,
  NOM_LOG_CATEGORY_TRACE_RENDER,
  NOM_LOG_CATEGORY_TRACE_GUI,
  NOM_LOG_CATEGORY_TRACE_EVENT,

  /// \todo Rename to NOM_LOG_CATEGORY_TRACE_STATE
  NOM_LOG_CATEGORY_TRACE_STATES,

  NOM_LOG_CATEGORY_TRACE_SYSTEM,

  /// \brief Call stacks for engine unit tests
  NOM_LOG_CATEGORY_TRACE_UNIT_TEST,

  /// \brief Call stacks for animation engine objects
  NOM_LOG_CATEGORY_TRACE_ANIMATION,

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

/// \brief Standard logging priorities
///
/// \remarks This enumeration exists to provide an abstraction for future
/// implementation of logging subsystems that derive from a common interface.
///
/// \see The SDL_LOG_PRIORITY enumeration.
enum LogPriority
{
  NOM_LOG_PRIORITY_VERBOSE = SDL_LOG_PRIORITY_VERBOSE,
  NOM_LOG_PRIORITY_DEBUG = SDL_LOG_PRIORITY_DEBUG,
  NOM_LOG_PRIORITY_INFO = SDL_LOG_PRIORITY_INFO,
  NOM_LOG_PRIORITY_WARN = SDL_LOG_PRIORITY_WARN,
  NOM_LOG_PRIORITY_ERROR = SDL_LOG_PRIORITY_ERROR,
  NOM_LOG_PRIORITY_CRITICAL = SDL_LOG_PRIORITY_CRITICAL,
  NOM_NUM_LOG_PRIORITIES = SDL_NUM_LOG_PRIORITIES
};

namespace priv {

/// \brief Customized log output function for SDL_LogMessage
///
/// \remarks This output function supports color output for UNIX (POSIX) and
/// Windows platforms.
void log_message( void* ptr, int cat, SDL_LogPriority prio, const char* msg );

} // namespace priv

/// \brief Helper method for nom::SDL2Logger.
///
/// \todo Optionally support modifying floating-point precision so to help
/// catch floating-point math errs such as 254.999984741 being represented
/// in the debug output as 255 without explicitly setting the precision to
/// five (5) or greater.
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

    /// \brief Human-friendly descriptions of the logging priorities.
    static const std::string priority_prefixes_[NOM_NUM_LOG_PRIORITIES];

    /// \brief Get initialization status of the default logging category
    /// priorities.
    static bool initialized( void );

    /// \brief Set the default logging category priority levels.
    ///
    /// \see NOM_LOG_CATEGORY enumeration.
    static void initialize();

    /// \brief Default constructor; initialize the log category to NOM and
    /// the log priority to LogPriority::NOM_LOG_PRIORITY_INFO.
    SDL2Logger();

    /// \brief Construct a logging object.
    ///
    /// \param cat        The logging category to assign.
    ///
    /// \param prio       A nom::LogPriority enumeration value to use as the
    /// logging priority.
    ///
    /// \see ::write.
    SDL2Logger( int cat, nom::LogPriority prio );

    /// \brief Destructor; the logging category, logging priority level and
    /// output stream is sent to the underlying implementation at this time.
    /// Care must be taken when using this interface outside the provided
    /// macros, as the message is not output until this point.
    ///
    /// If you wish to set a custom log output function for logging, you should
    /// do so before destruction occurs.
    ~SDL2Logger();

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
    void write();

    /// \brief Get the logging category.
    int category() const;

    /// \brief Get the logging priority level.
    LogPriority priority() const;

    /// \brief Get the log message.
    ///
    /// \returns A reference to the logger's C++ output stream.
    std::ostringstream& output_stream();

    /// \brief Convert an underlying implementation's logging priority level.
    ///
    /// \returns The equivalent nom::LogPriority enumeration type.
    static LogPriority priority( SDL_LogPriority prio );

    /// \brief Convert the logging priority level to the underlying
    /// implementation's type.
    static SDL_LogPriority SDL_priority( LogPriority prio );

    /// \brief Set the logging priority for all logging categories.
    ///
    /// \remarks This is a wrapper for SDL_SetAllPriority.
    static void set_logging_priorities( LogPriority prio );

    /// \brief Set the logging priority for a log category.
    ///
    /// \remarks This is a wrapper for SDL_SetPriority.
    static void set_logging_priority( int cat, LogPriority prio );

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
    nom::LogPriority priority_;

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
/// nom::SDL2Logger::set_logging_priorities( nom::LogPriority::NOM_LOG_PRIORITY_WARN );
/// \endcode
///
/// \note This implementation is a C++ wrapper for SDL2's logging facilities,
/// and is intended to be fully compatible & consistent with except where
/// explicitly stated otherwise.
///
/// \remarks Logging priorities are reset upon a call to SDL_Quit.
///
/// \see [SDL2 Logging API](https://wiki.libsdl.org/CategoryLog)
///
/// \see This implementation was inspired by a [Q&A blog](http://mortoray.com/2013/11/01/bridging-__va_args__-to-c-variadic-templates/)
/// and by a [SDL2 C++ wrapper](https://gist.github.com/aggsol/6339637).
///
/// \see Convenience logging macros -- NOM_LOG_* -- for the primary usage API.
///
