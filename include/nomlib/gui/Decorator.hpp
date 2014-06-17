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
#ifndef NOMLIB_GUI_DECORATOR_HPP
#define NOMLIB_GUI_DECORATOR_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/gui/IDecorator.hpp"
#include "nomlib/gui/Drawables.hpp"

namespace nom {

/// \brief Customized GUI widget theme style.
///
/// \todo Implement interface for calculating and providing border dimensions
/// so that our widget(s) are able to better account for their own size
/// requirements.
class Decorator: public IDecorator
{
  public:
    typedef Decorator self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor; do nothing.
    Decorator( void );

    /// \brief Destructor.
    virtual ~Decorator( void );

    /// \remarks Implements IDecorator::margins.
    virtual IntRect margins( void ) const;

    /// \brief Get the unaffected rendering area coordinates of the border.
    ///
    /// \remarks Implements IDecorator::frame_bounds.
    virtual IntRect frame_bounds( void ) const;

    /// \brief Implements IDecorator::invalidate.
    virtual void invalidate( void );

    /// \brief Re-implements IObject::type.
    ///
    /// \remarks This uniquely identifies the object's type.
    virtual ObjectTypeInfo type( void ) const;

    /// \remarks Re-implements Transformable::set_bounds.
    virtual void set_bounds( const IntRect& bounds );

    /// \remarks Implements IDecorator::set_margins.
    virtual void set_margins( const IntRect& margins );

    /// \brief Set the intended boundaries of the "client area", meaning the
    /// unaffected rendering area of the rendered border.
    ///
    /// \remarks The boundary coordinates should exclude the rendering area used
    ///  by the rendering of the border, leaving only what is free (think: safe)
    /// for others to render on.
    ///
    /// \note This information is provided as a convenience for visual
    /// interfaces that require such detail about their environment, such as
    /// a GUI engine -- nom::UIWidget and friends.
    virtual void set_frame_bounds( const IntRect& bounds );

  protected:
    /// \brief Internal container state for rendered objects.
    Drawables drawables_;

  private:
    IntRect margins_;

    /// \brief The rendering area unaffected by the rendered border.
    IntRect frame_bounds_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::Decorator
/// \ingroup gui
///
/// Note that you must first set the bounds -- position and size -- of this
/// object before a decorator can render anything. See also: set_bounds.
///
