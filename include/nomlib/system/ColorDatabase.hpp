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
#ifndef NOMLIB_SYSTEM_COLOR_DB_HPP
#define NOMLIB_SYSTEM_COLOR_DB_HPP

#include <string>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"

namespace nom {

/// \brief Container for accessing colors by name (string)
///
/// \see Inspired by http://docs.wxwidgets.org/trunk/classwx_colour_database.html
class ColorDatabase
{
  public:
    typedef ColorDatabase self_type;

    typedef std::pair<std::string, Color4i> pair;
    typedef std::map<std::string, Color4i> value_type;

    /// \brief Default constructor.
    ColorDatabase( void );

    /// \brief Destructor.
    ~ColorDatabase( void );

    const Color4i& find_color( const std::string& key );

    bool append_color( const std::string& key, const Color4i& c );

    void erase_color( const std::string& key );

  private:
    value_type colors_;
};

} // namespace nom

#endif // include guard defined
