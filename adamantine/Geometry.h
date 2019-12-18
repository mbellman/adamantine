#pragma once

#include "Math.h"

/**
 * A 2D triangle vertex.
 */
struct Vertex2d {
  Vec2f position;
  Vec3f color;
};

/**
 * A 3D polygon vertex.
 */
struct Vertex3d {
  Vertex3d() {};

  Vec3f position;
  Vec3f color;
};

/**
 * A polygon consisting of three 3D vertices. Since
 * polygons can share vertices, each vertex is held
 * by reference.
 */
struct Polygon {
  Polygon() {};
  Polygon(Vertex3d* v1, Vertex3d* v2, Vertex3d* v3) : v1(v1), v2(v2), v3(v3) {};

  Vertex3d* v1 = nullptr;
  Vertex3d* v2 = nullptr;
  Vertex3d* v3 = nullptr;
};