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
#ifndef NOMLIB_GUI_DECORATOR_PHOTOGRAPH_HPP
#define NOMLIB_GUI_DECORATOR_PHOTOGRAPH_HPP

#include <Rocket/Core/Decorator.h>

#include "nomlib/graphics/Texture.hpp"

namespace nom {

/// \todo Rename class
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Decorators
/// \see http://librocket.com/wiki/documentation/RCSS/Decorators
class DecoratorPhotograph : public Rocket::Core::Decorator
{
  public:
    virtual ~DecoratorPhotograph();

    bool Initialise(const Rocket::Core::String& image_source, const Rocket::Core::String& image_path);

    /// Called on a decorator to generate any required per-element data for a newly decorated element.
    /// @param element[in] The newly decorated element.
    /// @return A handle to a decorator-defined data handle, or NULL if none is needed for the element.
    virtual Rocket::Core::DecoratorDataHandle GenerateElementData(Rocket::Core::Element* element);

    /// Called to release element data generated by this decorator.
    /// @param element_data[in] The element data handle to release.
    virtual void ReleaseElementData(Rocket::Core::DecoratorDataHandle element_data);

    /// Called to render the decorator on an element.
    /// @param element[in] The element to render the decorator on.
    /// @param element_data[in] The handle to the data generated by the decorator for the element.
    virtual void RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle element_data);

  private:
    /// \todo Rename
    Texture image_;

    // int image_idx;
    // Point2i pos_;
    // Size2i dims_;
    // bool dirty_ = true;
};

} // namespace nom

#include <Rocket/Core/DecoratorInstancer.h>
// #include "nomlib/graphics/IDrawable.hpp"

namespace nom {

/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Decorators
/// \see http://librocket.com/wiki/documentation/RCSS/Decorators
class DecoratorInstancerPhotograph : public Rocket::Core::DecoratorInstancer
{
  public:
    DecoratorInstancerPhotograph();
    virtual ~DecoratorInstancerPhotograph();

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
