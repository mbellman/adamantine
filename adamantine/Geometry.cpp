#include "Geometry.h"

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