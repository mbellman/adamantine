#include "subsystem/Geometry.h"
#include "subsystem/Math.h"

/**
 * Vertex3d
 * --------
 */
void Vertex3d::updateNormal() {
  normal = Vec3f(0.0f);

  for (auto* polygon : polygons) {
    normal += polygon->normal;
  }
}

void Vertex3d::updateTangent() {
  tangent = Vec3f(0.0f);

  for (auto* polygon : polygons) {
    tangent += polygon->tangent;
  }
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
}

void Polygon::updateTangent() {
  const Vertex3d* v1 = vertices[0];
  const Vertex3d* v2 = vertices[1];
  const Vertex3d* v3 = vertices[2];

  Vec3f e1 = v2->position - v1->position;
  Vec3f e2 = v3->position - v1->position;

  float deltaU1 = v2->uv.x - v1->uv.x;
  float deltaV1 = v2->uv.y - v1->uv.y;
  float deltaU2 = v3->uv.x - v1->uv.x;
  float deltaV2 = v3->uv.y - v1->uv.y;

  float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

  tangent = {
    f * (deltaV2 * e1.x - deltaV1 * e2.x),
    f * (deltaV2 * e1.y - deltaV1 * e2.y),
    f * (deltaV2 * e1.z - deltaV1 * e2.z)
  };
}