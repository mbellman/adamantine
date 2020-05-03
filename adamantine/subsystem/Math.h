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
  Vec3f(float f) : Vec2f(f, f), z(f) {};
  Vec3f(float x, float y, float z) : Vec2f(x, y), z(z) {};

  float z = 0.0f;

  static Vec3f crossProduct(const Vec3f& v1, const Vec3f& v2);

  Vec3f operator+(const Vec3f& vector) const;
  void operator+=(const Vec3f& vector);
  Vec3f operator-(const Vec3f& vector) const;
  void operator-=(const Vec3f& vector);
  Vec3f operator*(float scalar) const;
  Vec3f operator*(const Vec3f& vector) const;
  void operator*=(float scalar);

  void debug() const;
  const float* float3() const;
  Vec3f gl() const;
  Vec3f invert() const;
  float magnitude() const;
  Vec3f unit() const;
};

/**
 * A 3x3 transformation matrix.
 */
struct Matrix3 {
  float m11, m12, m13, m21, m22, m23, m31, m32, m33;
};

/**
 * A 4x4 transformation matrix.
 */
struct Matrix4 {
  float m[16];

  static Matrix4 fromMatrix3(const Matrix3& matrix);
  static Matrix4 identity();
  static Matrix4 lookAt(const Vec3f& eye, const Vec3f& direction, const Vec3f& top);
  static Matrix4 orthographic(float top, float bottom, float left, float right, float near, float far);
  static Matrix4 projection(const Region2d<int>& area, float fov, float near, float far);
  static Matrix4 rotate(const Vec3f& rotation);
  static Matrix4 scale(const Vec3f& scale);
  static Matrix4 translate(const Vec3f& translation);

  Matrix4 operator*(const Matrix4& matrix) const;

  void debug() const;
  Matrix4 transpose() const;
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