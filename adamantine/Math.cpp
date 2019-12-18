#include <cmath>

#include "Math.h"

/**
 * Vec3f
 * -----
 */
Vec3f Vec3f::operator+(const Vec3f& vector) const {
  return {
    x + vector.x,
    y + vector.y,
    z + vector.z
  };
}

Vec3f Vec3f::operator-(const Vec3f& vector) const {
  return {
    x - vector.x,
    y - vector.y,
    z - vector.z
  };
}

Vec3f Vec3f::operator*(float scalar) const {
  return {
    x * scalar,
    y * scalar,
    z * scalar
  };
}

/**
 * Quaternion
 * ----------
 */
Quaternion Quaternion::fromAxisAngle(float angle, float x, float y, float z) {
  float sa = sinf(angle / 2);

  return {
    cosf(angle / 2),
    x * sa,
    y * sa,
    z * sa
  };
}

Matrix3 Quaternion::toMatrix3() const {
  return {
    1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w,
    2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w,
    2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y
  };
}

Quaternion Quaternion::operator*(const Quaternion& q2) const {
  return {
    w * q2.w - x * q2.x - y * q2.y - z * q2.z,
    w * q2.x + x * q2.w + y * q2.z - z * q2.y,
    w * q2.y - x * q2.z + y * q2.w + z * q2.x,
    w * q2.z + x * q2.y - y * q2.x + z * q2.w
  };
}