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

/**
 * A 2D floating point vector.
 */
struct Vec2f {
  Vec2f() {};
  Vec2f(float x, float y) : x(x), y(y) {};

  float x = 0.0f;
  float y = 0.0f;
};

/**
 * A 3D floating point vector.
 */
struct Vec3f : Vec2f {
  Vec3f() {};
  Vec3f(float x, float y, float z) : Vec2f(x, y), z(z) {};

  float z = 0.0f;

  Vec3f operator+(const Vec3f& vector) const;
  Vec3f operator-(const Vec3f& vector) const;
  Vec3f operator*(float scalar) const;
};

/**
 * A 3x3 float matrix.
 */
struct Matrix3 {
  float m11, m12, m13, m21, m22, m23, m31, m32, m33;
};

/**
 * A quaternion.
 */
struct Quaternion {
  float w;
  float x;
  float y;
  float z;

  static Quaternion fromAxisAngle(float angle, float x, float y, float z);
  Matrix3 toMatrix3() const;
  Quaternion operator*(const Quaternion& q2) const;
};