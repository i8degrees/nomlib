/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include <nomlib/system/OSX/ResourcePath.hpp>

namespace nom {

const std::string getBundleResourcePath ( const std::string& identifier )
{
  char resources_path [ PATH_MAX ]; // file-system path
  CFBundleRef bundle; // bundle type reference

  // Look for a bundle using its identifier if string passed is not null
  // terminated
  if ( identifier != "\0" )
  {
    CFStringRef identifier_ref; // Apple's string type

    identifier_ref = CFStringCreateWithCString  ( nullptr, identifier.c_str(),
                                                  strlen ( identifier.c_str() )
                                                );

    bundle = CFBundleGetBundleWithIdentifier ( identifier_ref );
  }
  else // Assume that we are looking for the top-level bundle's Resources path
  {
    bundle = CFBundleGetMainBundle();
  }

  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL ( bundle );

  if ( ! CFURLGetFileSystemRepresentation ( resourcesURL, true, ( uint8* ) resources_path, PATH_MAX ) )
  {
NOM_LOG_ERR ( "Could not obtain the bundle's Resources path." );

    CFRelease ( resourcesURL );

    return "\0";
  }

  CFRelease ( resourcesURL );

  return resources_path;
}


} // namespace nom
