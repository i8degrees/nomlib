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
#include <iostream>
#include <string>
#include <fstream>

#include "nomlib/tests/common.hpp"

#include "nomlib/serializers.hpp"
#include "nomlib/system.hpp"

namespace nom {

const std::string expected_output = "\n<html>\n\t<head>\n\t\t<title>Hello, world!</title>\n\t</head>\n\t<body>\n\t\t<p>Hello, there!</p>\n\t\t<img src=\"http://www.w3.org/Icons/valid-xhtml11\" alt=\"Valid XHTML 1.1\" width=\"88\" height=\"31\"/>\n\t</body>\n</html>";

void generate_output( HTMLElement& doc )
{
  doc = HTMLElement( "html" );

  HTMLElement* head = doc.append_element( "head" );
  head->append_element( "title" )->append_text( "Hello, world!" );

  HTMLElement* body = doc.append_element( "body" );
  body->append_element( "p" )->append_text( "Hello, there!" );

  HTMLElement* img = body->append_element( "img" );

  // Not really valid XHTML -- this was just the first image that came to mind :]
  img->append_attribute( "src", "http://www.w3.org/Icons/valid-xhtml11" );
  img->append_attribute( "alt", "Valid XHTML 1.1" );
  img->append_attribute( "width", "88" );
  img->append_attribute( "height", "31" );

  EXPECT_EQ( expected_output, doc.print() );
}

TEST( MiniHTMLTest, HelloWorld )
{
  HTMLElement doc;
  generate_output( doc );
}

TEST( MiniHTMLTest, OutputHelloWorld )
{
  File file;
  HTMLElement doc;

  generate_output( doc );

  std::ofstream fp;
  fp.open( "hello.html" );
  fp << doc.print();
  fp.close();

  EXPECT_EQ( true, file.is_file( "hello.html" ) );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
