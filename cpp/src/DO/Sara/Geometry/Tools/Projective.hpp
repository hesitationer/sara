// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2014-2016 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#pragma once

#include <DO/Sara/Core/EigenExtension.hpp>


namespace DO { namespace Sara { namespace P2 {

    using Line = Vector3d;
    using Point = Vector3d;
    using Point2 = Point2d;
    using Vector2 = Vector2d;

    inline Point2 euclidean(const Point& p)
    {
      return (p/p(2)).head(2);
    }

    inline Point homogeneous(const Point2& p)
    {
      return Point(p.x(), p.y(), 1.);
    }

    inline Point intersection(const Line& l1, const Line& l2)
    {
      return l1.cross(l2);
    }

    inline Line line(const Point& p, const Point& q)
    {
      return p.cross(q);
    }

    inline Line line(const Point2&a, const Point2& b)
    {
      return line(homogeneous(a), homogeneous(b));
    }

    inline double dist(const Point& p, const Line& l)
    {
      return std::abs((p/p(2)).dot(l))/l.head(2).norm();
    }

} /* namespace P2 */
} /* namespace Sara */
} /* namespace DO */
