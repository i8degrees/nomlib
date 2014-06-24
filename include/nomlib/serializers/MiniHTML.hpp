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
#ifndef NOMLIB_SERIALIZERS_MINI_HTML_HPP
#define NOMLIB_SERIALIZERS_MINI_HTML_HPP

#include <iostream>
#include <string>
#include <memory>
#include <list>
#include <sstream>

#include "nomlib/config.hpp"
#include "nomlib/serializers/serializers_config.hpp"

namespace nom {

/// \brief The following is a very simple representation of the HTML DOM.
/// It's sole purpose is outputting formatted html documents, there
/// are no provisions for traversal, parsing, etc.
class HTMLNode
{
  public:
    HTMLNode( void );
    virtual ~HTMLNode( void );

    virtual std::string print( const std::string& indent = "" ) = 0;
};

/// \brief A plain text node (cannot have children, text only).
class HTMLTextNode: public HTMLNode
{
  public:
    HTMLTextNode( const std::string& text );

    virtual ~HTMLTextNode( void );

    virtual std::string print( const std::string& indent = "" );

  private:
    std::string contents_;
};

/// \brief An HTML element, can contain children (either text or other
/// elements), as well as storing a list of attributes.
class HTMLElement: public HTMLNode
{
  public:
    HTMLElement( void );

    virtual ~HTMLElement( void );

    HTMLElement( const std::string& tag );

    void append_attribute( const std::string& name, const std::string& value );

    void append_child( HTMLNode* node );

    HTMLElement* append_element( const std::string& type );

    HTMLTextNode* append_text( const std::string& text );

    virtual std::string print( const std::string& indent = "" );

  private:
    std::string tag_name_;
    std::list<std::pair<std::string, std::string>> attributes_;
    std::list<HTMLNode*> children_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::MiniHTML
/// \ingroup serializers
///
/// Minimal HTML output interface
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
