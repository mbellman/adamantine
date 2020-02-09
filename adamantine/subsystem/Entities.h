#pragma once

#include <vector>
#include <functional>

#include "subsystem/Math.h"
#include "subsystem/Geometry.h"
#include "subsystem/ObjLoader.h"
#include "subsystem/Texture.h"

struct Entity {
  Entity();
  virtual ~Entity() {};

  static int total;
  int id;
  Vec3f position;
  Vec3f orientation;
  std::function<void(float)> onUpdate = nullptr;

  template<typename T>
  bool isOfType() {
    return dynamic_cast<T*>(this) != 0;
  }
};

struct Light : Entity {
  enum LightType {
    POINT = 0,
    DIRECTIONAL = 1,
    SPOTLIGHT = 2
  };

  Light() {};
  Light(const Vec3f& position, const Vec3f& color, float radius);

  LightType type = LightType::POINT;
  Vec3f color = Vec3f(1.0f);
  Vec3f direction;
  float radius = 100.0f;
  bool canCastShadows = false;
};

class Camera : public Entity {
public:
  // TODO Create 'Orientation' struct with below methods
  Vec3f getDirection() const;
  Vec3f getLeftDirection() const;
  Vec3f getOrientationDirection(const Vec3f& orientation) const;
  Vec3f getRightDirection() const;
};

class Object : public Entity {
public:
  // TODO represent scale as a Vec3f
  float scale = 1.0f;
  const Texture* texture = nullptr;
  const Texture* normalMap = nullptr;

  virtual ~Object();

  const Matrix4& getMatrix() const;
  const std::vector<Polygon*>& getPolygons() const;
  void rotate(const Vec3f& rotation);
  void setColor(const Vec3f& color);
  void setOrientation(const Vec3f& orientation);
  void setPosition(const Vec3f& position);
  void setScale(float scale);

protected:
  std::vector<Vertex3d*> vertices;
  std::vector<Polygon*> polygons;
  Matrix4 matrix;

  void addPolygon(int v1index, int v2index, int v3index);
  void addVertex(const Vec3f& position);
  void addVertex(const Vec3f& position, const Vec3f& color);
  void addVertex(const Vec3f& position, const Vec2f& uv);
  void addVertex(const Vec3f& position, const Vec3f& color, const Vec2f& uv);
  void recomputeMatrix();
  void updateNormals();
};

class Mesh : public Object {
public:
  Mesh(int w, int h, float tileSize);

  void defineOffsets(std::function<void(Vec3f&, int, int)> offsetHandler);

private:
  int width;
  int height;
};

class Cube : public Object {
public:
  Cube();

private:
  static Vec3f corners[8];
  static Vec2f uvs[4];
  static int faces[6][4];
};

class Skybox : public Object {
public:
  Skybox(const Texture* texture, float size);

private:
  static Vec3f vertexPositions[14];
  static Vec2f uvs[14];
};

class Model : public Object {
public:
  Model(const ObjLoader& loader);

private:
  void buildTexturedModel(const ObjLoader& loader);
  void buildUntexturedModel(const ObjLoader& loader);
};