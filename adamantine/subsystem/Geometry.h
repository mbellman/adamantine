#pragma once

#include <vector>

#include "Math.h"

struct Polygon;

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
  Vec2f uv;
  Vec3f normal;
  std::vector<Polygon*> polygons;

  void updateNormal();
};

/**
 * A polygon consisting of three 3D vertices. Since
 * polygons can share vertices, each vertex is held
 * by reference.
 */
struct Polygon {
  Polygon() {};
  Polygon(Vertex3d* v1, Vertex3d* v2, Vertex3d* v3);

  Vertex3d* vertices[3];
  Vec3f normal;

  void updateNormal();
};