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

/**
 * Defines a range between two values.
 *
 * @template T - A custom numeric or complex numeric type.
 */
template<typename T>
struct Range {
  T start;
  T end;
};