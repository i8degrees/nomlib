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
#ifndef NOMLIB_LIBROCKET_DECORATOR_INSTANCER_SPRITE_HPP
#define NOMLIB_LIBROCKET_DECORATOR_INSTANCER_SPRITE_HPP

#include "nomlib/config.hpp"

#include <Rocket/Core/DecoratorInstancer.h>

namespace nom {

/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Decorators
/// \see http://librocket.com/wiki/documentation/RCSS/Decorators
///
/// \see nom::DecoratorSpriteBatch
class DecoratorInstancerSprite: public Rocket::Core::DecoratorInstancer
{
  public:
    /// \brief Default constructor.
    ///
    /// \remarks Register RCSS properties for sprite-based decorators.
    DecoratorInstancerSprite();

    /// \brief Destructor.
    virtual ~DecoratorInstancerSprite();

    /// Instances a decorator given the property tag and attributes from the RCSS file.
    /// @param name The type of decorator desired. For example, "background-decorator: simple;" is declared as type "simple".
    /// @param properties All RCSS properties associated with the decorator.
    /// @return The decorator if it was instanced successful, NULL if an error occured.
    Rocket::Core::Decorator* InstanceDecorator(const Rocket::Core::String& name, const Rocket::Core::PropertyDictionary& properties);

    /// Releases the given decorator.
    /// @param decorator Decorator to release. This is guaranteed to have been constructed by this instancer.
    void ReleaseDecorator(Rocket::Core::Decorator* decorator);

    /// Releases the instancer.
    void Release();
};

} // namespace nom

#endif // include guard
