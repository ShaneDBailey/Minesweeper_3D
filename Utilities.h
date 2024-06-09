#ifndef UTILITIES_H
#define UTILITIES_H

#include<tuple>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

struct Vector3 {
    float x, y, z;

    Vector3 operator-() const {
        return {-x, -y, -z};
    }
    Vector3 operator+() const {
        return {x, y, z};
    }
};

struct Color {
    float r,g,b,a;
};

float barycentric_interpolation(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2);
float dot_product(const Vector3& v1, const Vector3& v2);
bool is_point_inside_triangle(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2);
std::tuple<float, float, float> barycentric_weights(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2);

Vector3 normalize(const Vector3& v);
Vector3 reflect(const Vector3& I, const Vector3& N);

#endif