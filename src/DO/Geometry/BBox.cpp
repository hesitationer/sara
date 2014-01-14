// ========================================================================== //
// This file is part of DO++, a basic set of libraries in C++ for computer 
// vision.
//
// Copyright (C) 2013 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public 
// License v. 2.0. If a copy of the MPL was not distributed with this file, 
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#include <DO/Geometry.hpp>
#include <DO/Graphics.hpp>

using namespace std;

namespace DO {

  void BBox::print() const
  {
    cout << "top left " << tl_.transpose() << endl;
    cout << "bottom right " << br_.transpose() << endl;
  }

  void BBox::drawOnScreen(const Color3ub& col, double z,
                          int thickness) const
  {
    Point2d tl(z*tl_);
    Point2d br(z*br_);
    Point2d sz(br-tl);
    drawRect(tl.cast<int>()(0), tl.cast<int>()(1),
             sz.cast<int>()(0), sz.cast<int>()(1),
             col, thickness);
  }

  bool isInside(const Point2d& p, const BBox& bbox)
  {
    return 
      p.x() >= bbox.topLeft().x() && p.x() <= bbox.bottomRight().x() &&
      p.y() >= bbox.topLeft().y() && p.y() <= bbox.bottomRight().y() ;
  }

  bool isDegenerate(const BBox& bbox, double areaThres)
  {
    return (bbox.area() < areaThres);
  }

} /* namespace DO */