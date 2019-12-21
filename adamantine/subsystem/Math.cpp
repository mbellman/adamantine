#include <cmath>
#include <cstdio>

#include "subsystem/Math.h"

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
 * Matrix4
 * -------
 */
Matrix4 Matrix4::fromMatrix3(const Matrix3& matrix) {
  return {
    matrix.m11, matrix.m12, matrix.m13, 0.0f,
    matrix.m21, matrix.m22, matrix.m23, 0.0f,
    matrix.m31, matrix.m32, matrix.m33, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4 Matrix4::identity() {
  return {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4 Matrix4::rotate(const Vec3f& rotation) {
  Quaternion q1 = Quaternion::fromAxisAngle(rotation.x, 1.0f, 0.0f, 0.0f);
  Quaternion q2 = Quaternion::fromAxisAngle(rotation.y, 0.0f, 1.0f, 0.0f);
  Quaternion q3 = Quaternion::fromAxisAngle(rotation.z, 0.0f, 0.0f, 1.0f);

  return Matrix4::fromMatrix3((q1 * q2 * q3).toMatrix3());
}

Matrix4 Matrix4::scale(const Vec3f& scale) {
  return {
    scale.x, 0.0f, 0.0f, 0.0f,
    0.0f, scale.y, 0.0f, 0.0f,
    0.0f, 0.0f, scale.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4 Matrix4::translate(const Vec3f& translation) {
  return {
    1.0f, 0.0f, 0.0f, translation.x,
    0.0f, 1.0f, 0.0f, translation.y,
    0.0f, 0.0f, 1.0f, translation.z,
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4 Matrix4::operator*(const Matrix4& matrix) const {
  Matrix4 product;

  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 4; c++) {
      float& value = product.m[r * 4 + c] = 0;

      for (int n = 0; n < 4; n++) {
        value += m[r * 4 + n] * matrix.m[n * 4 + c];
      }
    }
  }

  return product;
}

void Matrix4::debug() const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("[ %f ] ", m[i * 4 + j]);
    }

    printf("\n");
  }
}

Matrix4 Matrix4::transpose() const {
  return {
    m[0], m[4], m[8], m[12],
    m[1], m[5], m[9], m[13],
    m[2], m[6], m[10], m[14],
    m[3], m[7], m[11], m[15]
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