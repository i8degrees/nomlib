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
#include "nomlib/librocket/DecoratorInstancerSprite.hpp"

// Private headers
#include "nomlib/librocket/DecoratorSpriteBatch.hpp"

namespace nom {

DecoratorInstancerSprite::DecoratorInstancerSprite()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  RegisterProperty( "sheet-src", "" ).AddParser("string");
  RegisterProperty( "sheet-image-src", "" ).AddParser("string");
  RegisterProperty( "sheet-frame", "0" ).AddParser("number");
}

DecoratorInstancerSprite::~DecoratorInstancerSprite()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

Rocket::Core::Decorator* DecoratorInstancerSprite::InstanceDecorator(const Rocket::Core::String& ROCKET_UNUSED_PARAMETER(name), const Rocket::Core::PropertyDictionary& properties )
{
  ROCKET_UNUSED(name);

  const Rocket::Core::Property* sheet_source_property = properties.GetProperty("sheet-src");
  std::string sheet_source = sheet_source_property->Get<Rocket::Core::String>().CString();

  const Rocket::Core::Property* sheet_image_source_property = properties.GetProperty("sheet-image-src");
  std::string sheet_image_source = sheet_image_source_property->Get<Rocket::Core::String>().CString();

  const Rocket::Core::Property* sheet_frame_property = properties.GetProperty("sheet-frame");
  int sheet_frame = sheet_frame_property->Get<int>();

  DecoratorSpriteBatch* decorator = new DecoratorSpriteBatch();

  if( decorator->initialize( sheet_source, sheet_image_source, sheet_frame, sheet_source_property->source.CString() ) )
  {
    return decorator;
  }

  // Err
  decorator->RemoveReference();
  ReleaseDecorator(decorator);

  return nullptr;
}

void DecoratorInstancerSprite::ReleaseDecorator(Rocket::Core::Decorator* decorator)
{
  delete decorator;
}

void DecoratorInstancerSprite::Release()
{
  delete this;
}

} // namespace nom
