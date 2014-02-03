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
#ifndef NOMLIB_GUI_UI_LAYOUT_POLICY_HPP
#define NOMLIB_GUI_UI_LAYOUT_POLICY_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/gui/gui_config.hpp"

namespace nom {

/// \brief Layout control flags for widgets.
///
/// \note http://doc.qt.digia.com/4.6/qsizepolicy.html#details
class UILayoutPolicy
{
  public:
    typedef UILayoutPolicy self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Internal flags used to form the policies enumeration below.
    ///
    /// \note Borrowed from Qt 4.6.
    enum PolicyFlag: uint32
    {
      /// The widget can grow beyond its preferred size if necessary.
      GrowFlag = 0x1,

      /// The widget should get as much space as possible.
      ExpandFlag = 0x2,

      /// The widget can shrink below its preferred size if necessary.
      ShrinkFlag = 0x4,

      /// The widget's preferred size is ignored; the widget will get as much
      /// space as possible.
      IgnoreFlag = 0x8
    };

    /// \brief Per-dimension sizing types.
    ///
    /// \remarks The policy types are for use with UIWidget::size_hint.
    ///
    /// \note Borrowed from Qt 4.6.
    enum Policy: uint32
    {
      /// The preferred size is the only acceptable alternative; the widget can
      /// never grow or shrink.
      Fixed = 0x0,

      /// The preferred size is minimal, and sufficient; the widget can be
      /// expanded, but there is no advantage to it being larger. It cannot be
      /// smaller than the size provided by ::size_hint.
      Minimum = GrowFlag,

      /// The widget can be shrunk any amount without detriment if other
      /// widgets need the space. It cannot be larger than the size provided by
      /// ::size_hint.
      Maximum = ShrinkFlag,

      /// The widget prefers its own size. but it can still be shrunk and be
      /// useful.The widget can be expanded, but there is no advantage to it
      /// being larger than ::size_hint -- the default UIWidget size policy.
      Preferred = GrowFlag | ShrinkFlag,

      /// The preferred size is ignored; the widget is space hungry and will get
      /// as much space as possible.
      Ignored = ShrinkFlag | GrowFlag | IgnoreFlag
    };

    /// \brief Default constructor; initialize the horizontal and vertical size
    /// policies to Policy::Preferred.
    UILayoutPolicy( void );

    /// \brief Destructor.
    ~UILayoutPolicy( void );

    /// \brief Construct an object with an initialized size policy.
    UILayoutPolicy( uint32 h_policy, uint32 v_policy );

    Orientations directions( void ) const;

    uint32 horizontal_policy( void ) const;
    uint32 vertical_policy( void ) const;

    int horizontal_stretch( void ) const;
    int vertical_stretch( void ) const;

    void set_horizontal_policy( uint32 flags );
    void set_vertical_policy( uint32 flags );

    void set_horizontal_stretch( int factor );
    void set_vertical_stretch( int factor );

    /// \brief Transpose the data members -- policy, stretch, directions --
    /// in the object's instance.
    void swap( void );

    /// \brief Equality comparison operator.
    bool operator ==( const self_type& rhs ) const;

    /// \brief In-equality comparison operator.
    bool operator !=( const self_type& rhs ) const;

  private:
    uint32 h_policy_;
    uint32 v_policy_;

    int h_stretch_;
    int v_stretch_;

    enum Orientations directions_;
};

} // namespace nom

#endif // include guard defined
