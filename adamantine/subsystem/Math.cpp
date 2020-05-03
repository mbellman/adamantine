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

void Vec3f::operator+=(const Vec3f& vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;
}

Vec3f Vec3f::operator-(const Vec3f& vector) const {
  return {
    x - vector.x,
    y - vector.y,
    z - vector.z
  };
}

void Vec3f::operator-=(const Vec3f& vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
}

Vec3f Vec3f::operator*(float scalar) const {
  return {
    x * scalar,
    y * scalar,
    z * scalar
  };
}

Vec3f Vec3f::operator*(const Vec3f& vector) const {
  return {
    x * vector.x,
    y * vector.y,
    z * vector.z
  };
}

void Vec3f::operator*=(float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
}

Vec3f Vec3f::crossProduct(const Vec3f& v1, const Vec3f& v2) {
  return {
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  };
}

void Vec3f::debug() const {
  printf("[ %f ] [ %f ] [ %f ]\n", x, y, z);
}

const float* Vec3f::float3() const {
  return &x;
}

Vec3f Vec3f::gl() const {
  return *this * Vec3f(1.0f, 1.0f, -1.0f);
}

Vec3f Vec3f::invert() const {
  return *this * -1.0f;
}

float Vec3f::magnitude() const {
  return sqrtf(x*x + y*y + z*z);
}

Vec3f Vec3f::unit() const {
  float m = magnitude();

  return {
    x / m,
    y / m,
    z / m
  };
}

/**
 * Matrix4
 * -------
 */
void Matrix4::debug() const {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("[ %f ] ", m[i * 4 + j]);
    }

    printf("\n");
  }
}

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

Matrix4 Matrix4::lookAt(const Vec3f& eye, const Vec3f& direction, const Vec3f& top) {
  Vec3f forward = direction.unit();
  Vec3f right = Vec3f::crossProduct(top, forward).unit();
  Vec3f up = Vec3f::crossProduct(forward, right).unit();
  Matrix4 translation = Matrix4::translate(eye);

  Matrix4 rotation = {
    right.x, right.y, right.z, 0.0f,
    up.x, up.y, up.z, 0.0f,
    forward.x, forward.y, forward.z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  return rotation * translation;
}

Matrix4 Matrix4::orthographic(float top, float bottom, float left, float right, float near, float far) {
  return {
    2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
    0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
    0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
    0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4 Matrix4::projection(const Region2d<int>& area, float fov, float near, float far) {
  constexpr float PI = 3.14159265359;
  constexpr float DEG_TO_RAD = PI / 180.0f;
  float f = 1.0f / tanf(fov / 2.0f * DEG_TO_RAD);
  float aspectRatio = (float)area.width / (float)area.height;

  return {
    f / aspectRatio, 0.0f, 0.0f, 0.0f,
    0.0f, f, 0.0f, 0.0f,
    0.0f, 0.0f, (far + near) / (near - far), (2 * far * near) / (near - far),
    0.0f, 0.0f, -1.0f, 0.0f
  };
}

Matrix4 Matrix4::rotate(const Vec3f& rotation) {
  Quaternion pitch = Quaternion::fromAxisAngle(rotation.x, 1.0f, 0.0f, 0.0f);
  Quaternion yaw = Quaternion::fromAxisAngle(rotation.y, 0.0f, 1.0f, 0.0f);
  Quaternion roll = Quaternion::fromAxisAngle(rotation.z, 0.0f, 0.0f, 1.0f);

  return Matrix4::fromMatrix3((pitch * yaw * roll).toMatrix3());
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