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
#include <gtest/gtest.h>

#include <nomlib/config.hpp>
#include <nomlib/actions/ActionTimingCurves.hpp>
#include <nomlib/system/init.hpp>

using namespace nom;

/// \brief Animation engine unit tests
class ActionTimingCurvesTest: public ::testing::Test
{
  public:
    /// \remarks This method is called at the start of each unit test.
    ActionTimingCurvesTest()
    {
      //
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~ActionTimingCurvesTest()
    {
      //
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      //
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      //
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      //
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      //
    }

  protected:
    /// \brief The total number of frames, in fractional seconds.
    const real32 DURATION = 2.5f * 1000;

    /// \brief The initial value to begin from.
    const real32 b = 100.0f;

    /// \brief The total change over time.
    const real32 c = 300.0f - b;

    /// \brief The return value of the timing mode function.
    real32 ret = 0.0f;
};

// ...Linear tests...

TEST_F(ActionTimingCurvesTest, LinearEaseIn)
{
  ret = nom::Linear::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Linear::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.8, ret);

  ret = nom::Linear::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(104, ret);

  ret = nom::Linear::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(108, ret);

  ret = nom::Linear::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(120, ret);

  ret = nom::Linear::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(140, ret);

  ret = nom::Linear::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(160, ret);

  ret = nom::Linear::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(180, ret);

  ret = nom::Linear::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, LinearEaseOut)
{
  ret = nom::Linear::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Linear::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.8, ret);

  ret = nom::Linear::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(104, ret);

  ret = nom::Linear::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(108, ret);

  ret = nom::Linear::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(120, ret);

  ret = nom::Linear::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(140, ret);

  ret = nom::Linear::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(160, ret);

  ret = nom::Linear::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(180, ret);

  ret = nom::Linear::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, LinearEaseInOut)
{
  ret = nom::Linear::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Linear::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.8, ret);

  ret = nom::Linear::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(104, ret);

  ret = nom::Linear::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(108, ret);

  ret = nom::Linear::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(120, ret);

  ret = nom::Linear::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(140, ret);

  ret = nom::Linear::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(160, ret);

  ret = nom::Linear::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(180, ret);

  ret = nom::Linear::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Quad tests...

TEST_F(ActionTimingCurvesTest, QuadEaseIn)
{
  ret = nom::Quad::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quad::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0032, ret);

  ret = nom::Quad::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.08, ret);

  ret = nom::Quad::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.32, ret);

  ret = nom::Quad::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(102, ret);

  ret = nom::Quad::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(108, ret);

  ret = nom::Quad::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(118, ret);

  ret = nom::Quad::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(132, ret);

  ret = nom::Quad::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuadEaseOut)
{
  ret = nom::Quad::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quad::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.5968, ret);

  ret = nom::Quad::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(107.92, ret);

  ret = nom::Quad::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(115.68, ret);

  ret = nom::Quad::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(138, ret);

  ret = nom::Quad::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(172, ret);

  ret = nom::Quad::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(202, ret);

  ret = nom::Quad::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(228, ret);

  ret = nom::Quad::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuadEaseInOut)
{
  ret = nom::Quad::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quad::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0064, ret);

  ret = nom::Quad::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.16, ret);

  ret = nom::Quad::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.64, ret);

  ret = nom::Quad::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(104, ret);

  ret = nom::Quad::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(116, ret);

  ret = nom::Quad::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(136, ret);

  ret = nom::Quad::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(164, ret);

  ret = nom::Quad::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Cubic tests...

TEST_F(ActionTimingCurvesTest, CubicEaseIn)
{
  ret = nom::Cubic::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Cubic::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00002, ret);

  ret = nom::Cubic::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0016, ret);

  ret = nom::Cubic::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0128, ret);

  ret = nom::Cubic::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.2, ret);

  ret = nom::Cubic::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.6, ret);

  ret = nom::Cubic::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(105.4, ret);

  ret = nom::Cubic::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(112.8, ret);

  ret = nom::Cubic::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, CubicEaseOut)
{
  ret = nom::Cubic::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Cubic::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.39042, ret);

  ret = nom::Cubic::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(111.76158, ret);

  ret = nom::Cubic::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(123.05281, ret);

  ret = nom::Cubic::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(154.20001, ret);

  ret = nom::Cubic::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(197.60001, ret);

  ret = nom::Cubic::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(231.39999, ret);

  ret = nom::Cubic::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(256.79999, ret);

  ret = nom::Cubic::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, CubicEaseInOut)
{
  ret = nom::Cubic::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Cubic::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00005, ret);

  ret = nom::Cubic::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0064, ret);

  ret = nom::Cubic::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0512, ret);

  ret = nom::Cubic::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.8, ret);

  ret = nom::Cubic::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(106.4, ret);

  ret = nom::Cubic::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(121.60001, ret);

  ret = nom::Cubic::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(151.2, ret);

  ret = nom::Cubic::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Quart tests...

TEST_F(ActionTimingCurvesTest, QuartEaseIn)
{
  ret = nom::Quart::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quart::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00002, ret);

  ret = nom::Quart::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00003, ret);

  ret = nom::Quart::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00051, ret);

  ret = nom::Quart::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.02, ret);

  ret = nom::Quart::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.32, ret);

  ret = nom::Quart::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.62, ret);

  ret = nom::Quart::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(105.12, ret);

  ret = nom::Quart::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuartEaseOut)
{
  ret = nom::Quart::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quart::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.18086, ret);

  ret = nom::Quart::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(115.52634, ret);

  ret = nom::Quart::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(130.13071, ret);

  ret = nom::Quart::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(168.78001, ret);

  ret = nom::Quart::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(218.07999, ret);

  ret = nom::Quart::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(251.98, ret);

  ret = nom::Quart::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(274.07999, ret);

  ret = nom::Quart::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuartEaseInOut)
{
  ret = nom::Quart::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quart::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quart::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00026, ret);

  ret = nom::Quart::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0041, ret);

  ret = nom::Quart::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.16, ret);

  ret = nom::Quart::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.56, ret);

  ret = nom::Quart::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(112.96, ret);

  ret = nom::Quart::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(140.96001, ret);

  ret = nom::Quart::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Quint tests...

TEST_F(ActionTimingCurvesTest, QuintEaseIn)
{
  ret = nom::Quint::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quint::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00002, ret);

  ret = nom::Quint::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quint::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00002, ret);

  ret = nom::Quint::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.002, ret);

  ret = nom::Quint::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.064, ret);

  ret = nom::Quint::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.486, ret);

  ret = nom::Quint::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.048, ret);

  ret = nom::Quint::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuintEaseOut)
{
  ret = nom::Quint::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quint::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.96814, ret);

  ret = nom::Quint::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(119.21581, ret);

  ret = nom::Quint::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(136.92548, ret);

  ret = nom::Quint::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(181.90201, ret);

  ret = nom::Quint::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(234.464, ret);

  ret = nom::Quint::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(266.38599, ret);

  ret = nom::Quint::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(284.448, ret);

  ret = nom::Quint::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, QuintEaseInOut)
{
  ret = nom::Quint::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quint::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Quint::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00001, ret);

  ret = nom::Quint::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00033, ret);

  ret = nom::Quint::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.032, ret);

  ret = nom::Quint::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.024, ret);

  ret = nom::Quint::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(107.776, ret);

  ret = nom::Quint::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(132.76801, ret);

  ret = nom::Quint::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Back tests...

TEST_F(ActionTimingCurvesTest, BackEaseIn)
{
  ret = nom::Back::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Back::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.994591, ret);

  ret = nom::Back::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.868195, ret);

  ret = nom::Back::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.490074, ret);

  ret = nom::Back::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(97.137154, ret);

  ret = nom::Back::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(90.709885, ret);

  ret = nom::Back::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(83.960091, ret);

  ret = nom::Back::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(80.129669, ret);

  ret = nom::Back::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, BackEaseOut)
{
  ret = nom::Back::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Back::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.74081, ret);

  ret = nom::Back::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(118.29839, ret);

  ret = nom::Back::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(135.59822, ret);

  ret = nom::Back::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(181.76562, ret);

  ret = nom::Back::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(241.16043, ret);

  ret = nom::Back::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(281.42645, ret);

  ret = nom::Back::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(305.80548, ret);

  ret = nom::Back::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, BackEaseInOut)
{
  ret = nom::Back::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Back::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.983574, ret);

  ret = nom::Back::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.607819, ret);

  ret = nom::Back::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(98.523315, ret);

  ret = nom::Back::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(92.496292, ret);

  ret = nom::Back::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(81.488869, ret);

  ret = nom::Back::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(84.233307, ret);

  ret = nom::Back::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(117.98517, ret);

  ret = nom::Back::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Bounce tests...

TEST_F(ActionTimingCurvesTest, BounceEaseIn)
{
  ret = nom::Bounce::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Bounce::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.5258, ret);

  ret = nom::Bounce::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.14499, ret);

  ret = nom::Bounce::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.08, ret);

  ret = nom::Bounce::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.375, ret);

  ret = nom::Bounce::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(112, ret);

  ret = nom::Bounce::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(113.875, ret);

  ret = nom::Bounce::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(145.5, ret);

  ret = nom::Bounce::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, BounceEaseOut)
{
  ret = nom::Bounce::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Bounce::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0242, ret);

  ret = nom::Bounce::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.605, ret);

  ret = nom::Bounce::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.42, ret);

  ret = nom::Bounce::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(115.125, ret);

  ret = nom::Bounce::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(160.5, ret);

  ret = nom::Bounce::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(236.12502, ret);

  ret = nom::Bounce::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(282, ret);

  ret = nom::Bounce::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, BounceEaseInOut)
{
  ret = nom::Bounce::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Bounce::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.5016, ret);

  ret = nom::Bounce::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.54, ret);

  ret = nom::Bounce::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.66, ret);

  ret = nom::Bounce::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(106, ret);

  ret = nom::Bounce::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(122.75, ret);

  ret = nom::Bounce::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(109, ret);

  ret = nom::Bounce::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(169.75, ret);

  ret = nom::Bounce::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Circ tests...

TEST_F(ActionTimingCurvesTest, CircEaseIn)
{
  ret = nom::Circ::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Circ::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00159, ret);

  ret = nom::Circ::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.04, ret);

  ret = nom::Circ::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.16006, ret);

  ret = nom::Circ::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.00251, ret);

  ret = nom::Circ::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(104.04082, ret);

  ret = nom::Circ::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(109.21217, ret);

  ret = nom::Circ::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(116.69698, ret);

  ret = nom::Circ::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, CircEaseOut)
{
  ret = nom::Circ::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Circ::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(117.8707, ret);

  ret = nom::Circ::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(139.79947, ret);

  ret = nom::Circ::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(156, ret);

  ret = nom::Circ::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(187.17799, ret);

  ret = nom::Circ::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(220, ret);

  ret = nom::Circ::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(242.82857, ret);

  ret = nom::Circ::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(260, ret);

  ret = nom::Circ::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, CircEaseInOut)
{
  ret = nom::Circ::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Circ::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0032, ret);

  ret = nom::Circ::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.08003, ret);

  ret = nom::Circ::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.32051, ret);

  ret = nom::Circ::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.02041, ret);

  ret = nom::Circ::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(108.34849, ret);

  ret = nom::Circ::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(120, ret);

  ret = nom::Circ::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(140, ret);

  ret = nom::Circ::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Elastic tests...

TEST_F(ActionTimingCurvesTest, ElasticEaseIn)
{
  ret = nom::Elastic::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Elastic::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.914505, ret);

  ret = nom::Elastic::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.976547, ret);

  ret = nom::Elastic::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.07964, ret);

  ret = nom::Elastic::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.39062, ret);

  ret = nom::Elastic::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.609375, ret);

  ret = nom::Elastic::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.21875, ret);

  ret = nom::Elastic::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.125, ret);

  ret = nom::Elastic::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, ElasticEaseOut)
{
  ret = nom::Elastic::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Elastic::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(106.15126, ret);

  ret = nom::Elastic::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(140.94249, ret);

  ret = nom::Elastic::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(198.57877, ret);

  ret = nom::Elastic::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(350, ret);

  ret = nom::Elastic::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(325, ret);

  ret = nom::Elastic::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(275, ret);

  ret = nom::Elastic::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(306.25, ret);

  ret = nom::Elastic::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, ElasticEaseInOut)
{
  ret = nom::Elastic::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Elastic::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.02914, ret);

  ret = nom::Elastic::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.08622, ret);

  ret = nom::Elastic::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.16344, ret);

  ret = nom::Elastic::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.06783, ret);

  ret = nom::Elastic::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(99.21875, ret);

  ret = nom::Elastic::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(104.78778, ret);

  ret = nom::Elastic::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(76.507683, ret);

  ret = nom::Elastic::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Expo tests...

TEST_F(ActionTimingCurvesTest, ExpoEaseIn)
{
  ret = nom::Expo::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Expo::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.20081, ret);

  ret = nom::Expo::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.22436, ret);

  ret = nom::Expo::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.25771, ret);

  ret = nom::Expo::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.39062, ret);

  ret = nom::Expo::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.78125, ret);

  ret = nom::Expo::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.5625, ret);

  ret = nom::Expo::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(103.125, ret);

  ret = nom::Expo::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, ExpoEaseOut)
{
  ret = nom::Expo::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Expo::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(105.46901, ret);

  ret = nom::Expo::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(125.88988, ret);

  ret = nom::Expo::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(148.42834, ret);

  ret = nom::Expo::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(200, ret);

  ret = nom::Expo::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(250, ret);

  ret = nom::Expo::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(275, ret);

  ret = nom::Expo::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(287.5, ret);

  ret = nom::Expo::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, ExpoEaseInOut)
{
  ret = nom::Expo::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Expo::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.10322, ret);

  ret = nom::Expo::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.12886, ret);

  ret = nom::Expo::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.17003, ret);

  ret = nom::Expo::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.39062, ret);

  ret = nom::Expo::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.5625, ret);

  ret = nom::Expo::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(106.25, ret);

  ret = nom::Expo::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(125, ret);

  ret = nom::Expo::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

// ...Sine tests...

TEST_F(ActionTimingCurvesTest, SineEaseIn)
{
  ret = nom::Sine::ease_in(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Sine::ease_in(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.00394, ret);

  ret = nom::Sine::ease_in(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.09869, ret);

  ret = nom::Sine::ease_in(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.39465, ret);

  ret = nom::Sine::ease_in(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(102.46233, ret);

  ret = nom::Sine::ease_in(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(109.7887, ret);

  ret = nom::Sine::ease_in(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(121.7987, ret);

  ret = nom::Sine::ease_in(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(138.19661, ret);

  ret = nom::Sine::ease_in(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, SineEaseOut)
{
  ret = nom::Sine::ease_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Sine::ease_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(101.25663, ret);

  ret = nom::Sine::ease_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(106.28215, ret);

  ret = nom::Sine::ease_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(112.55811, ret);

  ret = nom::Sine::ease_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(131.2869, ret);

  ret = nom::Sine::ease_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(161.80341, ret);

  ret = nom::Sine::ease_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(190.7981, ret);

  ret = nom::Sine::ease_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(217.55705, ret);

  ret = nom::Sine::ease_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

TEST_F(ActionTimingCurvesTest, SineEaseInOut)
{
  ret = nom::Sine::ease_in_out(0, b, c, DURATION);
  EXPECT_FLOAT_EQ(100, ret);

  ret = nom::Sine::ease_in_out(10, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.0079, ret);

  ret = nom::Sine::ease_in_out(50, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.19733, ret);

  ret = nom::Sine::ease_in_out(100, b, c, DURATION);
  EXPECT_FLOAT_EQ(100.78853, ret);

  ret = nom::Sine::ease_in_out(250, b, c, DURATION);
  EXPECT_FLOAT_EQ(104.89435, ret);

  ret = nom::Sine::ease_in_out(500, b, c, DURATION);
  EXPECT_FLOAT_EQ(119.0983, ret);

  ret = nom::Sine::ease_in_out(750, b, c, DURATION);
  EXPECT_FLOAT_EQ(141.22148, ret);

  ret = nom::Sine::ease_in_out(1000, b, c, DURATION);
  EXPECT_FLOAT_EQ(169.0983, ret);

  ret = nom::Sine::ease_in_out(DURATION, b, c, DURATION);
  EXPECT_FLOAT_EQ(300, ret);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init(argc, argv) == false ) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  return RUN_ALL_TESTS();
}
