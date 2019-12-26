#include <cmath>

#include "SDL.h"
#include "subsystem/Entities.h"
#include "subsystem/RNG.h"

/**
 * Entity
 * ------
 */
Entity::Entity() {
  id = Entity::total++;
}

int Entity::total = 0;

/**
 * Camera
 * ------
 */
Vec3f Camera::getDirection() const {
  return getOrientationDirection(orientation);
}

Vec3f Camera::getLeftDirection() const {
  constexpr float RAD_90 = 90.0f * M_PI / 180.0f;

  return getOrientationDirection({ 0, orientation.y - RAD_90, 0 });
}

Vec3f Camera::getOrientationDirection(const Vec3f& o) const {
  float pitch = o.x;
  float yaw = o.y;
  float roll = o.z;
  float p = std::abs(cosf(pitch));

  Vec3f direction = {
    -sinf(yaw) * p,
    sinf(pitch),
    cosf(yaw) * p
  };

  return direction.unit();
}

Vec3f Camera::getRightDirection() const {  
  constexpr float RAD_90 = 90.0f * M_PI / 180.0f;

  return getOrientationDirection({ 0, orientation.y + RAD_90, 0 });
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
  matrix = (
    Matrix4::translate({ position.x, position.y, -1.0f * position.z }) *
    Matrix4::rotate(orientation) *
    Matrix4::scale({ scale, scale, scale })
  ).transpose();
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
    vertex->color = { RNG::random(), RNG::random(), RNG::random() };

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