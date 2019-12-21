#include <cmath>

#include "SDL.h"
#include "subsystem/Entities.h"

/**
 * Camera
 * ------
 */
Vec3f Camera::getDirection() const {
  return getOrientationDirection(orientation);
}

Vec3f Camera::getLeftDirection() const {
  return getOrientationDirection({
    orientation.x,
    orientation.y - 90.0f,
    orientation.z
  });
}

Vec3f Camera::getOrientationDirection(const Vec3f& o) const {
  constexpr float DEG_TO_RAD = M_PI / 180.0f;
  float pitch = o.x * DEG_TO_RAD;
  float yaw = o.y * DEG_TO_RAD;
  float roll = o.z * DEG_TO_RAD;
  float p = std::abs(cosf(pitch));

  Vec3f direction = {
    -sinf(yaw) * p,
    sinf(pitch),
    cosf(yaw) * p
  };

  return direction.unit();
}

Vec3f Camera::getRightDirection() const {
  return getOrientationDirection({
    orientation.x,
    orientation.y + 90.0f,
    orientation.z
  });
}

/**
 * Object
 * ------
 */
Object::~Object() {
  for (auto* polygon : polygons) {
    delete polygon;
  }

  for (auto* vertex : vertices) {
    delete vertex;
  }

  polygons.clear();
  vertices.clear();
}

const Matrix4& Object::getMatrix() const {
  return matrix;
}

const std::vector<Polygon*>& Object::getPolygons() const {
  return polygons;
}

void Object::recomputeMatrix() {
  matrix = (Matrix4::translate(position) * Matrix4::rotate(orientation) * Matrix4::scale({ scale, scale, scale })).transpose();
}

void Object::setScale(float scale) {
  this->scale = scale;

  recomputeMatrix();
}

void Object::setPosition(const Vec3f& position) {
  this->position = position;

  recomputeMatrix();
}

/**
 * Cube
 * ----
 */
Cube::Cube() {
  // Add vertices
  for (int c = 0; c < 8; c++) {
    Vec3f position = Cube::corners[c];
    Vertex3d* vertex = new Vertex3d();

    vertex->position = position;
    vertex->color = { 1.0f, 1.0f, 1.0f };

    vertices.push_back(vertex);
  }

  // Add polygons
  for (int f = 0; f < 6; f++) {
    int (&corners)[4] = Cube::faces[f];
    Vertex3d* verts[4];

    for (int i = 0; i < 4; i++) {
      verts[i] = vertices[corners[i]];
    }

    polygons.push_back(new Polygon(verts[0], verts[1], verts[2]));
    polygons.push_back(new Polygon(verts[0], verts[3], verts[2]));
  }
}

// Unit cube corners as 3-vectors.
Vec3f Cube::corners[8] = {
  { -1.0f, -1.0f, -1.0f },
  { 1.0f, -1.0f, -1.0f },
  { 1.0f, 1.0f, -1.0f },
  { -1.0f, 1.0f, -1.0f },
  { -1.0f, -1.0f, 1.0f },
  { 1.0f, -1.0f, 1.0f },
  { 1.0f, 1.0f, 1.0f },
  { -1.0f, 1.0f, 1.0f }
};

// The six cube faces, each defined by
// four corner indices.
int Cube::faces[6][4] = {
  { 0, 1, 2, 3 }, // Back
  { 3, 2, 6, 7 }, // Top
  { 7, 6, 5, 4 }, // Front
  { 4, 5, 1, 0 }, // Bottom
  { 0, 4, 7, 3 }, // Left
  { 5, 1, 2, 6 }  // Right
};