// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
//
// Copyright (C) 2013-2017 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

#pragma once

#include <DO/Sara/Defines.hpp>
#include <DO/Sara/Core/Image.hpp>


namespace DO { namespace Sara {

  DO_SARA_EXPORT
  void imread(Image<unsigned char>& image, const std::string& filepath);

  DO_SARA_EXPORT
  void imread(Image<Rgb8>& image, const std::string& filepath);

  template <typename T>
  inline void imread(Image<T>& image, const std::string& filepath)
  {
    Image<Rgb8> rgb8image;
    imread(rgb8image, filepath);
    image = rgb8image.convert<T>();
  }

  DO_SARA_EXPORT
  void imwrite(const Image<Rgb8>& image, const std::string& filepath,
               int quality = 85);


} /* namespace Sara */
} /* namespace DO */
