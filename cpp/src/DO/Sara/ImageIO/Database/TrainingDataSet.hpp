// ========================================================================== //
// This file is part of Sara, a basic set of libraries in C++ for computer
// vision.
// Copyright (C) 2017 David Ok <david.ok8@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================== //

//! @file

#pragma once

#include <tuple>

#include <DO/Sara/Defines.hpp>
#include <DO/Sara/ImageIO/Database/ImageDataSet.hpp>


namespace DO { namespace Sara {

  namespace details {
    template <typename Out>
    void split(const std::string& s, char delim, Out result)
    {
      std::stringstream ss;
      ss.str(s);
      auto item = std::string{};
      while (std::getline(ss, item, delim))
        *(result++) = item;
    }
  }


  template <typename XIterator, typename YIterator>
  class TrainingDataSetIterator
  {
  public:
    using x_iterator = XIterator;
    using y_iterator = YIterator;

    using x_type = typename XIterator::value_type;
    using y_type = typename YIterator::value_type;
    using self_type = TrainingDataSetIterator;

    inline TrainingDataSetIterator() = default;

    inline TrainingDataSetIterator(x_iterator x, y_iterator y)
      : _x{x}
      , _y{y}
    {
    }

    inline auto operator++() -> self_type&
    {
      ++_x;
      ++_y;
      return *this;
    }

    inline auto operator--() -> self_type&
    {
      --_x;
      --_y;
      return *this;
    }

    inline auto operator+=(std::ptrdiff_t n) -> self_type&
    {
      _x += n;
      _y += n;
      return *this;
    }

    inline auto operator-=(std::ptrdiff_t n) -> self_type&
    {
      _x -= n;
      _y -= n;
      return *this;
    }

    inline auto x() const -> x_iterator
    {
      return _x;
    }

    inline auto y() const -> y_iterator
    {
      return _y;
    }

    inline auto x_ref() const -> const x_type&
    {
      return *_x;
    }

    inline auto y_ref() const -> const y_type&
    {
      return *_y;
    }

    inline auto operator==(const self_type& other) const -> bool
    {
      return std::make_pair(_x, _y) == std::make_pair(other._x, other._y);
    }

    inline auto operator!=(const self_type& other) const -> bool
    {
      return !(operator==(other));
    }

    inline auto operator*() const
        -> decltype(std::make_pair(std::declval<const x_type>(),
                                   std::declval<const y_type>()))
    {
      return std::make_pair(*_x, *_y);
    }

  private:
    mutable x_iterator _x;
    mutable y_iterator _y;
  };


  template <typename XHandle, typename YHandle>
  class TrainingDataSet
  {
  public:
    using x_handle = XHandle;
    using y_handle = YHandle;

    using x_set_type = std::vector<XHandle>;
    using y_set_type = std::vector<YHandle>;

    inline TrainingDataSet() = default;

    inline void clear()
    {
      x.clear();
      y.clear();
    }

    inline bool operator==(const TrainingDataSet& other) const
    {
      return x == other.x && y == other.y;
    }

    inline bool operator!=(const TrainingDataSet& other) const
    {
      return !(*this == other);
    }

    x_set_type x;
    y_set_type y;
  };


  class ImageClassificationTrainingDataSet
      : public TrainingDataSet<std::string, int>
  {
    using base_type = TrainingDataSet<std::string, int>;

  public:
    using x_iterator = ImageDataSetIterator<Image<Rgb8>>;
    using y_iterator = typename y_set_type::const_iterator;

    using iterator = TrainingDataSetIterator<x_iterator, y_iterator>;

    inline ImageClassificationTrainingDataSet() = default;

    inline auto begin() const -> iterator
    {
      return iterator{x_begin(), y_begin()};
    }

    inline auto end() const -> iterator
    {
      return iterator{x_end(), y_end()};
    }

    auto x_begin() const -> x_iterator
    {
      return x_iterator{x.begin(), x.end()};
    }

    auto x_end() const -> x_iterator
    {
      return x_iterator{x.end(), x.end()};
    }

    auto y_begin() const -> y_iterator
    {
      return y.begin();
    }

    auto y_end() const -> y_iterator
    {
      return y.end();
    }
  };


  class ImageSegmentationTrainingDataSet
      : public TrainingDataSet<std::string, std::string>
  {
    using base_type = TrainingDataSet<std::string, std::string>;

  public:
    using x_set_type = typename base_type::x_set_type;
    using y_set_type = typename base_type::y_set_type;
    using x_iterator = ImageDataSetIterator<Image<Rgb8>>;
    using y_iterator = ImageDataSetIterator<Image<int>>;
    using iterator = TrainingDataSetIterator<x_iterator, y_iterator>;

    inline ImageSegmentationTrainingDataSet() = default;

    inline auto begin() const -> iterator
    {
      return iterator{x_begin(), y_begin()};
    }

    inline auto end() const -> iterator
    {
      return iterator{x_end(), y_end()};
    }

    auto x_begin() const -> x_iterator
    {
      return x_iterator{x.begin(), x.end()};
    }

    auto x_end() const -> x_iterator
    {
      return x_iterator{x.end(), x.end()};
    }

    auto y_begin() const -> y_iterator
    {
      return y_iterator{y.begin(), y.end()};
    }

    auto y_end() const -> y_iterator
    {
      return y_iterator{y.end(), y.end()};
    }
  };


  DO_SARA_EXPORT
  void read_from_csv(ImageClassificationTrainingDataSet& data_set,
                     const std::string& csv_filepath);

  DO_SARA_EXPORT
  void write_to_csv(const ImageClassificationTrainingDataSet& data_set,
                    const std::string& csv_filepath);

  DO_SARA_EXPORT
  void read_from_csv(ImageSegmentationTrainingDataSet& data_set,
                     const std::string& csv_filepath);

  DO_SARA_EXPORT
  void write_to_csv(const ImageSegmentationTrainingDataSet& data_set,
                    const std::string& csv_filepath);

} /* namespace Sara */
} /* namespace DO */
