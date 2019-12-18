#pragma once

#include <vector>

#include "Math.h"
#include "Geometry.h"

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
  float scale;

  virtual ~Object();

  const std::vector<Polygon*>& getPolygons() const;

protected:
  std::vector<Vertex3d*> vertices;
  std::vector<Polygon*> polygons;
};

class Cube : public Object {
public:
  Cube();

private:
  static Vec3f corners[8];
  static int faces[6][4];
};