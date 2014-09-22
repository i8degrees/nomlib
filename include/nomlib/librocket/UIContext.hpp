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
#ifndef NOMLIB_LIBROCKET_UICONTEXT_HPP
#define NOMLIB_LIBROCKET_UICONTEXT_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"

// Forward declarations (third-party)
namespace Rocket {
namespace Core {

class Context;
class RenderInterface;
class ElementDocument;

} // namespace Core
} // namespace Rocket

namespace nom {

// Forward declarations
struct Event;
class IUIEventHandler;

/// \brief libRocket context abstraction
class UIContext
{
  public:
    typedef UIContext self_type;

    /// \brief Default constructor; initialize the visual debugger state to
    /// boolean FALSE.
    UIContext();

    /// \brief Destructor.
    virtual ~UIContext();

    /// \brief Free the resources (documents) used by the context followed by
    /// the context itself.
    ///
    /// \remarks This must be explicitly called by the application to properly
    /// free the memory.
    void shutdown();

    /// \brief Get the state of the visual debugger for the context.
    bool debugger_enabled() const;

    /// \brief Enable libRocket's visual debugger tool.
    void enable_debugger();

    /// \brief Disable libRocket's visual debugger tool.
    void disable_debugger();

    /// \brief Get libRocket's context.
    ///
    /// \remarks The returned pointer is managed by the engine, and must not be
    /// be freed by the end-user.
    ///
    /// \see UIContext::create_context.
    Rocket::Core::Context* context() const;

    /// \brief Create a new element context used by libRocket.
    ///
    /// \params interface The custom rendering interface to use, or NULL to use
    /// the default interface. The default interface **must** be provided by an
    /// explicit call to Rocket::Core::SetRenderInterface.
    ///
    /// \remarks libRocket **must** be initialized prior to making this call.
    ///
    /// \note The visual debugger must be explicitly enabled with a call to
    /// UIContext::enable_debugger.
    ///
    /// \see nom::init_librocket and nom::shutdown_librocket.
    /// \see nom::RocketSDL2RenderInterface.
    bool create_context(  const std::string& name, const Size2i& res,
                          Rocket::Core::RenderInterface* interface = nullptr );

    Rocket::Core::ElementDocument* load_mouse_cursor_file( const std::string& filename );

    bool load_font( const std::string& filename );

    void resize( const Size2i& dims );

    /// \brief Event handler for the context's instance.
    ///
    /// \remarks This should be called within the main loop, typically once per
    /// frame.
    void process_event( const Event& event );

    /// \remarks The boolean return from the context's ::Update method is
    /// currently ignored.
    void update();

    /// \remarks The boolean return from the context's ::Render method is
    /// currently ignored.
    void draw();

  private:
    /// \brief Active state of libRocket's visual debugger.
    bool debugger_;

    /// \brief The implementation's element context (used by libRocket).
    ///
    /// \remarks This is a pointer managed by libRocket and must not be freed.
    Rocket::Core::Context* context_;

    /// \brief Rendering bridge between libRocket and nomlib
    ///
    /// \remarks This is a pointer managed by libRocket and must not be freed.
    Rocket::Core::RenderInterface* renderer_;

    /// \brief The event handler for this context; automatically initialized to
    /// nom::UIContextEventHandler.
    std::shared_ptr<IUIEventHandler> evt_;
};

} // namespace nom

#endif // include guard defined
