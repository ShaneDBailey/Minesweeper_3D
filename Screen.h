#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "Model.h"
#include "Utilities.h"

class Screen {
private:
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;

public:
    Screen();
    ~Screen();

    void place_pixel(float x, float y);
    void draw_vertices(const Model& model);
    void draw_wireframe(const Model& model);
    void draw_line(const Vector3& start, const Vector3& end);
    void clear_display();
    void display();
    void input();
};

#endif // SCREEN_H
