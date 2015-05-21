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

//! @file

#ifndef DO_SARA_IMAGEPROCESSING_NORM_HPP
#define DO_SARA_IMAGEPROCESSING_NORM_HPP


#include <DO/Sara/Core/Image/Image.hpp>


namespace DO {

  /*!
    \ingroup Differential
    @{
   */

  /*!
    \brief Squared norm computation
    @param[in] src scalar field.
    @param[in, out] scalar field of squared norms
   */
  template <typename T, int M, int N, int D>
  void squared_norm(const Image<Matrix<T,M,N>, D>& src, Image<T, D>& dst)
  {
    if (dst.sizes() != src.sizes())
      dst.resize(src.sizes());

    typedef typename Image<Matrix<T,M,N>, D>::const_iterator InputIterator;
    typedef typename Image<T, D>::iterator OutputIterator;

    InputIterator src_it(src.begin());
    InputIterator src_it_end(src.end());
    OutputIterator dst_it(dst.begin());
    for ( ; src_it != src_it_end; ++src_it, ++dst_it)
      *dst_it = src_it->squaredNorm();
  }

  /*!
    \brief Squared norm computation
    @param[in] src scalar field.
    \return scalar field of squared norms
   */
  template <typename T, int M, int N, int D>
  Image<T, D> squared_norm(const Image<Matrix<T,M,N>, D>& src)
  {
    Image<T, D> squared_norm_image;
    squared_norm(src, squared_norm_image);
    return squared_norm_image;
  }

  /*!
    \brief Blue norm computation
    @param[in] src scalar field.
    @param[in, out] scalar field of norms
   */
  template <typename T, int M, int N, int D>
  void blue_norm(const Image<Matrix<T,M,N>, D>& src, Image<T, D>& dst)
  {
    if (dst.sizes() != src.sizes())
      dst.resize(src.sizes());

    typedef typename Image<Matrix<T,M,N>, D>::const_iterator InputIterator;
    typedef typename Image<T, D>::iterator OutputIterator;

    InputIterator src_it(src.begin());
    InputIterator src_it_end(src.end());
    OutputIterator dst_it(dst.begin());
    for ( ; src_it != src_it_end; ++src_it, ++dst_it)
      *dst_it = src_it->blueNorm();
  }

  /*!
    \brief Blue norm computation
    @param[in] src scalar field.
    \return scalar field of norms
   */
  template <typename T, int M, int N, int D>
  Image<T, D> blue_norm(const Image<Matrix<T,M,N>, D>& src)
  {
    Image<T, D> blue_norm_image;
    blue_norm(src, blue_norm_image);
    return blue_norm_image;
  }

  /*!
    \brief Stable norm computation
    @param[in] src scalar field.
    @param[in, out] scalar field of norms
   */
  template <typename T, int M, int N, int D>
  void stable_norm(const Image<Matrix<T,M,N>, D>& src, Image<T, D>& dst)
  {
    if (dst.sizes() != src.sizes())
      dst.resize(src.sizes());

    typedef typename Image<Matrix<T,M,N>, D>::const_iterator InputIterator;
    typedef typename Image<T, D>::iterator OutputIterator;

    InputIterator src_it(src.begin());
    InputIterator src_it_end(src.end());
    OutputIterator dst_it(dst.begin());
    for ( ; src_it != src_it_end; ++src_it, ++dst_it)
      *dst_it = src_it->stableNorm();
  }

  /*!
    \brief Stable norm computation
    @param[in] src scalar field.
    \return scalar field of norms
   */
  template <typename T, int M, int N, int D>
  Image<T, D> stable_norm(const Image<Matrix<T,M,N>, D>& src)
  {
    Image<T, D> stable_norm_image;
    stable_norm(src, stable_norm_image);
    return stable_norm_image;
  }

#define CREATE_NORM_FUNCTOR(Function, function)                       \
   /*! \brief Helper class to use Image<T,N>::compute<Function>() */  \
  template <typename T, int N>                                        \
  struct Function                                                     \
  {                                                                   \
    typedef typename T::Scalar scalar_type;                           \
    typedef Image<T, N> matrix_field_type;                            \
    typedef Image<scalar_type, N> scalar_field_type, return_type;     \
    inline Function(const matrix_field_type& matrix_field)            \
      : matrix_field_(matrix_field) {}                                \
    return_type operator()() const                                    \
    { return function(matrix_field_); }                               \
    const matrix_field_type& matrix_field_;                           \
  }

  CREATE_NORM_FUNCTOR(SquaredNorm, squared_norm);
  CREATE_NORM_FUNCTOR(BlueNorm, blue_norm);
  CREATE_NORM_FUNCTOR(StableNorm, stable_norm);

#undef CREATE_NORM_FUNCTOR

  //! @}

} /* namespace DO */

#endif /* DO_SARA_IMAGEPROCESSING_NORM_HPP */