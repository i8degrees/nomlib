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
#include "nomlib/system/resource_handlers.hpp"

// Private headers
#include "nomlib/system/ResourceFile.hpp"
#include "nomlib/graphics/fonts/Font.hpp"
#include "nomlib/graphics/fonts/BitmapFont.hpp"
#include "nomlib/graphics/fonts/TrueTypeFont.hpp"

namespace nom {

void create_font( const ResourceFile& res, Font& font )
{
  if( res.type() == ResourceFile::Type::BitmapFont )
  {
    font = Font( std::shared_ptr<IFont>( new BitmapFont() ) );

    // Do not make cloned copies of the font; cloning is only necessary for
    // fonts that do not need to share data, such as point size, which is not
    // yet implemented.
    font.set_sharable( false );
  }
  else if( res.type() == ResourceFile::Type::TrueTypeFont )
  {
    font = Font( std::shared_ptr<IFont>( new TrueTypeFont() ) );

    // We need clones of the font, because otherwise we'd be sharing point size,
    // etc. when the same font is used in multiple objects.
    font.set_sharable( true );
  }
}

} // namespace nom
