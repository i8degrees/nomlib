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
#include "nomlib/gui/DecoratorPhotograph.hpp"

#include <Rocket/Core/Math.h>
#include <Rocket/Core/Element.h>
#include <Rocket/Core/Texture.h>

// Private headers
#include <SDL_image.h>

#include "nomlib/gui/RocketSDL2RenderInterface.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"

// TODO: Clean up these headers
#include "nomlib/graphics/Image.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

DecoratorPhotograph::~DecoratorPhotograph()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

bool DecoratorPhotograph::Initialise(const Rocket::Core::String& image_source, const Rocket::Core::String& image_path )
{
  // this->image_idx = LoadTexture(image_source, image_path);
  // if ( this->image_idx == -1)
  // {
  //   return false;
  // }

  // return true;

  Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
  Rocket::Core::FileHandle file_handle = file_interface->Open( image_source );

  if( ! file_handle )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not obtain file handle for source:", image_source.CString() );
    return false;
  }

  file_interface->Seek(file_handle, 0, SEEK_END);
  nom::size_type buffer_size = file_interface->Tell(file_handle);
  file_interface->Seek(file_handle, 0, SEEK_SET);

  char* buffer = new char[buffer_size];
  file_interface->Read(buffer, buffer_size, file_handle);
  file_interface->Close(file_handle);

  nom::size_type i;
  for(i = image_source.Length() - 1; i > 0; i--)
  {
    if(image_source[i] == '.')
    {
      break;
    }
  }

  Rocket::Core::String extension = image_source.Substring(i+1, image_source.Length()-i);

  nom::Image img;
  img.initialize( IMG_LoadTyped_RW(SDL_RWFromMem(buffer, buffer_size), 1, extension.CString() ) );

  if( img.valid() )
  {
    this->image_.create( img );
    return true;
  }
  else
  {
    NOM_LOG_ERR( NOM, "Could not initialize Decorator; invalid texture." );
    return false;
  }
}

Rocket::Core::DecoratorDataHandle DecoratorPhotograph::GenerateElementData(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element))
{
  ROCKET_UNUSED(element);

  return Rocket::Core::Decorator::INVALID_DECORATORDATAHANDLE;
}

void DecoratorPhotograph::ReleaseElementData(Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  ROCKET_UNUSED(element_data);
}

void DecoratorPhotograph::RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  // ROCKET_UNUSED(element_data);

  // Rocket::Core::Vector2f pos_margins = element->GetAbsoluteOffset(Rocket::Core::Box::MARGIN);
  Rocket::Core::Vector2f pos = element->GetAbsoluteOffset(Rocket::Core::Box::PADDING);

  // Rocket::Core::Vector2f size = element->GetBox().GetSize(Rocket::Core::Box::PADDING);

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "position_margins:", pos_margins.x, pos_margins.y);
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "pos:", pos.x, pos.y);
  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_GUI, "size:", this->image_.size().w, this->image_.size().h );

  nom::RocketSDL2RenderInterface* p = NOM_DYN_PTR_CAST( nom::RocketSDL2RenderInterface*, Rocket::Core::GetRenderInterface() );

  if( p )
  {
    const RenderWindow* target = p->window_;

    this->image_.set_position( Point2i( pos.x, pos.y ) );
    // this->image_.set_position( Point2i( 0, 0 ) );
    this->image_.draw( target->renderer() );
  }
}

} // namespace nom

// #include "DecoratorInstancerPhotograph.hpp"
#include <Rocket/Core/Math.h>
#include <Rocket/Core/String.h>

namespace nom {

DecoratorInstancerPhotograph::DecoratorInstancerPhotograph()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);

  RegisterProperty( "image-src", "" ).AddParser("string");
}

DecoratorInstancerPhotograph::~DecoratorInstancerPhotograph()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_GUI, nom::NOM_LOG_PRIORITY_VERBOSE);
}

Rocket::Core::Decorator* DecoratorInstancerPhotograph::InstanceDecorator(const Rocket::Core::String& ROCKET_UNUSED_PARAMETER(name), const Rocket::Core::PropertyDictionary& properties )
{
  ROCKET_UNUSED(name);

  const Rocket::Core::Property* image_source_property = properties.GetProperty("image-src");
  Rocket::Core::String image_source = image_source_property->Get< Rocket::Core::String >();

  DecoratorPhotograph* decorator = new DecoratorPhotograph();
  if( decorator->Initialise( image_source, image_source_property->source ) )
  {
    return decorator;
  }

  decorator->RemoveReference();
  ReleaseDecorator(decorator);

  return nullptr;
}

void DecoratorInstancerPhotograph::ReleaseDecorator(Rocket::Core::Decorator* decorator)
{
  delete decorator;
}

void DecoratorInstancerPhotograph::Release()
{
  delete this;
}

} // namespace nom
