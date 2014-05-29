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
#include "nomlib/system/PlatformSettings.hpp"

// Private headers
#include "nomlib/system/File.hpp"
#include "nomlib/system/Path.hpp"
#include "nomlib/system/resource_handlers.hpp"

// Forward declarations
#include "nomlib/graphics/fonts/Font.hpp"

namespace nom {

// Static initialization
bool PlatformSettings::initialized_ = false;

bool PlatformSettings::initialized( void )
{
  return PlatformSettings::initialized_;
}

void PlatformSettings::initialize( void )
{
  PlatformSettings::enumerate_fonts();

  PlatformSettings::initialized_ = true;
}

Color4i PlatformSettings::get_system_color( SystemColorType index )
{
  switch( index )
  {
    default:
    {
      return Color4i::White;
      break;
    }

    case ButtonLabelText:
    {
      return Color4i::Black;
      break;
    }
  } // end switch index
}

Font* PlatformSettings::get_system_font( SystemFontType index )
{
  if( PlatformSettings::initialized() == false )
  {
    PlatformSettings::enumerate_fonts();
  }

  switch( index )
  {
    case FixedBitmap:
    case VariableBitmap:
    {
      return SystemFonts::cache().load_resource( "VIII" );

      break;
    }

    case FixedTrueType:
    {
      return SystemFonts::cache().load_resource( "Monaco" );

      break;
    }

    default:
    case VariableTrueType:
    {
      return SystemFonts::cache().load_resource( "LucidaGrande" );

      break;
    }
  }

  return nullptr;
}

Font* PlatformSettings::find_system_font( const std::string& key )
{
  if( PlatformSettings::initialized() == false )
  {
    PlatformSettings::enumerate_fonts();
  }

  return SystemFonts::cache().load_resource( key );
}

// Private scope

void PlatformSettings::enumerate_fonts( void )
{
  Path p;
  File fp;

  // This should have been called in nom::init and SDLApp::initialize (if the
  // class interface was used), but just in case...(no performance hit, so!)
  SystemFonts::initialize();

  SystemFonts::cache().set_resource_handler( [&] ( const ResourceFile& res, Font& font ) { create_font( res, font ); } );

  Path sys( "/System/Library/Fonts" );
  Path lib( "/Library/Fonts" );

  SystemFonts::cache().append_resource( ResourceFile( "Arial", lib.prepend("Arial.ttf"), ResourceFile::Type::TrueTypeFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "LucidaGrande", sys.prepend("LucidaGrande.ttc"), ResourceFile::Type::TrueTypeFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "MinionPro", lib.prepend("MinionPro-Regular.otf"), ResourceFile::Type::TrueTypeFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "Monaco", sys.prepend("Monaco.dfont"), ResourceFile::Type::TrueTypeFont ) );

  p = Path( fp.resource_path( "org.i8degrees.nomlib" ) + p.native() + "fonts" );

  SystemFonts::cache().append_resource( ResourceFile( "LiberationSans-Regular", p.prepend("LiberationSans-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "LiberationSerif-Regular", p.prepend("LiberationSerif-Regular.ttf"), ResourceFile::Type::TrueTypeFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "VIII", p.prepend("VIII.png"), ResourceFile::Type::BitmapFont ) );
  SystemFonts::cache().append_resource( ResourceFile( "VIII_small", p.prepend("VIII_small.png"), ResourceFile::Type::BitmapFont ) );

  // return true;
}

} // namespace nom
