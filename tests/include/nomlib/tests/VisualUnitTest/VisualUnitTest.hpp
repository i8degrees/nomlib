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
#ifndef NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_HPP
#define NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_HPP

#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include "gtest/gtest.h"

#include "nomlib/config.hpp"
#include "nomlib/tests/UnitTest/UnitTest.hpp"
#include "nomlib/tests/VisualUnitTest/ImageTestSet.hpp"
#include "nomlib/tests/VisualUnitTest/ImageDiff.hpp"
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/system/Timer.hpp"
#include "nomlib/system/FPS.hpp"
#include "nomlib/system/Event.hpp"
#include "nomlib/system/EventHandler.hpp"
#include "nomlib/system/InputMapper/InputStateMapper.hpp"

namespace nom {

/// \brief Base class interface for visual unit testing within Google Test
class VisualUnitTest: public UnitTest
{
  public:
    /// \remarks Access to global VisualUnitTest::results_ (test result totals)
    /// and VisualUnitTest::timestamp_ (file & directory sync).
    friend class VisualUnitTestResultWriter;

    typedef std::function<void( Event )> event_callback_type;
    typedef std::function<void( float )> update_callback_type;

    /// \todo Consider removing the callback argument; it is confusing as it is
    /// invalid to use a RenderWindow object that differs from the one defined
    /// in this class.
    typedef std::function<void( const RenderWindow& )> render_callback_type;

    /// \brief Initialize the environment for the unit test.
    ///
    /// \note This is done for us automatically in the constructor of
    /// VisualUnitTest.
    ///
    /// \remarks The necessary directory tree -- if any (this is dependent upon
    /// parsed command line options) -- is initialized at this time. The
    /// recorded timestamp of the visual unit test will be the set upon the
    /// first constructed object instance of this class.
    void initialize( const Size2i& res );

    /// \brief Default constructor; initialize the environment.
    ///
    /// \remarks The resolution default of 640 x 480 is used.
    VisualUnitTest( void );

    /// \brief Construct the object using a non-default rendering resolution.
    ///
    /// \remarks This is intended for derived classes.
    ///
    /// \remarks The default resolution set for the rendering window is
    /// initialized to 640 x 480 -- to change the rendering resolution, a call
    /// to ::set_resolution must be made before ::SetUp is called.
    ///
    /// If using the image comparison features of the class, you must append
    /// at least one screen-shot frame before the ::on_run loop is executed.
    VisualUnitTest( const Size2i& res );

    /// \brief Destructor.
    ///
    /// \remarks Destruction of the object flushes the configuration file for
    /// the visual test set when it has images in its list.
    virtual ~VisualUnitTest( void );

    /// \brief Initialize the rendering subsystem.
    ///
    /// \remarks To override this method with a custom environment, at minimum,
    /// you must provide the appropriate call to RenderWindow::create.
    ///
    /// \note This method gets called ::SetUp, which is called by Google Test
    /// at the start of every test.
    ///
    /// \see nom::RenderWindow
    virtual bool init_rendering();

    /// \brief Initialize the default input bindings used for each unit test.
    ///
    /// \remarks When re-implementing this method for your own test, you must
    /// call this class, otherwise the rendering and input subsystems do not
    /// get setup.
    ///
    /// \note Re-implements ::testing::SetUp.
    ///
    /// \see nom::InputStateMapper, ::testing::SetUp.
    virtual void SetUp( void );

    /// \brief Compare the current test run image against a reference set.
    ///
    /// \remarks This method supports Google Test's EXPECT & friends macros;
    /// typical usage is EXPECT_TRUE( this->compare() ).
    ///
    /// \todo Allow margin of err in image diff tests?
    /// See also:
    /// [Allow approximate comparison with a margin of error #38](https://github.com/facebook/ios-snapshot-test-case/issues/38)
    /// [AllimageCompare](https://github.com/aleph7/AIImageCompare)
    ::testing::AssertionResult compare( void );

    /// \brief Get the visibility state of the FPS counter.
    bool fps( void ) const;

    /// \brief Get the window flags used.
    uint32 window_flags() const;

    /// \brief The resolution (width and height, in pixels) used for creating
    /// a rendering window for the test.
    const Size2i& resolution( void ) const;

    /// \brief The output directory prefix used for the unit test.
    const std::string& output_directory( void ) const;

    /// \brief Get the directory path prefix used for the current test case.
    const std::string& test_set_directory( void ) const;

    /// \brief Get the directory path used for dumping screen-shots and its
    /// associated meta-data.
    const std::string& test_reference_directory( void ) const;

    /// \brief Get the filename used for outputting screen-shots.
    const std::string& output_filename( void ) const;

    /// \brief Get the rendering window used for this test.
    ///
    /// \remarks A validity check (assertion) is done for the rendering window.
    ///
    /// \see ::SetUp.
    RenderWindow& render_window( void );

    /// \brief Set the default window flags.
    ///
    /// \remarks The window flags are applied to the rendering window that is
    /// created during default initialization (when ::init_rendering is not
    /// overridden). This implies that you must make a call to this method
    /// before ::SetUp is called. Typically, this means in your derived class's
    /// constructor.
    void set_window_flags( uint32 flags );

    /// \brief Toggle the visibility state of the FPS counter.
    void show_fps( bool state );

    /// \brief Set the render window's resolution.
    void set_resolution( const Size2i& res );

    /// \brief Set the output directory prefix used for the unit test.
    void set_output_directory( const std::string& dir_path );

    /// \brief Add a frame number to the list of frames to screen capture (for
    /// use as either a reference or comparison image set).
    ///
    /// \remarks If using the image comparison features of the class, you must
    /// append at least one screen-shot frame before the ::on_run loop is
    /// executed.
    void append_screenshot_frame( uint frame );

    virtual int on_run( void );

    /// \brief Screen shot processing logic that must be ran at the end of the
    /// current (rendered) frames.
    ///
    /// \remarks When overriding this method, you will almost certainly want to
    /// ensure that a call is made to its base class -- VisualUnitTeset::
    /// on_frame_end. Note that it is critical that you handle the return value
    /// correctly.
    ///
    /// \returns Boolean TRUE when the unit test is ready to terminate its main
    /// loop -- meaning the test has captured its target frames -- boolean FALSE
    /// when the unit test is not ready to terminate its main loop (has not
    /// captured all of its target frames).
    virtual bool on_frame_end( uint elapsed_frames );

    // void save_screenshot( const std::string& file_path );

    // TODO: VisualUnitTest::event_callback_type default_event_callback();
    VisualUnitTest::update_callback_type default_update_callback();
    VisualUnitTest::render_callback_type default_render_callback();

    /// \brief Insert an event callback at an ordered position for execution
    /// in the main loop.
    ///
    /// \param pos  The insertion position from the beginning of the container.
    /// \param func The event callback function.
    ///
    /// \returns The number of registered event callbacks (after insertion).
    ///
    /// \see nom::VisualUnitTest::append_event_callback
    int insert_event_callback(  nom::size_type pos,
                                const event_callback_type& func );

    /// \brief Insert an event callback to the end of the list of callbacks
    /// to be executed in the main loop.
    ///
    /// \param func The event callback function.
    ///
    /// \returns The number of registered event callbacks (after insertion).
    int append_event_callback(const event_callback_type& func);

    /// \brief Insert an update callback at an ordered position for execution
    /// in the main loop.
    ///
    /// \param pos  The insertion position from the beginning of the container.
    /// \param func The update callback function.
    ///
    /// \returns The number of registered update callbacks (after insertion).
    ///
    /// \see nom::VisualUnitTest::append_update_callback
    int insert_update_callback( nom::size_type pos,
                                const update_callback_type& func );

    /// \brief Insert an update callback to the end of the list of callbacks
    /// to be executed in the main loop.
    ///
    /// \param func The update callback function.
    ///
    /// \returns The number of registered update callbacks (after insertion).
    int append_update_callback(const update_callback_type& func);

    /// \brief Insert a rendering callback at an ordered position for execution
    /// in the main loop.
    ///
    /// \param pos  The insertion position from the beginning of the container.
    /// \param func The render callback function.
    ///
    /// \returns The number of registered render callbacks (after insertion).
    ///
    /// \see nom::VisualUnitTest::append_render_callback
    int insert_render_callback( nom::size_type pos,
                                const render_callback_type& func );

    /// \brief Insert a rendering callback to the end of the list of callbacks
    /// to be executed in the main loop.
    ///
    /// \param func The render callback function.
    ///
    /// \returns The number of registered render callbacks (after insertion).
    int append_render_callback(const render_callback_type& func);

    /// \brief Destroy all of the registered event callbacks in storage.
    ///
    /// \remarks The default callback(s) for the event logic inside the loop
    /// is destroyed -- be sure not to forget to take care of that...
    ///
    /// \note This method should *not* be called during execution in the main
    /// loop.
    void clear_event_callbacks();

    /// \brief Destroy all of the registered update callbacks in storage.
    ///
    /// \remarks The default callback(s) for the update logic inside the loop
    /// is destroyed -- be sure not to forget to take care of that...
    ///
    /// \note This method should *not* be called during execution in the main
    /// loop.
    void clear_update_callbacks();

    /// \brief Destroy all of the registered rendering callbacks in storage.
    ///
    /// \remarks The default callback(s) for the rendering of the window used
    /// by this interface is also destroyed -- be sure not to forget to take
    /// care of that...
    ///
    /// \note This method should *not* be called during execution in the main
    /// loop.
    ///
    /// \see ::default_render_callback
    void clear_render_callbacks();

  protected:
    /// \brief Default window initialization flags.
    uint32 window_flags_;

    /// \brief Track visibility state of the FPS counter.
    bool show_fps_;

    /// \brief Interval at which we refresh the FPS counter
    Timer fps_counter_update_;

    /// \brief FPS counter
    FPS fps_counter_;

    /// \brief The rendering window (including its rendering context) used.
    RenderWindow window_;

    /// \brief The polling method used for input events.
    EventHandler evt_;

    /// \brief The registered input bindings.
    InputStateMapper input_mapper_;

    static ImageTestSet visual_test_;

  private:
    /// \brief Create the necessary directory tree for screen-shot dumps.
    ///
    /// \remarks This modifies the output directory.
    ///
    /// \see ::set_output_directory.
    ///
    /// \todo Clean up the logic in this method.
    void initialize_directories( void );
    // void initialize_directories( const std::string& dir_name );

    /// \brief Get the timestamp string recorded for the visual unit test.
    static const std::string& timestamp( void );

    /// \brief Set the directory prefix used for the current unit test case.
    void set_test_set_directory( const std::string& dir_path );

    /// \brief Set the directory prefix for dumping unit test reference
    /// screen-shots.
    void set_test_reference_directory( const std::string& dir_path );

    void set_output_filename( const std::string& filename );

    /// \brief The common timestamp string shared across object instances.
    static std::string timestamp_;

    /// \brief Ensure one-time only initialization of the timestamp string.
    ///
    /// \remarks This check prevents object instances from potentially creating
    /// multiple output directories for one set of screen-shot dumps. (Within
    /// the context of being ran within the GTest environment, multiple object
    /// instances within a short time are quite common).
    static bool timestamp_initialized_;

    /// \brief The resolution used for the creation of a rendering window.
    Size2i resolution_;

    std::vector<uint> screenshot_frames_;

    /// \remarks The output directory path in use.
    std::string output_directory_;

    /// \remarks The directory path prefix used for the current test case.
    std::string test_set_directory_;

    /// \remarks The directory path used for test reference screen-shots and
    /// meta-data file.
    std::string test_reference_directory_;

    std::string output_filename_;

    std::vector<event_callback_type> event_callbacks_;
    std::vector<update_callback_type> update_callbacks_;
    std::vector<render_callback_type> render_callbacks_;

    static ImageDiffResultBatch results_;
};

/// \brief Convenience macro for binding an event listener to the unit test's
/// main loop.
#define NOM_TEST_ADD_EVENT(type, func) \
  ( this->append_event_callback( [&] ( type ) { func; } ) )

/// \brief Convenience macro for binding an update method to the unit test's
/// main loop.
#define NOM_TEST_ADD_UPDATE(func) \
  ( this->append_update_callback( [&] ( float delta ) { func; } ) )

/// \brief Convenience macro for binding a rendering method to the unit test's
/// main loop.
#define NOM_TEST_ADD_RENDER(type, func ) \
  ( this->append_render_callback( [&] ( type ) { func; } ) )

} // namespace nom

#endif // include guard defined

/// \class nom::VisualUnitTest
/// \ingroup tests
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
/// \see https://github.com/facebook/ios-snapshot-test-case
///
