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
#ifndef NOMLIB_LIBROCKET_UICONTEXT_EVENT_HANDLER_HPP
#define NOMLIB_LIBROCKET_UICONTEXT_EVENT_HANDLER_HPP

#include <Rocket/Core/Input.h>

#include "nomlib/config.hpp"
#include "nomlib/gui/IUIEventHandler.hpp"

namespace nom {

// Forward declarations
class UIContext;
struct Event;

/// \brief The default event handler interface for a nom::UIContext
class UIContextEventHandler: public IUIEventHandler
{
  public:
    typedef UIContextEventHandler self_type;
    typedef self_type* raw_ptr;

    /// \brief Default constructor.
    ///
    /// \params ctx An object pointer to the context utilizing this instance.
    UIContextEventHandler( UIContext* ctx );

    /// \brief Destructor.
    virtual ~UIContextEventHandler();

    /// \brief Injection of libRocket's event loop.
    ///
    /// \remarks Implements IUIEventHandler::process_event.
    virtual void process_event( const Event& ev );

  protected:
    virtual Rocket::Core::Input::KeyIdentifier translate_key( const Event& ev );
    virtual int translate_mouse_button(const Event& ev);

    /// \returns Positive X, Y axis wheel values are translated from SDL2 to
    /// libRocket as the 'up' direction in libRocket and negative X, Y axis
    /// wheel values are translated to libRocket as the 'down' direction.
    ///
    /// \remarks libRocket uses negative wheel values as the 'up' direction
    /// (away from the end-user), and positive wheel values as the 'down'
    /// direction (towards the end-user). This is indeed the inverse of how
    /// SDL2 handles the values.
    virtual int translate_mouse_wheel(const Event& ev);

    virtual int translate_key_modifiers( const Event& ev );

  private:
    UIContext* ctx_;
};

} // namespace nom

#endif // include guard defined
