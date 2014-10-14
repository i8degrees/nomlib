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
#include "nomlib/gui/DecoratorSpriteBatch.hpp"

// Forward declarations
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

// Private headers
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/gui/RocketFileInterface.hpp"
#include "nomlib/gui/RocketSDL2RenderInterface.hpp"

namespace nom {

DecoratorSpriteBatch::DecoratorSpriteBatch()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

DecoratorSpriteBatch::~DecoratorSpriteBatch()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

bool DecoratorSpriteBatch::initialize(  const std::string& sheet_src,
                                        const std::string& sheet_image_src,
                                        int sheet_frame,
                                        const std::string& rml_source )
{
  // Open the specified sprite sheet file for buffering
  Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();

  // Resolve the full image path for loading the sprite sheet's image source
  // from.
  //
  // This must be done because nom::Sprite & friends have no concept of a file
  // interface; file paths specified in RML and RCSS documents are relative to
  // the directory path of said document, not to the executable's working
  // directory path, as would be the usual case for us.
  std::string image_source_path = sheet_image_src;
  std::string sheet_source_path = sheet_src;

  RocketFileInterface* fs_root =
    NOM_DYN_PTR_CAST( RocketFileInterface*, file_interface );

  NOM_ASSERT( fs_root != nullptr );
  if( fs_root )
  {
    // Append root directory path that needs to be set at the time of libRocket
    // initialization (Rocket::Core::SetFileInterface)
    image_source_path = fs_root->root() += image_source_path;
    sheet_source_path = fs_root->root() += sheet_src;
  }

  this->sprite_.reset( new SpriteBatch() );

  if( this->sprite_->load_sheet_file(sheet_source_path) == false )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not load sprite sheet: ", sheet_source_path );
    return false;
  }

  if( this->sprite_->load( image_source_path ) == false )
  {
    // Invalid RCSS sprite-sheet-image-src?
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not load sheet image source:", image_source_path );
    return false;
  }

  this->sprite_->set_frame(sheet_frame);

  return true;
}

Rocket::Core::DecoratorDataHandle DecoratorSpriteBatch::GenerateElementData(Rocket::Core::Element* ROCKET_UNUSED_PARAMETER(element))
{
  return Rocket::Core::Decorator::INVALID_DECORATORDATAHANDLE;
}

void DecoratorSpriteBatch::ReleaseElementData(Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
}

void DecoratorSpriteBatch::RenderElement(Rocket::Core::Element* element, Rocket::Core::DecoratorDataHandle ROCKET_UNUSED_PARAMETER(element_data))
{
  Rocket::Core::Vector2f pos = element->GetAbsoluteOffset(Rocket::Core::Box::PADDING);

  nom::RocketSDL2RenderInterface* target = NOM_DYN_PTR_CAST( nom::RocketSDL2RenderInterface*, Rocket::Core::GetRenderInterface() );
  NOM_ASSERT( target != nullptr );
  if( target == nullptr ) return;

  const RenderWindow* context = target->window_;

  NOM_ASSERT( context != nullptr );
  if( context )
  {
    if( this->sprite_ )
    {
      this->sprite_->set_position( Point2i(pos.x, pos.y) );
      this->sprite_->draw( *context );
    }
  }
}

} // namespace nom
