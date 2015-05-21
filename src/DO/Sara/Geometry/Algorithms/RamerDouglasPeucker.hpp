// ========================================================================== //
// This file is part of DO-CV, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2014 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#ifndef DO_SARA_GEOMETRY_ALGORITHMS_RAMERDOUGLASPEUCKER_HPP
#define DO_SARA_GEOMETRY_ALGORITHMS_RAMERDOUGLASPEUCKER_HPP

#include <DO/Sara/Core/EigenExtension.hpp>
#include <DO/Sara/Core/StdVectorHelpers.hpp>

namespace DO {

  std::vector<Point2d>
  ramer_douglas_peucker(const std::vector<Point2d>& contours, double eps);

}

#endif /* DO_SARA_GEOMETRY_ALGORITHMS_RAMERDOUGLASPEUCKER_HPP */