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
#include "nomlib/ptree/VString.hpp"

#include "nomlib/core/helpers.hpp"

namespace nom {

VString::VString()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->index_ = 0;
}

VString::~VString()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);
}

VString::VString(const char* key)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->cstr_ = std::string(key);
  this->index_ = 0;
}

VString::VString(const std::string& key)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->cstr_ = key;
  this->index_ = 0;
}

VString::VString(ArrayIndex index)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->index_ = index;
}

VString::VString(const self_type& rhs)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE, NOM_LOG_PRIORITY_VERBOSE);

  this->cstr_ = rhs.cstr_;
  this->index_ = rhs.index_;
}

VString::self_type& VString::operator =(const self_type& rhs)
{
  VString copy(rhs);
  this->swap(copy);

  return *this;
}

void VString::swap(VString& rhs)
{
  std::swap(this->cstr_, rhs.cstr_);
  std::swap(this->index_, rhs.index_);
}

// TODO: Try to use a more efficient sorting algorithm for string comparisons?
bool VString::operator <(const self_type& rhs) const
{
  if( this->cstr_ != "" && rhs.cstr_ != "" ) {
    return( nom::compare_string(this->cstr_, rhs.cstr_) < 0);
  } else {
    return(this->index_ < rhs.index_);
  }
}

// TODO: Try to use a more efficient sorting algorithm for string comparisons?
bool VString::operator ==(const self_type& rhs) const
{
  if( this->cstr_ != "" && rhs.cstr_ != "" ) {
    return( nom::compare_string(this->cstr_, rhs.cstr_) == 0);
  } else {
    return(this->index_ == rhs.index_);
  }
}

std::string VString::string() const
{
  return this->cstr_;
}

const char* VString::c_str() const
{
  return this->cstr_.c_str();
}

ArrayIndex VString::index() const
{
  return this->index_;
}

} // namespace nom
