#include "Screen.h"
#include "Model.h"

Screen::Screen() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
}

Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::place_pixel(float x, float y) {
    points.emplace_back(SDL_FPoint{x, y});
}

void Screen::drawModel(const Model& model) {
    // Iterate over the faces of the model
    for (const auto& face : model.getFaces()) {
        // Get vertices of the face
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0]];
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1]];
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2]];
        place_pixel(v0.x, v0.y);
        place_pixel(v1.x, v1.y);
        place_pixel(v2.x, v2.y);
    
    }
}

void Screen::clear_display() {
    points.clear();
}

void Screen::display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(auto& point : points) {
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }

    SDL_RenderPresent(renderer);
}

void Screen::input() {
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
    }
}
