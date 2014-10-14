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
#include "nomlib/gui/DOM.hpp"

namespace nom {

DOM::DOM( Rocket::Core::ElementDocument* model )
{
  this->doc_ = model;
}

Rocket::Core::Element* DOM::id( const std::string& id ) const
{
  NOM_ASSERT( this->doc_ != nullptr );

  Rocket::Core::Element* element = this->doc_->GetElementById( id.c_str() );

  NOM_ASSERT( element != nullptr );

  return element;
}

Rocket::Core::ElementList DOM::ids( const std::string& id ) const
{
  Rocket::Core::ElementList tags;

  NOM_ASSERT( this->doc_ != nullptr );

  Rocket::Core::Element* element = this->doc_->GetElementById( id.c_str() );

  NOM_ASSERT( element != nullptr );

  tags.push_back( element );

  return tags;
}

Rocket::Core::ElementList DOM::class_names( const std::string& q ) const
{
  Rocket::Core::ElementList tags;
  Rocket::Core::ElementList tags_output;
  std::string tag_name;
  std::string class_name;

  nom::size_type pos = q.find_first_of(".");

  if( pos != std::string::npos ) // match found
  {
    tag_name = q.substr( 0, pos );
  }

  if( q.find_last_of(".") != std::string::npos ) // match found
  {
    class_name = q.substr( q.find_last_of(".") + 1 );
  }

  // NOM_DUMP(q);
  // NOM_DUMP(tag_name);
  // NOM_DUMP(class_name);

  Rocket::Core::ElementUtilities::GetElementsByTagName( tags, this->doc_->GetParentNode(), tag_name.c_str() );

  for( auto itr = tags.begin(); itr != tags.end(); ++itr )
  {
    if( (*itr) && (*itr)->GetClassNames().Find( class_name.c_str() ) != nom::npos )
    {
      tags_output.push_back( (*itr) );
    }
  }

  return tags_output;
}

void DOM::set_rml( Rocket::Core::ElementList elements, const std::string& text )
{
  for( auto itr = elements.begin(); itr != elements.end(); ++itr )
  {
    (*itr)->SetInnerRML( text.c_str() );
  }
}

} // namespace nom
