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

#define BOOST_TEST_MODULE "ImageProcessing/Image Resize"

#include <boost/test/unit_test.hpp>

#include <DO/Sara/ImageProcessing/Resize.hpp>

#include "../AssertHelpers.hpp"


using namespace std;
using namespace DO::Sara;

BOOST_AUTO_TEST_SUITE(TestImageResize)

BOOST_AUTO_TEST_CASE(test_upscale)
{
  auto src = Image<float>{2, 2};
  src.matrix() << 0, 1, 2, 3;

  auto dst = Image<float>{};
  dst = upscale(src, 2);

  auto true_dst = Image<float>{4, 4};
  true_dst.matrix() << 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3;
  BOOST_CHECK_EQUAL(true_dst.matrix(), dst.matrix());
}

BOOST_AUTO_TEST_CASE(test_downscale)
{
  auto src = Image<float>{4, 4};
  src.matrix() << 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3;

  auto dst = Image<float>{};
  dst = downscale(src, 2);

  auto true_dst = Image<float>{2, 2};
  true_dst.matrix() << 0, 1, 2, 3;
  BOOST_CHECK_EQUAL(true_dst.matrix(), dst.matrix());
}

BOOST_AUTO_TEST_CASE(test_enlarge_on_image_views)
{
  auto src = Image<float>{5, 5};
  for (int y = 0; y < src.height(); ++y)
    src.matrix().row(y).fill(y);
  src.matrix() <<
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    4, 4, 4, 4, 4;

  auto dst = Image<float>{5, 10};
  enlarge(src, dst);

  auto true_dst = Image<float>{5, 10};
  true_dst.matrix() <<
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.5, 0.5, 0.5, 0.5, 0.5,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.5, 1.5, 1.5, 1.5, 1.5,
    2.0, 2.0, 2.0, 2.0, 2.0,
    2.5, 2.5, 2.5, 2.5, 2.5,
    3.0, 3.0, 3.0, 3.0, 3.0,
    3.5, 3.5, 3.5, 3.5, 3.5,
    4.0, 4.0, 4.0, 4.0, 4.0,
    4.0, 4.0, 4.0, 4.0, 4.0;

  BOOST_CHECK_LE((true_dst.matrix() - dst.matrix()).norm(), 1e-9);
}

BOOST_AUTO_TEST_CASE(test_enlarge)
{
  auto src = Image<float>{2, 2};
  src.matrix() << 0, 1, 2, 3;

  auto true_dst = Image<float>{4, 4};
  true_dst.matrix() <<
    0, 0.5, 1, 1,
    1, 1.5, 2, 2,
    2, 2.5, 3, 3,
    2, 2.5, 3, 3;

  auto dst = Image<float>{};

  dst = enlarge(src, {4, 4});
  BOOST_CHECK_EQUAL(true_dst.matrix(), dst.matrix());

  dst = enlarge(src, 2);
  BOOST_CHECK_EQUAL(true_dst.matrix(), dst.matrix());
}

BOOST_AUTO_TEST_CASE(test_reduce_on_image_views)
{
  auto src = Image<float>{10, 10};
  for (int y = 0; y < 10; ++y)
    src.matrix().row(y).fill(y);

  auto dst = Image<float>{10, 5};
  reduce(src, dst);

  // Make the following check except for y = 0, which should be sorted out
  // later.
  for (int y = 1; y < dst.height(); ++y)
  {
    auto true_value = RowVectorXf{10};
    true_value.fill(y * 2);
    BOOST_CHECK_LE(std::abs(true_value[0] - dst.matrix().row(y)[0]), 0.05);
  }
}

BOOST_AUTO_TEST_CASE(test_reduce_single_channel)
{
  auto src = Image<float>{4, 4};
  src.matrix() <<
    0, 0.5, 1, 1,
    1, 1.5, 2, 2,
    2, 2.5, 3, 3,
    2, 2.5, 3, 3;

  auto true_dst = Image<float>{2, 2};
  true_dst.matrix() <<
    0, 1,
    2, 3;

  auto dst = Image<float>{};

  dst = reduce(src, {2, 2});
  BOOST_CHECK_LE((true_dst.matrix() - dst.matrix()).lpNorm<Infinity>(), 0.4);

  dst = reduce(src, 2);
  BOOST_CHECK_LE((true_dst.matrix() - dst.matrix()).lpNorm<Infinity>(), 0.4);
}

BOOST_AUTO_TEST_CASE(test_reduce_rgb)
{
  auto lambda = [](double lambda) { return Rgb64f{lambda, lambda, lambda}; };
  auto src = Image<Rgb64f>{4, 4};
  src(0, 0) = lambda(0); src(1, 0) = lambda(0.5); src(2, 0) = lambda(1); src(3, 0) = lambda(1);
  src(0, 1) = lambda(1); src(1, 1) = lambda(1.5); src(2, 1) = lambda(2); src(3, 1) = lambda(2);
  src(0, 2) = lambda(2); src(1, 2) = lambda(2.5); src(2, 2) = lambda(3); src(3, 2) = lambda(3);
  src(0, 3) = lambda(2); src(1, 3) = lambda(2.5); src(2, 3) = lambda(3); src(3, 3) = lambda(3);

  auto true_dst = Image<Rgb64f>{2, 2};
  true_dst.matrix();
  true_dst(0, 0) = lambda(0); true_dst(1, 0) = lambda(1);
  true_dst(0, 1) = lambda(2); true_dst(1, 1) = lambda(3);

  Image<Rgb64f> dst;
  dst = reduce(src, 2);

  auto dst_pixel = dst.begin();
  auto true_dst_pixel = true_dst.begin();
  for (; dst_pixel != dst.end(); ++dst_pixel, ++true_dst_pixel)
    BOOST_CHECK_LE((*true_dst_pixel - *dst_pixel).lpNorm<Infinity>(), 0.4);
}

BOOST_AUTO_TEST_CASE(test_resize)
{
  auto src = Image<float>{4, 4};
  for (int y = 0; y < src.height(); ++y)
    src.matrix().row(y).fill(y);

  // Check case 1: inplace downscaling.
  {
    auto dst = Image<float>{2, 2};
    resize(src, dst);

    for (int y = 1; y < dst.height(); ++y)
    {
      auto true_value = RowVectorXf{dst.width()};
      true_value.fill(y * 2);
      BOOST_CHECK_LE(std::abs(true_value[0] - dst.matrix().row(y)[0]), 0.05);
    }
  }

  // Check case 2: inplace upscaling.
  {
    auto dst = Image<float>{8, 8};
    resize(src, dst);

    // Check the height.
    for (int y = 0; y < dst.height() - 1; ++y)
    {
      auto true_value = RowVectorXf{dst.width()};
      true_value.fill(y / 2.);
      BOOST_CHECK_LE((true_value - dst.matrix().row(y)).norm(), 1e-6);
    }

    // Check the last row.
    BOOST_CHECK_LE((RowVectorXf::Ones(dst.width()) * 3 -
                    dst.matrix().row(dst.height() - 1))
                       .norm(),
                   1e-6);
  }

  // Check case 3: inplace mixed between downscaling and upscaling.
  {
    auto dst = Image<float>{8, 2};
    resize(src, dst);
    BOOST_CHECK_LE(
        (dst.matrix().row(1) - RowVectorXf::Ones(dst.width()) * 2).norm(),
        1e-1f);
  }

  // Check case 3: same thing but instead this creates a new resized image.
  {
    auto dst = resize(src, {8, 2});
    BOOST_CHECK_LE(
        (dst.matrix().row(1) - RowVectorXf::Ones(dst.width()) * 2).norm(),
        1e-1f);
  }
}

BOOST_AUTO_TEST_CASE(test_resize_while_preserving_ratio)
{
  auto src = Image<float>{4, 3};
  src.matrix() <<
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11;

  auto true_dst = Image<float>{8, 8};
  true_dst.matrix() <<
    0,   0, 0,   0,  0,    0,  0,  0,
    0, 0.5, 1, 1.5,  2,  2.5,  3,  3,
    2, 2.5, 3, 3.5,  4,  4.5,  5,  5,
    4, 4.5, 5, 5.5,  6,  6.5,  7,  7,
    6, 6.5, 7, 7.5,  8,  8.5,  9,  9,
    8, 8.5, 9, 9.5, 10, 10.5, 11, 11,
    8, 8.5, 9, 9.5, 10, 10.5, 11, 11,
    0,   0, 0,   0,  0,    0,  0,  0;

  {
    auto dst = Image<float>{8, 8};
    const auto resize_image = SizeRatioPreservingImageResizer{};
    const auto resize_info = resize_image(src, dst);
    BOOST_CHECK_LE((dst.matrix() - true_dst.matrix()).norm(), 1e-9);

    const auto& window = std::get<0>(resize_info);
    // Top-left corner.
    BOOST_CHECK_EQUAL(window[0], 0);
    BOOST_CHECK_EQUAL(window[1], 1);
    // Bottom-right corner.
    BOOST_CHECK_EQUAL(window[2], 8);
    BOOST_CHECK_EQUAL(window[3], 7);

    const auto& scale = std::get<1>(resize_info);
    BOOST_CHECK_LE(std::abs(scale - 2), 1e-9);

    const auto& padding = std::get<2>(resize_info);
    // Top padding.
    BOOST_CHECK_EQUAL(padding[0], 1);
    // Bottom padding.
    BOOST_CHECK_EQUAL(padding[1], 1);
    // Left padding.
    BOOST_CHECK_EQUAL(padding[2], 0);
    // Right padding.
    BOOST_CHECK_EQUAL(padding[3], 0);
  }

  {
    const auto resize_image = SizeRatioPreservingImageResizer{};
    const auto out = resize_image(src, {8, 8});
    const auto& dst = std::get<0>(out);
    BOOST_CHECK_LE((dst.matrix() - true_dst.matrix()).norm(), 1e-9);
  }
}

BOOST_AUTO_TEST_SUITE_END()
