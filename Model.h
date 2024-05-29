#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include "Utilities.h"

struct Vector3 {
    float x, y, z;
};

struct Vertex_Texture {
    float start, end;
};

struct Vertex_Normal {
    float x, y, z;
};

struct Face { //triangles only
    int vertexIndex[3]; 
    int textureIndex[3]; 
    int normalIndex[3]; 
};

class Model {
private:
    std::vector<Vector3> vertices;
    std::vector<Vertex_Texture> textures;
    std::vector<Vertex_Normal> normals;
    std::vector<Face> faces;

    Vector3 center_of_origin;
    float furthest_point;

public:
    Model();
    void normalize_vertices();
    void find_origin();
    void rotate(float x, float y, float z);

    const std::vector<Vector3>& getVertices() const;
    const std::vector<Vertex_Texture>& getTextures() const;
    const std::vector<Vertex_Normal>& getNormals() const;
    const std::vector<Face>& getFaces() const;
    const Vector3& getCenterOfOrigin() const;
};

#endif // MODEL_H
