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
#ifndef NOMLIB_GUI_DATAVIEW_ITEM_HPP
#define NOMLIB_GUI_DATAVIEW_ITEM_HPP

#include <memory>

#include "nomlib/config.hpp"

namespace nom {

template <typename T>
class DataViewItem
{
  public:
    typedef DataViewItem SelfType;

    typedef SelfType* RawPtr;
    typedef std::unique_ptr<SelfType> UniquePtr;
    typedef std::shared_ptr<SelfType> SharedPtr;

    DataViewItem( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    virtual ~DataViewItem( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    DataViewItem( const SelfType& copy ) :
      data_{ copy.data() }
    {
      // NOM_LOG_TRACE( NOM );
    }

    DataViewItem( const T& data )
    {
      this->data_ = data;
    }

    const T& data( void ) const
    {
      return this->data_;
    }

  private:
    T data_;
};

} // namespace nom

#endif // include guard defined
