// ========================================================================== //
// This file is part of DO-CV, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#include <gtest/gtest.h>

#include <DO/Sara/Core/EigenExtension.hpp>
#include <DO/Sara/Geometry/Algorithms/SutherlandHodgman.hpp>

#include "../AssertHelpers.hpp"


using namespace std;
using namespace DO::Sara;


TEST(TestSutherlandHodgmanPolygonClipping,
     test_subject_polygon_in_clip_polygon)
{
  vector<Point2d> clip_polygon;
  vector<Point2d> subject_polygon;
  vector<Point2d> result;

  // The clip polygon is a square.
  clip_polygon.push_back(Point2d(0, 0));
  clip_polygon.push_back(Point2d(1, 0));
  clip_polygon.push_back(Point2d(1, 1));
  clip_polygon.push_back(Point2d(0, 1));

  // The subject polygon is a triangle inside the clip polygon.
  subject_polygon.push_back(Point2d(0.25, 0.25));
  subject_polygon.push_back(Point2d(0.75, 0.25));
  subject_polygon.push_back(Point2d(0.50, 0.75));

  // The resulting polygon must the subject polygon.
  result = sutherland_hodgman(subject_polygon, clip_polygon);
  EXPECT_EQ(result, subject_polygon);
}

TEST(TestSutherlandHodgmanPolygonClipping,
     test_subject_polygon_outside_of_clip_polygon)
{
  vector<Point2d> clip_polygon;
  vector<Point2d> subject_polygon;
  vector<Point2d> result;

  // The clip polygon is a square.
  clip_polygon.push_back(Point2d(0, 0));
  clip_polygon.push_back(Point2d(1, 0));
  clip_polygon.push_back(Point2d(1, 1));
  clip_polygon.push_back(Point2d(0, 1));

  // The subject polygon is a triangle outside the clip polygon.
  subject_polygon.push_back(Point2d(3., 0.));
  subject_polygon.push_back(Point2d(5., 0.));
  subject_polygon.push_back(Point2d(4., 1.));

  // The resulting polygon must the empty polygon.
  result = sutherland_hodgman(subject_polygon, clip_polygon);
  EXPECT_TRUE(result.empty());
}

TEST(TestSutherlandHodgmanPolygonClipping,
     test_clip_polygon_in_subject_polygon)
{
  vector<Point2d> clip_polygon;
  vector<Point2d> subject_polygon;
  vector<Point2d> result;

  // The clip polygon is a square.
  clip_polygon.push_back(Point2d(0, 0));
  clip_polygon.push_back(Point2d(1, 0));
  clip_polygon.push_back(Point2d(1, 1));
  clip_polygon.push_back(Point2d(0, 1));

  // The subject polygon is a triangle containing the clip polygon.
  subject_polygon.push_back(Point2d(-10.,  0.));
  subject_polygon.push_back(Point2d( 10.,  0.));
  subject_polygon.push_back(Point2d( 10., 10.));

  // The resulting polygon must the empty polygon.
  result = sutherland_hodgman(subject_polygon, clip_polygon);
  EXPECT_TRUE(result.empty());
}

TEST(TestSutherlandHodgmanPolygonClipping, test_interesecting_bboxes)
{
  // The clip polygon is a square.
  vector<Point2d> clip_polygon;
  clip_polygon.push_back(Point2d(0, 0));
  clip_polygon.push_back(Point2d(1, 0));
  clip_polygon.push_back(Point2d(1, 1));
  clip_polygon.push_back(Point2d(0, 1));

  // The subject polygon is a triangle containing the clip polygon.
  vector<Point2d> subject_polygon;
  subject_polygon.push_back(Point2d(0.5,  0.5));
  subject_polygon.push_back(Point2d(1.5,  0.5));
  subject_polygon.push_back(Point2d(1.5,  1.5));
  subject_polygon.push_back(Point2d(0.5,  1.5));

  // The actual result of the implementation.
  vector<Point2d> actual_result;
  actual_result = sutherland_hodgman(subject_polygon, clip_polygon);

  // The expected result is a smaller box.
  vector<Point2d> expected_result;
  expected_result.push_back(Point2d(0.5, 0.5));
  expected_result.push_back(Point2d(1.0, 0.5));
  expected_result.push_back(Point2d(1.0, 1.0));
  expected_result.push_back(Point2d(0.5, 1.0));

  // 1. Check that the points are identical.
  EXPECT_ITEMS_EQ(expected_result, actual_result);

  // 2. Check that the points are enumerated in a CCW manner..
  const size_t N = actual_result.size();
  const vector<Point2d>& P = actual_result;
  for (size_t i = 0; i < N; ++i)
    EXPECT_EQ(1, ccw(P[i], P[(i+1)%N], P[(i+2)%N]));
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}