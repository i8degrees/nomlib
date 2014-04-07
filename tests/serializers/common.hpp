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
#include <nomlib/ptree.hpp>   // Property Tree (nom::Value)
#include <nomlib/system.hpp>

namespace nom {

/// Relative file path name to our resources for this unit test
const nom::Path p;

/// Relative file path name of the resources directory for JSON input
const std::string APP_RESOURCES_JSON_DIR = "Resources" + p.native() + "json" + p.native();

/// Relative file path name of the resources directory for XML input
const std::string APP_RESOURCES_XML_DIR = "Resources" + p.native() + "xml" + p.native();

const std::string RESOURCE_SANITY = APP_RESOURCES_JSON_DIR + "sanity.json";
const std::string RESOURCE_SANITY2 = APP_RESOURCES_JSON_DIR + "sanity2.json";

/// \brief World of Warcraft auction house data (JSON)
const std::string RESOURCE_JSON_AUCTIONS = APP_RESOURCES_JSON_DIR + "auctions.json";

/// \brief World of Warcraft inventory dump (JSON)
const std::string RESOURCE_JSON_INVENTORY = APP_RESOURCES_JSON_DIR + "inventory.json";

/// \FIXME Only this particular file, for whatever reason, refuses to open unless we call Json::Reader::parse directly
const std::string RESOURCE_JSON_GAMEDATA = APP_RESOURCES_JSON_DIR + "omnom.json";

const std::string RESOURCE_XML = APP_RESOURCES_XML_DIR + "test_one.xml";
const std::string RESOURCE_JSON_BMFONT_GAMEOVER = APP_RESOURCES_XML_DIR + "gameover.json";
const std::string RESOURCE_XML_BMFONT_GAMEOVER = APP_RESOURCES_XML_DIR + "gameover.xml";

/// \brief Adapted from the original sanity2.json
Value sanity2_out( void );

void CheckObjectValues_NullValue( /*const*/ Value& o );
void CheckObjectValues_IntegerValue( /*const*/ Value& o );

} // namespace nom
