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
#include "nomlib/gui/UILayoutPolicy.hpp"

namespace nom {

UILayoutPolicy::UILayoutPolicy( void ) :
  h_policy_{ Policy::Preferred },
  v_policy_{ Policy::Preferred }
{
  // NOM_LOG_TRACE( NOM );
}

UILayoutPolicy::~UILayoutPolicy( void )
{
  // NOM_LOG_TRACE( NOM );
}

UILayoutPolicy::UILayoutPolicy( uint32 h_policy, uint32 v_policy )
{
  // NOM_LOG_TRACE( NOM );

  this->set_horizontal_policy( h_policy );
  this->set_vertical_policy( v_policy );
}

Orientations UILayoutPolicy::directions( void ) const
{
  return this->directions_;
}

uint32 UILayoutPolicy::horizontal_policy( void ) const
{
  return this->h_policy_;
}

uint32 UILayoutPolicy::vertical_policy( void ) const
{
  return this->v_policy_;
}

int UILayoutPolicy::horizontal_stretch( void ) const
{
  return this->h_stretch_;
}

int UILayoutPolicy::vertical_stretch( void ) const
{
  return this->v_stretch_;
}

void UILayoutPolicy::set_horizontal_policy( uint32 flags )
{
  this->h_policy_ = flags;
}

void UILayoutPolicy::set_vertical_policy( uint32 flags )
{
  this->v_policy_ = flags;
}

void UILayoutPolicy::set_horizontal_stretch( int factor )
{
  this->h_stretch_ = factor;
}

void UILayoutPolicy::set_vertical_stretch( int factor )
{
  this->v_stretch_ = factor;
}

void UILayoutPolicy::swap( void )
{
  uint32 h_data = this->horizontal_policy();
  uint32 v_data = this->vertical_policy();

  int h_stretch = this->horizontal_stretch();
  int v_stretch = this->vertical_stretch();

  this->set_horizontal_policy( h_data );
  this->set_vertical_policy( v_data );

  this->set_horizontal_stretch( h_stretch );
  this->set_horizontal_stretch( v_stretch );
}

bool UILayoutPolicy::operator ==( const self_type& rhs ) const
{
  return( this->h_policy_ == rhs.h_policy_    &&  this->v_policy_ == rhs.v_policy_
                                              &&
          this->h_stretch_ == rhs.h_stretch_  && this->v_stretch_ == rhs.v_stretch_
                                              &&
          this->directions_ == rhs.directions_
        );

}

bool UILayoutPolicy::operator !=( const self_type& rhs ) const
{
  return! ( this->h_policy_ == rhs.h_policy_    &&  this->v_policy_ == rhs.v_policy_
                                                &&
            this->h_stretch_ == rhs.h_stretch_  && this->v_stretch_ == rhs.v_stretch_
                                                &&
            this->directions_ == rhs.directions_
        );
}

} // namespace nom
