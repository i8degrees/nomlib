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
#include "nomlib/xml/RapidXmlSerializer.hpp"

namespace nom {

RapidXmlSerializer::RapidXmlSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

RapidXmlSerializer::~RapidXmlSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

bool RapidXmlSerializer::serialize( const Value& source, const std::string& output ) const
{
  NOM_STUBBED( NOM );

  return true;
}

bool RapidXmlSerializer::unserialize( const std::string& input, Value& dest ) const
{
  std::stringstream buffer;
  std::string xml_doc;
  std::ifstream fp;

  rapidxml::xml_document<> doc;

  fp.open( input );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "File access failure on file: " + input );
    return false;
  }

  buffer << fp.rdbuf();
  xml_doc = std::string( buffer.str() );
// NOM_DUMP(xml_doc);
  doc.parse<0>( &xml_doc[0] );

NOM_DUMP( doc.first_node()->name() );

  rapidxml::xml_node<> *root_node = doc.first_node();

  for( rapidxml::xml_node<> *nodes = root_node->first_node(); nodes; nodes = nodes->next_sibling() )
  {
    std::string node = nodes->name();
    NOM_DUMP(node);

    if( nodes->parent() )
    {
      for( rapidxml::xml_node<> *children = nodes->first_node(); children; children = children->next_sibling() )
      {
        std::string node = children->name();
        NOM_DUMP(node);
        NOM_DUMP( children->type() );

        if( children->parent() )
        {
          // NOM_DUMP( children->name() );
          NOM_DUMP( children->type() );
          for( rapidxml::xml_node<> *childrens = children->first_node(); childrens; childrens = childrens->next_sibling() )
          {
            if( childrens->parent() )
            {
              NOM_DUMP( childrens->name() );
              // NOM_DUMP( childrens->value() );
              NOM_DUMP( childrens->type() );

              for( rapidxml::xml_attribute<> *attr = childrens->first_attribute(); attr; attr = attr->next_attribute() )
              {
                NOM_DUMP( attr->name() );
                NOM_DUMP( attr->value() );
              }

              for( rapidxml::xml_node<> *childrenss = childrens->first_node(); childrenss; childrenss = childrenss->next_sibling() )
              {
                NOM_DUMP( childrenss->name() );
                NOM_DUMP( childrenss->value() );
                NOM_DUMP( childrenss->type() );

                /*
                for( rapidxml::xml_attribute<> *attr = childrenss->first_attribute(); attr; attr = attr->next_attribute() )
                {
                  NOM_DUMP( attr->name() );
                  NOM_DUMP( attr->value() );
                }
                */
              }
            }
          }
        }
      }
    }
  }

  return true;
}

} // namespace nom
