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

void Screen::draw_vertices(const Model& model) {

    for (const auto& face : model.getFaces()) {
        
        const Vector3& vertex_0 = model.getVertices()[face.vertexIndex[0]];
        const Vector3& vertex_1 = model.getVertices()[face.vertexIndex[1]];
        const Vector3& vertex_2 = model.getVertices()[face.vertexIndex[2]];
        
        place_pixel(vertex_0.x, vertex_0.y);
        place_pixel(vertex_1.x, vertex_1.y);
        place_pixel(vertex_2.x, vertex_2.y);
    
    }
}
void Screen::draw_wireframe(const Model& model) {
    for (const auto& face : model.getFaces()) {
        const Vector3& vertex_0 = model.getVertices()[face.vertexIndex[0] -1]; // -1 to correct 0 index
        const Vector3& vertex_1 = model.getVertices()[face.vertexIndex[1] -1];
        const Vector3& vertex_2 = model.getVertices()[face.vertexIndex[2] -1];

        draw_line(vertex_0, vertex_1);
        draw_line(vertex_1, vertex_2);
        draw_line(vertex_2, vertex_0);
    }
}

void Screen::draw_line(const Vector3& start, const Vector3& end) {
    // Bresenham's line algorithm
    int start_x = static_cast<int>(start.x);
    int start_y = static_cast<int>(start.y);
    int end_x = static_cast<int>(end.x);
    int end_y = static_cast<int>(end.y);

    int abs_distance_x = abs(end_x - start_x);
    int abs_distance_y = abs(end_y - start_y);
    int direction_x = (start_x < end_x) ? 1 : -1;
    int direction_y = (start_y < end_y) ? 1 : -1;
    int deviation_from_line = abs_distance_x - abs_distance_y;

    while (true) {
        place_pixel(start_x, start_y);

        if (start_x == end_x && start_y == end_y){
            break;
        }

        int twice_deviation = 2 * deviation_from_line;
        if (twice_deviation > -abs_distance_y) { deviation_from_line -= abs_distance_y; start_x += direction_x; }
        if (twice_deviation < abs_distance_x) { deviation_from_line += abs_distance_x; start_y += direction_y; }
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
