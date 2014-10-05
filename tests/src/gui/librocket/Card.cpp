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
#include "nomlib/tests/gui/librocket/Card.hpp"

/// \brief Disable NOM_ASSERT macros so that they do not interfere with tests
/// that check for failure conditions; i.e.: libRocketDataGridTest.
#if defined( NOM_ASSERT )
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr)
#endif // defined NOM_ASSERT

namespace nom {

const Card Card::null = Card();

Card::Card() :
  id_(-1),
  name_("\0"),
  num_(0)
{
}

Card::~Card()
{
}

Card::Card( int id, const std::string& name, int num_cards )
{
  this->set_id(id);
  this->set_name(name);
  this->set_num( num_cards );
}

int Card::id() const
{
  return this->id_;
}

const std::string& Card::name() const
{
  return this->name_;
}

int Card::num() const
{
  return this->num_;
}

void Card::set_id(int id)
{
  this->id_ = id;
}

void Card::set_name(const std::string& name)
{
  this->name_ = name;
}

void Card::set_num(int num)
{
  this->num_ = num;
}

} // namespace nom
