#include "subsystem/Entities.h"

/**
 * Camera
 * ------
 */
Matrix3 Camera::getRotationMatrix() const {
  Quaternion q1 = Quaternion::fromAxisAngle(orientation.x, 1, 0, 0);
  Quaternion q2 = Quaternion::fromAxisAngle(orientation.y, 0, 1, 0);
  Quaternion q3 = Quaternion::fromAxisAngle(orientation.z, 0, 0, 1);

  return (q1 * q2 * q2).toMatrix3();
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