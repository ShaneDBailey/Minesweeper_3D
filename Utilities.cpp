#include "Utilities.h"


float dot_product(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


bool is_point_inside_triangle(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2) { 
    //barycentric coordinates:: found on internet somewhere
    float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float gamma = 1.0f - alpha - beta;
    return alpha > 0 && beta > 0 && gamma > 0; 
}

float barycentric_interpolation(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2) {
    float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float gamma = 1.0f - alpha - beta;
    return alpha * v0.z + beta * v1.z + gamma * v2.z;
}

std::tuple<float, float, float> barycentric_weights(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2) {
    float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y));
    float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y));
    float gamma = 1.0f - alpha - beta;
    return std::make_tuple(alpha, beta, gamma);
}

Vector3 normalize(const Vector3& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / length, v.y / length, v.z / length};
}

Vector3 reflect(const Vector3& I, const Vector3& N) {
    float dot = dot_product(I, N);
    return {I.x - 2 * dot * N.x, I.y - 2 * dot * N.y, I.z - 2 * dot * N.z};
}