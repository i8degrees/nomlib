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
#include "nomlib/librocket/UIDataViewList.hpp"

// Private headers (third-party)
#include <Rocket/Core/ElementUtilities.h>

using namespace Rocket::Core;

namespace nom {

UIDataViewList::UIDataViewList()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_selection( 0 );
}

UIDataViewList::~UIDataViewList()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

std::string UIDataViewList::column_title( int col ) const
{
  ElementList tags;

  NOM_ASSERT( this->valid() != false );
  if( this->valid() == true )
  {
    Element* parent = this->document()->GetElementById("content");
    NOM_ASSERT( parent != nullptr );
    if( parent )
    {
      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, parent, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );
      if( col < tags.size() )
      {
        return tags[col]->GetInnerRML().CString();
      }
    }
  }

  // ..No title found?
  return "\0";
}

bool UIDataViewList::set_column_title( int col, const std::string& name )
{
  ElementList tags;

  NOM_ASSERT( this->valid() != false );
  if( this->valid() == true )
  {
    Element* parent = this->document()->GetElementById("content");
    NOM_ASSERT( parent != nullptr );
    if( parent )
    {
      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, parent, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );
      if( col < tags.size() )
      {
        tags[col]->SetInnerRML( name.c_str() );
        return true;
      }
    }
  }

  return false;
}

// curr_page * per_page + selection
int UIDataViewList::selection() const
{
  return this->selection_;
}

void UIDataViewList::set_selection( int idx )
{
  this->selection_ = idx;
}

} // namespace nom
