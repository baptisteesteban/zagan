#pragma once

namespace zagan
{
  template <typename I>
  using image_value_t = typename I::value_t;

  template <typename I>
  using image_point_t = typename I::point_t;

  template <typename I>
  using image_domain_t = typename I::domain_t;
} // namespace zagan