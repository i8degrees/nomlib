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
#ifndef NOMLIB_GUI_FINAL_FANTASY_DECORATOR_HPP
#define NOMLIB_GUI_FINAL_FANTASY_DECORATOR_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/gui/Decorator.hpp"

namespace nom {

/// \brief Final Fantasy theme widget style.
class FinalFantasyDecorator: public Decorator
{
  public:
    typedef FinalFantasyDecorator self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor; initialize the Final Fantasy VIII theme
    /// style (TTcards).
    FinalFantasyDecorator( void );

    /// \brief Destructor.
    ~FinalFantasyDecorator( void );

    /// \brief Re-implements IObject::type.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Implements required interface IDrawable::clone.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Implements required interface IDrawable::update.
    void update( void );

    /// \brief Implements required interface IDrawable::draw.
    void draw( RenderTarget& target ) const;

  private:
    /// \brief Color sets used to create the gradients used as the widget's
    /// background.
    Color4iColors g_colors_[2];
};

} // namespace nom

#endif // include guard defined
