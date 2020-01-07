#include "subsystem/Geometry.h"
#include "subsystem/Math.h"

/**
 * Vertex3d
 * --------
 */
void Vertex3d::updateNormal() {
  Vec3f normal;

  for (auto* polygon : polygons) {
    normal += polygon->normal;
  }

  this->normal = normal.unit();
}

/**
 * Polygon
 * -------
 */
Polygon::Polygon(Vertex3d* v1, Vertex3d* v2, Vertex3d* v3) {
  vertices[0] = v1;
  vertices[1] = v2;
  vertices[2] = v3;
}

void Polygon::updateNormal() {
  const Vec3f& v1 = vertices[0]->position;
  const Vec3f& v2 = vertices[1]->position;
  const Vec3f& v3 = vertices[2]->position;

  normal = Vec3f::crossProduct(v2 - v1, v3 - v1);
  normal.z *= -1.0f;
}