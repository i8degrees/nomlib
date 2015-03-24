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
#include "nomlib/version.hpp"

// Private headers
#include <ctype.h>
#include "nomlib/revision.hpp"

namespace nom {

std::ostream& operator <<(std::ostream& os, const VersionInfo& v)
{
  os
  << v.major()
  << "."
  << v.minor()
  << "."
  << v.patch();

  return os;
}

bool operator ==(const VersionInfo& lhs, const VersionInfo& rhs)
{
  return( lhs.major() == rhs.major() &&
          lhs.minor() == rhs.minor() &&
          lhs.patch() == rhs.patch() );
}

bool operator !=(const VersionInfo& lhs, const VersionInfo& rhs)
{
  return !( lhs == rhs );
}

bool operator <(const VersionInfo& lhs, const VersionInfo& rhs)
{
  if( ( lhs.major() < rhs.major() ) ) {
    return true;
  } // major

  if( ( lhs.minor() < rhs.minor() ) ) {
    return true;
  } // minor

  if( ( lhs.patch() < rhs.patch() ) ) {
    return true;
  } // patch

  return false;
}

bool operator <=(const VersionInfo& lhs, const VersionInfo& rhs)
{
  if( ( lhs.major() <= rhs.major() ) ) {
    return true;
  } // major

  if( ( lhs.minor() <= rhs.minor() ) ) {
    return true;
  } // minor

  if( ( lhs.patch() <= rhs.patch() ) ) {
    return true;
  } // patch

  return false;
}

bool operator >(const VersionInfo& lhs, const VersionInfo& rhs)
{
  if( ( rhs.major() < lhs.major() ) ) {
    return true;
  } // major

  if( ( rhs.minor() < lhs.minor() ) ) {
    return true;
  } // minor

  if( ( rhs.patch() < lhs.patch() ) ) {
    return true;
  } // patch

  return false;
}

bool operator >=(const VersionInfo& lhs, const VersionInfo& rhs)
{
  if( ( rhs.major() <= lhs.major() ) ) {
    return true;
  } // major

  if( ( rhs.minor() <= lhs.minor() ) ) {
    return true;
  } // minor

  if( ( rhs.patch() <= lhs.patch() ) ) {
    return true;
  } // patch

  return false;
}

VersionInfo::VersionInfo() :
  major_(0),
  minor_(0),
  patch_(0)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

VersionInfo::~VersionInfo()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

VersionInfo::VersionInfo(int major, int minor, int patch ) :
  major_(major),
  minor_(minor),
  patch_(patch)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

int VersionInfo::major() const
{
  return this->major_;
}

int VersionInfo::minor() const
{
  return this->minor_;
}

int VersionInfo::patch() const
{
  return this->patch_;
}

std::string VersionInfo::version_string() const
{
  std::string major = std::to_string( this->major() );
  std::string minor = std::to_string( this->minor() );
  std::string patch = std::to_string( this->patch() );

  return( major + "." + minor + "." + patch );
}

bool VersionInfo::convert_version_string( const std::string& ver_string,
                                          VersionInfo& info)
{
  const char* str = ver_string.c_str();

  if( isdigit( NOM_SCAST(uchar, *str) ) == false ) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
    return false;
  }

  info.major_ = std::atoi(str);
  if(info.major_ < 0) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
    return false;
  }

  for(; *str && isdigit( NOM_SCAST(uchar, *str) ); ++str) {}

  if(*str != '.') {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
    return false;
  }
  ++str;

  if( isdigit( NOM_SCAST(uchar, *str) ) == false ) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
    return false;
  }

  info.minor_ = std::atoi(str);
  if(info.minor_ < 0) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
    return false;
  }

  for(; *str && isdigit( NOM_SCAST(uchar, *str) ); ++str) {}

  if(*str != 0) {

    if(*str != '.') {
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
      return false;
    }
    ++str;

    info.patch_ = std::atoi(str);
    if(info.patch_ < 0) {
      NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, "Invalid version format");
      return false;
    }
  }

  return true;
}

std::string revision()
{
  return GIT_REVISION;
}

} // namespace nom
