#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Model.h"
#include "Utilities.h"

class Screen {
private:
    SDL_Event event;
    SDL_Window* window;
    std::vector<SDL_FPoint> points;

public:
    SDL_Renderer* renderer;
    Screen();
    ~Screen();

    void draw_vertices(const Model& model);
    void draw_wireframe(const Model& model);
    void draw_scanline(float x1, float x2, int y, const Color& color);
    bool is_point_inside_triangle(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2);
    void draw_triangle_rasterization(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    void draw_face_rasterization(const Model& model) ;
    void draw_face(const Model& model);
    void draw_horizontal_line(float left_x, float right_x, float height);
    float interpolateX(const Vector3& top_vertex, const Vector3& bottom_vertex, const Vector3& vertex, int y);
    void draw_line(const Vector3& start, const Vector3& end);
    void clear_display();
    void input();
};

#endif // SCREEN_H
