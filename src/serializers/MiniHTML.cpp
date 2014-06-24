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
#include "nomlib/serializers/MiniHTML.hpp"

namespace nom {

HTMLNode::HTMLNode( void )
{
  // NOM_LOG_TRACE( NOM );
}

HTMLNode::~HTMLNode( void )
{
  // NOM_LOG_TRACE( NOM );
}

HTMLTextNode::HTMLTextNode( const std::string& text ) :
  contents_{ text }
{
  // NOM_LOG_TRACE( NOM );
}

HTMLTextNode::~HTMLTextNode( void )
{
  // NOM_LOG_TRACE( NOM );
}

std::string HTMLTextNode::print( const std::string& indent )
{
  return this->contents_;
}

HTMLElement::HTMLElement( void )
{
  // NOM_LOG_TRACE( NOM );
}

HTMLElement::~HTMLElement( void )
{
  // NOM_LOG_TRACE( NOM );

  while( this->children_.empty() == false )
  {
    NOM_DELETE_PTR( this->children_.back() );
    this->children_.pop_back();
  }
}

HTMLElement::HTMLElement( const std::string& tag )
{
  this->tag_name_ = tag;
}

void HTMLElement::append_attribute  (
                                      const std::string& name,
                                      const std::string& value
                                    )
{
  std::pair<std::string, std::string> pair( name, value );

  this->attributes_.push_back( pair );
}

void HTMLElement::append_child( HTMLNode* node )
{
  NOM_ASSERT( node != nullptr );

  if( node != nullptr )
  {
    this->children_.push_back( node );
  }
}

HTMLElement* HTMLElement::append_element( const std::string& type )
{
  HTMLElement* node = new HTMLElement( type );

  // NOM_ASSERT( node != nullptr );

  this->children_.push_back( node );

  return node;
}

HTMLTextNode* HTMLElement::append_text( const std::string& text )
{
  HTMLTextNode* node = new HTMLTextNode( text );

  // NOM_ASSERT( node != nullptr );

  this->children_.push_back( node );

  return node;
}

std::string HTMLElement::print( const std::string& indent )
{
  // stream we'll output to
  std::stringstream out;

  out << "\n" << indent;
  // out << indent;

  // print opening tag
  // open bracket and name
  out << "<" << this->tag_name_;

  // attributes
  for ( auto itr = this->attributes_.begin(); itr != this->attributes_.end(); ++itr )
  {
    // name="value"
    // out << " " << (*itr).first << "=\"" << (*itr).second << "\"";
    out << " " << (*itr).first << "=\"" << (*itr).second << "\"";
  }

  // self-closing is done here
  if( this->children_.empty() == true )
  {
      out << "/>";
      return out.str();
  }

  // closing bracket
  out << ">";

  // print children
  for( auto itr = this->children_.begin(); itr != this->children_.end(); ++itr )
  {
    out << (*itr)->print( indent + "\t" );
    // out << (*itr)->print( indent );
  }

  // if the last child was an actual element start a newline
  // (otherwise, if text, we'll close on the same line)
  if( dynamic_cast<HTMLElement*>( this->children_.back() ) )
  {
    out << "\n" << indent;
    // out << indent;
  }

  // print closing tag
  out << "</" << this->tag_name_ << ">";

  return out.str();
}

} // namespace nom
