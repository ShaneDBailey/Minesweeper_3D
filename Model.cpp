#include "Model.h"

Model::Model() {
    float size = 120;

    vertices = {
        {-1, -1, -1}, // 0
        {-1, -1, 1},  // 1
        {-1, 1 , -1},  // 2
        {-1, 1, 1},   // 3
        {1, -1, -1},  // 4
        {1, -1, 1},   // 5
        {1, 1, -1},   // 6
        {1, 1, 1}     // 7
    };
    normalize_vertices();
    faces = {
        // Front face
        {{0, 1, 2}, {1, 0, 0}, {0, 0, 0}},
        {{1, 3, 2}, {0, 0, 0}, {0, 0, 0}},
        // Back face
        {{4, 5, 6}, {0, 0, 0}, {0, 0, 0}},
        {{5, 7, 6}, {0, 0, 0}, {0, 0, 0}},
        // Left face
        {{0, 4, 2}, {0, 0, 0}, {0, 0, 0}},
        {{4, 6, 2}, {0, 0, 0}, {0, 0, 0}},
        // Right face
        {{1, 5, 3}, {0, 0, 0}, {0, 0, 0}},
        {{5, 7, 3}, {0, 0, 0}, {0, 0, 0}},
        // Top face
        {{2, 3, 6}, {0, 0, 0}, {0, 0, 0}},
        {{3, 7, 6}, {0, 0, 0}, {0, 0, 0}},
        // Bottom face
        {{0, 1, 4}, {0, 0, 0}, {0, 0, 0}},
        {{1, 5, 4}, {0, 0, 0}, {0, 0, 0}}
    };

    find_origin();
    std::cout <<  this->center_of_origin.x << ", " << this->center_of_origin.y << ", " << this->center_of_origin.z << ", " <<std::endl;
}

void Model::normalize_vertices(){
    for(auto& vertex : this->vertices){
        this->furthest_point = std::max(this->furthest_point, std::abs(vertex.x));
        this->furthest_point = std::max(this->furthest_point, std::abs(vertex.y));
        this->furthest_point = std::max(this->furthest_point, std::abs(vertex.z));
    }

    // Normalize each coordinate of the vertices
    for(auto& vertex : this->vertices){
        vertex.x /= this->furthest_point;
        vertex.y /= this->furthest_point;
        vertex.z /= this->furthest_point;

        vertex.x = vertex.x*SCREEN_WIDTH/4 + SCREEN_WIDTH/2;
        vertex.y = vertex.y*SCREEN_WIDTH/4 + SCREEN_WIDTH/2;
        vertex.z = vertex.z*SCREEN_WIDTH/4 + SCREEN_WIDTH/2;
    }
}

void Model::find_origin() {
    float origin_x = 0, origin_y = 0, origin_z = 0;

    for (auto& vertex : this->vertices) {
        origin_x += vertex.x;
        origin_y += vertex.y;
        origin_z += vertex.z;
    }
    this->center_of_origin = {
        origin_x / this->vertices.size(),
        origin_y / this->vertices.size(),
        origin_z / this->vertices.size()
    };
}

void Model::rotate(float x, float y, float z){
    for(auto& vertex : this->vertices){
        // Translate to the origin
        vertex.x -= this->center_of_origin.x;
        vertex.y -= this->center_of_origin.y;
        vertex.z -= this->center_of_origin.z;

        // X rotation
        float temp_y = vertex.y;
        vertex.y = std::cos(x) * vertex.y - std::sin(x) * vertex.z;
        vertex.z = std::sin(x) * temp_y + std::cos(x) * vertex.z;

        // Y rotation
        float temp_x = vertex.x;
        vertex.x = std::cos(y) * vertex.x + std::sin(y) * vertex.z;
        vertex.z = -std::sin(y) * temp_x + std::cos(y) * vertex.z;

        // Z rotation
        temp_x = vertex.x;
        vertex.x = std::cos(z) * vertex.x - std::sin(z) * vertex.y;
        vertex.y = std::sin(z) * temp_x + std::cos(z) * vertex.y;

        // Translate back
        vertex.x += this->center_of_origin.x;
        vertex.y += this->center_of_origin.y;
        vertex.z += this->center_of_origin.z;
    }
}

const std::vector<Vector3>& Model::getVertices() const {
    return vertices;
}

const std::vector<Vertex_Texture>& Model::getTextures() const {
    return textures;
}

const std::vector<Vertex_Normal>& Model::getNormals() const {
    return normals;
}

const std::vector<Face>& Model::getFaces() const {
    return faces;
}

const Vector3& Model::getCenterOfOrigin() const {
    return center_of_origin;
}
