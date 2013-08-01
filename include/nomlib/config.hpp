/******************************************************************************

    Configuration file for nomlib library

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_CONFIG_HEADERS
#define NOMLIB_CONFIG_HEADERS

// nomlib version
#include "nomlib/version.hpp"

// nomlib data types
#include "nomlib/types.hpp"

// Platform detection
#include "nomlib/platforms.hpp"

/// \brief nomlib Debugging Options
///
/// Available preprocessor flags:
///
///     DEBUG
/// Activates output of all info, warning and error messages.
///
///     DEBUG_ASSERT
/// Activate run-time assert checks scattered throughout the code-base.
///
///     DEBUG_TRACE
/// Special debug level for activating output of each class object construction
/// and destruction that occurs. Unless you are an active developer of nomlib,
/// or perhaps suspect that an error in your software could be caused by nomlib,
/// you are unlikely to be interested in this debug level -- this option
/// generates a hefty amount of output text!
///
/// These definitions are passed at build time for activation like so:
///
///     cd build && cmake -DDEBUG=on ..
///
/// You may mix and match these flags however you wish.

#ifndef __cplusplus
  #warning "nomlib requires a C++11 compiler"
#endif


#endif // NOMLIB_CONFIG_HEADERS defined
