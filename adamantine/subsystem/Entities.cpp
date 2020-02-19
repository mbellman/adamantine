#include <cmath>
#include <vector>
#include <map>
#include <utility>

#include "SDL.h"
#include "subsystem/Entities.h"
#include "subsystem/RNG.h"
#include "subsystem/Geometry.h"

/**
 * Entity
 * ------
 */
Entity::Entity() {
  id = Entity::total++;
}

bool Entity::isExpired() const {
  return lifetime == 0.0f;
}

int Entity::total = 0;

/**
 * Light
 * -----
 */
Light::Light(const Vec3f& position, const Vec3f& color, float radius) {
  this->position = position;
  this->color = color;
  this->radius = radius;
}

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

Vec3f Camera::getOrientationDirection(const Vec3f& orientation) const {
  float pitch = orientation.x;
  float yaw = orientation.y;
  float roll = orientation.z;
  float pitchFactor = std::abs(cosf(pitch));

  Vec3f direction = {
    sinf(yaw) * pitchFactor,
    sinf(pitch),
    cosf(yaw) * pitchFactor
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

void Object::addPolygon(int v1index, int v2index, int v3index) {
  Polygon* polygon = new Polygon();

  polygon->vertices[0] = vertices[v1index];
  polygon->vertices[1] = vertices[v2index];
  polygon->vertices[2] = vertices[v3index];

  vertices[v1index]->polygons.push_back(polygon);
  vertices[v2index]->polygons.push_back(polygon);
  vertices[v3index]->polygons.push_back(polygon);

  polygons.push_back(polygon);
}

void Object::addVertex(const Vec3f& position) {
  addVertex(position, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
}

void Object::addVertex(const Vec3f& position, const Vec3f& color) {
  addVertex(position, color, { 0.0f, 0.0f });
}

void Object::addVertex(const Vec3f& position, const Vec2f& uv) {
  addVertex(position, { 0.0f, 0.0f, 0.0f }, uv);
}

void Object::addVertex(const Vec3f& position, const Vec3f& color, const Vec2f& uv) {
  Vertex3d* vertex = new Vertex3d();

  vertex->position = position;
  vertex->color = color;
  vertex->uv = uv;

  vertices.push_back(vertex);
}

const Matrix4& Object::getMatrix() const {
  return matrix;
}

const std::vector<Polygon*>& Object::getPolygons() const {
  return polygons;
}

const Object* Object::getReference() const {
  return reference;
}

void Object::recomputeMatrix() {
  matrix = (
    Matrix4::translate({ position.x, position.y, -1.0f * position.z }) *
    Matrix4::rotate(orientation) *
    Matrix4::scale({ scale, scale, scale })
  ).transpose();
}

void Object::rotate(const Vec3f& rotation) {
  orientation += rotation;

  recomputeMatrix();
}

void Object::setColor(const Vec3f& color) {
  for (auto* vertex : vertices) {
    vertex->color = color;
  }
}

void Object::setOrientation(const Vec3f& orientation) {
  this->orientation = orientation;

  recomputeMatrix();
}

void Object::setPosition(const Vec3f& position) {
  this->position = position;

  recomputeMatrix();
}

void Object::setReference(const Object* reference) {
  this->reference = reference;
}

void Object::setScale(float scale) {
  this->scale = scale;

  recomputeMatrix();
}

void Object::updateNormals() {
  for (auto* polygon : polygons) {
    polygon->updateNormal();
    polygon->updateTangent();
  }

  for (auto* vertex : vertices) {
    vertex->updateNormal();
    vertex->updateTangent();
  }
}

/**
 * Mesh
 * ----
 */
void Mesh::create(int w, int h, float tileSize) {
  width = w;
  height = h;

  // Add vertices
  Vec2f offset = {
    w * tileSize * -0.5f,
    h * tileSize * -0.5f
  };

  for (int i = 0; i < h + 1; i++) {
    for (int j = 0; j < w + 1; j++) {
      Vertex3d* vertex = new Vertex3d();
      float x = j * tileSize + offset.x;
      float z = i * tileSize + offset.y;

      float u = (float)j / (float)w;
      float v = (float)i / (float)h;

      addVertex({ x, 0.0f, z}, Vec3f(1.0f), { u, v });
    }
  }

  // Add polygons
  int totalPolygons = w * h * 2;

  for (int p = 0; p < totalPolygons; p++) {
    int tileIndex = p / 2;
    int rowIndex = tileIndex / w;
    int vOffset = rowIndex * (w + 1) + tileIndex % w;
    bool isTopPolygon = p % 2 == 0;
    int v1i = isTopPolygon ? vOffset : vOffset + 1;
    int v2i = vOffset + w + 1;
    int v3i = isTopPolygon ? vOffset + 1 : vOffset + w + 2;

    addPolygon(v1i, v2i, v3i);
  }

  updateNormals();
}

void Mesh::defineOffsets(std::function<void(Vec3f&, int, int)> offsetHandler) {
  for (int i = 0; i < (height + 1); i++) {
    for (int j = 0; j < (width + 1); j++) {
      int idx = i * (width + 1) + j;

      offsetHandler(vertices[idx]->position, j, i);
    }
  }

  updateNormals();
}

/**
 * Cube
 * ----
 */
Cube::Cube() {
  // Add faces
  for (int f = 0; f < 6; f++) {
    int (&face)[4] = Cube::faces[f];

    // Add face corners
    for (int c = 0; c < 4; c++) {
      int cornerIndex = face[c];
      Vec3f position = Cube::corners[cornerIndex];
      Vec3f color = Vec3f(1.0f);
      Vec2f uv = Cube::uvs[c];

      addVertex(position, color, uv);
    }
  }

  // Add polygons
  for (int f = 0; f < 6; f++) {
    int offset = f * 4;

    addPolygon(offset, offset + 1, offset + 2);
    addPolygon(offset, offset + 2, offset + 3);
  }

  updateNormals();
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

// UV coordinates for each cube face.
Vec2f Cube::uvs[4] = {
  { 1.0f, 1.0f }, // Bottom right
  { 0.0f, 1.0f }, // Bottom left
  { 0.0f, 0.0f }, // Top left
  { 1.0f, 0.0f }  // Top right
};

// The six cube faces, each defined by
// four corner indices.
int Cube::faces[6][4] = {
  { 1, 0, 3, 2 }, // Back
  { 7, 6, 2, 3 }, // Top
  { 4, 5, 6, 7 }, // Front
  { 0, 1, 5, 4 }, // Bottom
  { 0, 4, 7, 3 }, // Left
  { 5, 1, 2, 6 }  // Right
};

/**
 * Skybox
 * ------
 */
void Skybox::from(const Texture* texture) {
  this->texture = texture;
  this->isEmissive = true;

  for (int i = 0; i < 14; i++) {
    addVertex(Skybox::vertexPositions[i], Skybox::uvs[i]);
  }

  addPolygon(0, 1, 2);
  addPolygon(3, 4, 0);
  addPolygon(5, 6, 3);
  addPolygon(7, 8, 5);
  addPolygon(9, 3, 10);
  addPolygon(8, 11, 6);
  addPolygon(0, 4, 1);
  addPolygon(3, 6, 4);
  addPolygon(5, 8, 6);
  addPolygon(7, 12, 8);
  addPolygon(9, 5, 3);
  addPolygon(8, 13, 11);

  updateNormals();
}

Vec3f Skybox::vertexPositions[14] = {
  { -1.0f, -1.0f, -1.0f },
  { -1.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f, 1.0f },
  { 1.0f, -1.0f, -1.0f },
  { -1.0f, 1.0f, -1.0f },
  { 1.0f, -1.0f, 1.0f },
  { 1.0f, 1.0f, -1.0f },
  { -1.0f, -1.0f, 1.0f },
  { 1.0f, 1.0f, 1.0f },
  { -1.0f, -1.0f, 1.0f },
  { -1.0f, -1.0f, -1.0f },
  { -1.0f, 1.0f, -1.0f },
  { -1.0f, 1.0f, 1.0f },
  { -1.0f, 1.0f, 1.0f }
};

Vec2f Skybox::uvs[14] = {
  { 0.749978, 0.666474 },
  { 0.999934, 0.334000 },
  { 0.999934, 0.666474 },
  { 0.499000, 0.666474 },
  { 0.749978, 0.334000 },
  { 0.251000, 0.666476 },
  { 0.499000, 0.334000 },
  { 0.000108, 0.666476 },
  { 0.251000, 0.334000 },
  { 0.251000, 0.999427 },
  { 0.499000, 0.999427 },
  { 0.499000, 0.000572 },
  { 0.000108, 0.334000 },
  { 0.251000, 0.000572 }
};

/**
 * Model
 * -----
 */
void Model::from(const ObjLoader& loader) {
  bool hasTextureData = loader.textureCoordinates.size() > 0;

  if (hasTextureData) {
    buildTexturedModel(loader);
  } else {
    buildUntexturedModel(loader);
  }

  updateNormals();
}

void Model::from(const Model* reference) {
  setReference(reference);
}

void Model::buildTexturedModel(const ObjLoader& loader) {
  // Since there may be a different number of defined vertex
  // vectors and vertex texture coordinates (owing to the way
  // .obj files store vertex information), we have to examine
  // the vertex index + texture coordinate index tuples defined
  // for each face, map these to a vertex vector and texture
  // coordinate, create and add a single vertex per unique tuple,
  // and finally add our polygons once all vertices are set.

  // Track unique vertex/texture coordinate index pairs and their
  // associated Object vertex index
  std::map<std::pair<int, int>, int> uniqueVertexIndexMap;

  // Track the vertex indices for each polygon, so we can create
  // them after all vertices are defined
  std::vector<std::tuple<int, int, int>> polygonVertexIndices;

  for (const Face& face : loader.faces) {
    std::pair<int, int> v_vt_pairs[3];
    int vertexIndices[3];

    v_vt_pairs[0] = { face.v1.vertexIndex, face.v1.textureCoordinateIndex };
    v_vt_pairs[1] = { face.v2.vertexIndex, face.v2.textureCoordinateIndex };
    v_vt_pairs[2] = { face.v3.vertexIndex, face.v3.textureCoordinateIndex };

    for (int t = 0; t < 3; t++) {
      std::pair<int, int>& pair = v_vt_pairs[t];
      auto uniqueVertex = uniqueVertexIndexMap.find(pair);

      if (uniqueVertex != uniqueVertexIndexMap.end()) {
        vertexIndices[t] = uniqueVertex->second;
      } else {
        const Vec3f& vector = loader.vertices.at(v_vt_pairs[t].first);
        Vec2f uv = loader.textureCoordinates.at(v_vt_pairs[t].second);
        int index = vertices.size();

        vertexIndices[t] = index;
        uv.y = 1 - uv.y;

        addVertex(vector, uv);

        uniqueVertexIndexMap.emplace(pair, index);
      }
    }

    polygonVertexIndices.push_back({ vertexIndices[0], vertexIndices[1], vertexIndices[2] });
  }

  for (const auto& vertexIndices : polygonVertexIndices) {
    addPolygon(std::get<0>(vertexIndices), std::get<1>(vertexIndices), std::get<2>(vertexIndices));
  }

  uniqueVertexIndexMap.clear();
  polygonVertexIndices.clear();
}

void Model::buildUntexturedModel(const ObjLoader& loader) {
  for (int v = 0; v < loader.vertices.size(); v++) {
    Vec3f vector = loader.vertices.at(v);

    addVertex(vector, Vec3f(1.0));
  }

  for (int f = 0; f < loader.faces.size(); f++) {
    const Face& face = loader.faces.at(f);

    int v1 = face.v1.vertexIndex;
    int v2 = face.v2.vertexIndex;
    int v3 = face.v3.vertexIndex;

    addPolygon(v1, v2, v3);
  }
}