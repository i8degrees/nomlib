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
#ifndef NOMLIB_GUI_UI_EVENT_CALLBACK_HPP
#define NOMLIB_GUI_UI_EVENT_CALLBACK_HPP

#include <functional>

#include "nomlib/config.hpp"
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

class UIEventCallback
{
  public:
    typedef UIEventCallback SelfType;
    typedef SelfType* RawPtr;

    typedef UIWidgetEvent event_type;
    typedef std::function<void(event_type&)> ValueType;

    /// \brief Default constructor.
    UIEventCallback( void );

    /// \brief Destructor.
    ~UIEventCallback( void );

    /// \brief Construct an object and initialize its callback (object method).
    UIEventCallback( const ValueType& callback );

    /// \brief Copy constructor.
    UIEventCallback( const SelfType& copy );

    /// \brief Copy assignment operator.
    SelfType& operator =( const SelfType& other );

    /// \brief Obtain a pointer to the object.
    RawPtr get( void );

    /// \brief Query the validity of the object.
    bool valid( void ) const;

    /// \brief Obtain a reference to the assigned callback object method.
    const ValueType& method( void ) const;

    /// \brief C++ functor; execute the assigned object method.
    void operator() ( event_type& ev ) const;

    /// \brief Convenience method for executing the assigned callback object
    /// method.
    void execute( event_type& ev );

  private:
    ValueType method_;
};

} // namespace nom

#endif // include guard defined
