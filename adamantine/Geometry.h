#pragma once

/**
 * Defines a region of 2D space.
 *
 * @template T - A custom numeric type allowing for variable precision.
 */
template<typename T>
struct Region2d {
  T x;
  T y;
  T width;
  T height;
};