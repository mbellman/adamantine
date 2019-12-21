#pragma once

#include <vector>

#include "subsystem/Math.h"
#include "subsystem/Geometry.h"

struct Entity {
  virtual ~Entity() {};

  Vec3f position;
  Vec3f velocity;
  Vec3f orientation;

  template<typename T>
  bool isOfType() {
    return dynamic_cast<T*>(this) != 0;
  }
};

struct Camera : Entity {
  // TODO Create 'Orientation' struct with below methods
  Vec3f getDirection() const;
  Vec3f getLeftDirection() const;
  Vec3f getOrientationDirection(const Vec3f& o) const;
  Vec3f getRightDirection() const;
};

class Object : public Entity {
public:
  // TODO represent scale as a Vec3f
  float scale = 1.0f;

  virtual ~Object();

  const Matrix4& getMatrix() const;
  const std::vector<Polygon*>& getPolygons() const;
  void setScale(float scale);
  void setPosition(const Vec3f& position);

protected:
  std::vector<Vertex3d*> vertices;
  std::vector<Polygon*> polygons;
  Matrix4 matrix;

  void recomputeMatrix();
};

class Cube : public Object {
public:
  Cube();

private:
  static Vec3f corners[8];
  static int faces[6][4];
};