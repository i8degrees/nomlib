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
#ifndef NOMLIB_GUI_UIEVENT_LISTENER_HPP
#define NOMLIB_GUI_UIEVENT_LISTENER_HPP

#include <functional>

#include <Rocket/Core/EventListener.h>
// #include <Rocket/Core/Types.h>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Interface for an event listener for libRocket's platform.
///
/// \note Observer pattern
///
/// \note This object must outlive its element's destruction.
///
/// \see http://librocket.com/wiki/documentation/C%2B%2BManual/Events
/// \see http://www.librocket.com/wiki/documentation/PythonManual/AttachingToEvents
class UIEventListener: public Rocket::Core::EventListener
{
  public:
    typedef UIEventListener self_type;
    typedef Rocket::Core::Event event_type;
    typedef std::function<void(event_type&)> callback_type;

    /// \brief Register an event listener for an element.
    ///
    /// \param observer The callback to be executed when the element has an
    /// event.
    UIEventListener(const callback_type& observer);

    /// \brief Destructor.
    virtual ~UIEventListener();

    /// \brief Not used.
    virtual void OnAttach(Rocket::Core::Element* element);

    /// \brief Not used.
    virtual void OnDetach(Rocket::Core::Element* element);

  protected:
    /// \brief The element's event handler.
    ///
    /// \remarks The end-user's callback function is executed here; this feature
    /// is provided by nomlib.
    ///
    /// \note This method is called by libRocket internally when there is an
    /// event for the element ready to be processed.
    virtual void ProcessEvent(event_type& event);

  private:
    callback_type observer_;
};

} // namespace nom

#endif // include guard defined
