#pragma once

#include <vector>

#include "subsystem/Math.h"
#include "subsystem/Geometry.h"

struct Entity {
  Vec3f position;
  Vec3f velocity;
  Vec3f orientation;
};

struct Camera : Entity {
  Matrix3 getRotationMatrix() const;
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