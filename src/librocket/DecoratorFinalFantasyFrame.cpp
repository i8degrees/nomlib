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
#include "nomlib/librocket/DecoratorFinalFantasyFrame.hpp"

#include <Rocket/Core/Math.h>
#include <Rocket/Core/Element.h>

// Private headers
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/FinalFantasyDecorator.hpp"
#include "nomlib/librocket/RocketSDL2Renderer.hpp"

// Forward declarations
#include "nomlib/gui/IDecorator.hpp"

namespace nom {

DecoratorFinalFantasyFrame::DecoratorFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  this->decorator_.reset( new FinalFantasyDecorator() );
}

DecoratorFinalFantasyFrame::~DecoratorFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

bool DecoratorFinalFantasyFrame::Initialise()
{
  return true;
}

Rocket::Core::DecoratorDataHandle DecoratorFinalFantasyFrame::GenerateElementData(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element))
{
  ROCKET_UNUSED(element);

  return Rocket::Core::Decorator::INVALID_DECORATORDATAHANDLE;
}

void DecoratorFinalFantasyFrame::ReleaseElementData(Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  ROCKET_UNUSED(element_data);
}

void DecoratorFinalFantasyFrame::RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  ROCKET_UNUSED(element_data);

  Rocket::Core::Vector2f position = element->GetAbsoluteOffset(Rocket::Core::Box::PADDING);
  Rocket::Core::Vector2f size = element->GetBox().GetSize(Rocket::Core::Box::PADDING);

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "position:", position.x, position.y);
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "size:", size.x, size.y);

  // TODO: Optimization (?); determine render state; dirty means we need to
  // update our decorator, such as when the element is resized or moved...
  //  For starters: a) ensure float math interface functions for Point2f &&
  // Size2f; b) translation from GL coordinates to pixel.
  //
  // ...or...
  //
  // Probably more productive to just optimize our gradient rendering class and
  // call it quits!

  nom::RocketSDL2Renderer* p = NOM_DYN_PTR_CAST( nom::RocketSDL2Renderer*, Rocket::Core::GetRenderInterface() );

  if( p )
  {
    const RenderWindow* target = p->window_;

    Point2i pos( position.x, position.y );
    Size2i dims( size.x, size.y );

    decorator_->set_bounds( pos, dims );

    // FIXME: We shouldn't need to do this -- ::set_bounds ought to take care
    // of internal updating...
    // decorator_->update();

    decorator_->invalidate();

    decorator_->draw( *target );
  }
}

} // namespace nom
