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
#ifndef NOMLIB_SDL2_IDRAWABLE_HPP
#define NOMLIB_SDL2_IDRAWABLE_HPP

#include <vector>
#include <memory>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

/// \brief Base interface class for grouping objects under a united front
class IDrawable
{
  public:
    /// std::unique_ptr definition type for your convenience, sir!
    typedef std::unique_ptr<IDrawable> UniquePtr;
    /// std::shared_ptr definition type at your leisure, sir!
    typedef std::shared_ptr<IDrawable> SharedPtr;
    /// Vector of std::shared_ptr definition type for the brotherhood, sir!
    typedef std::vector<std::shared_ptr<IDrawable>> SharedDrawables;

    /// std::unique_ptr vector container type
    typedef std::vector<std::unique_ptr<IDrawable>> UniqueDrawables;

    typedef const RenderWindow RenderTarget;

    /// Do nothing at all default constructor
    IDrawable ( void ) {}

    /// Do nothing at all destructor
    virtual ~IDrawable ( void ) {}

    /// Intended to be used internally by the class re-implementing this method;
    /// for use in preparing the buffer to be drawn using the draw method call.
    /// Whereas both Update & Draw are typically publicly exposed to the
    /// end-user, only the Draw method call should be expected of the end-user
    /// to be explicitly called.
    ///
    /// \todo Not all the interfaced (inherited) IDrawable objects in this
    /// library conform to the expectations stated above. We need to first
    /// figure out if we wish to stay on this course (of expectation), or
    /// diverge all together. Undecided.
    /// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-03
    virtual void update ( void ) = 0;

    /// Render the object onto the primary video buffer -- your visible screen.
    virtual void draw ( RenderTarget& ) const = 0;
};


} // namespace nom

#endif // NOMLIB_IDRAWABLE_HEADERS defined

/// \class nom::IDrawable
/// \ingroup graphics
///
/// nom::IDrawable is a simple, small base interface class ("a pure abstract class")
/// intended for bridging a reliable contract between two or more classes.
///
///           [ADDITIONAL DESCRIPTION]
///
/// You may freely use this class both inside and outside of the library, with
/// little worry of trouble arising. Sprite layering (think: groups) are the
/// first use that comes to mind!
///
/// Usage example:
/// \code
/// #include <nomlib/graphics.hpp>
///
///           [TODO]
///
/// \endcode
