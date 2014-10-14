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
#include "nomlib/gui/DecoratorInstancerFinalFantasyFrame.hpp"

// #include <Rocket/Core/Math.h>
// #include <Rocket/Core/String.h>
#include "nomlib/gui/DecoratorFinalFantasyFrame.hpp"

namespace nom {

DecoratorInstancerFinalFantasyFrame::DecoratorInstancerFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  // RegisterProperty( "image-src", "" ).AddParser("string");

  // TODO: nom::Gradient instantiation
  // RegisterProperty("start-color", "").AddParser("colour");
  // RegisterProperty("end-color", "").AddParser("colour");
  // FinalFantasyFrame instantiation: outer-border, inner-border
}

DecoratorInstancerFinalFantasyFrame::~DecoratorInstancerFinalFantasyFrame()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

Rocket::Core::Decorator* DecoratorInstancerFinalFantasyFrame::InstanceDecorator(const Rocket::Core::String& ROCKET_UNUSED_PARAMETER(name), const Rocket::Core::PropertyDictionary& ROCKET_UNUSED_PARAMETER(properties) )
{
  ROCKET_UNUSED(name);
  ROCKET_UNUSED(properties);

  // const Rocket::Core::Property* scolor_prop = properties.GetProperty("start-color");
  // Rocket::Core::Colour<byte,255> color = scolor_prop->Get< Rocket::Core::Colour >();
  // NOM_DUMP( color.red, color.g, color.b, color.a );

  DecoratorFinalFantasyFrame* decorator = new DecoratorFinalFantasyFrame();
  // if( decorator->Initialise( scolor_prop, scolor_prop->source ) )
  if( decorator->Initialise() )
  {
    return decorator;
  }

  decorator->RemoveReference();
  ReleaseDecorator(decorator);

  return nullptr;
}

void DecoratorInstancerFinalFantasyFrame::ReleaseDecorator(Rocket::Core::Decorator* decorator)
{
  delete decorator;
}

void DecoratorInstancerFinalFantasyFrame::Release()
{
  delete this;
}

} // namespace nom
