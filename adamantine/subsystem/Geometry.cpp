#include "subsystem/Geometry.h"

/**
 * Polygon
 * -------
 */
Polygon::Polygon(Vertex3d* v1, Vertex3d* v2, Vertex3d* v3) {
  vertices[0] = v1;
  vertices[1] = v2;
  vertices[2] = v3;
}