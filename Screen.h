#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "Model.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;


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
    void drawModel(const Model& model);
    void clear_display();
    void display();
    void input();
};

#endif // SCREEN_H
