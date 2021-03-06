// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2013-2016 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //


#include <DO/Sara/Features.hpp>
#include <DO/Sara/Graphics.hpp>
#include <DO/Sara/Core/DebugUtilities.hpp>

using namespace std;


namespace DO { namespace Sara {

  void InterestPoint::draw(const Color3ub& color,
                           float scale,
                           const Point2f& offset) const
  {
    const auto& z = scale;
    const auto cross_offset = 3.0f;

    Vector2f p1{ z * (center() + offset) };
    Vector2f c1{ p1 - Vector2f{ cross_offset, 0.f } };
    Vector2f c2{ p1 + Vector2f{ cross_offset, 0.f } };
    Vector2f c3{ p1 - Vector2f{ 0.f,cross_offset } };
    Vector2f c4{ p1 + Vector2f{ 0.f,cross_offset } };

    draw_line(c1, c2, Black8, 5);
    draw_line(c3, c4, Black8, 5);
    draw_line(c1, c2, color, 3);
    draw_line(c3, c4, color, 3);
  }

  void OERegion::draw(const Color3ub& color,
                      float scale,
                      const Point2f& offset) const
  {
    const auto& z = scale;

    // Solve characteristic equation to find eigenvalues
    JacobiSVD<Matrix2f> svd(shape_matrix(), ComputeFullU);
    const Vector2f& D = svd.singularValues();
    const Matrix2f& U = svd.matrixU();

    // Eigenvalues are l1 and l2.
    const Vector2f radii{ D.cwiseSqrt().cwiseInverse() };

    const auto& a = radii(0);
    const auto& b = radii(1);

    // Orientation.
    auto ellipse_ori = atan2(U(1,0), U(0,0));

    // Start and end points of orientation line.
    Matrix2f L{ affinity().block(0, 0, 2, 2) };
    Vector2f p1{ z * (center() + offset) };
    Vector2f p2{ p1 + z * L * Vector2f{ 1.f, 0.f } };

    // Draw.
    if (z*a > 1.f && z*b > 1.f && (p1 - p2).squaredNorm() > 1.f)
    {
      // Contour of orientation line.
      draw_line(p1, p2, Black8, 5);
      // Contour of ellipse.
      draw_ellipse(p1, z*a, z*b, 180.f*ellipse_ori/float(M_PI), Black8, 5);
      // Fill-in of orientation line.
      draw_line(p1, p2, color, 3);
      // Fill-in of ellipse.
      draw_ellipse(p1, z*a, z*b, 180.f*ellipse_ori/float(M_PI), color, 3);
    }
    else
      InterestPoint::draw(color, z, offset);
  }

  void draw_oe_regions(const vector<OERegion>& features,
                       const Color3ub& color,
                       float scale,
                       const Point2f& offset)
  {
    for (size_t i = 0; i < features.size(); ++i)
      features[i].draw(color, scale, offset);
  }

} /* namespace Sara */
} /* namespace DO */
