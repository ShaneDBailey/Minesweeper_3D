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

void Screen::draw_vertices(const Model& model) {

    for (const auto& face : model.getFaces()) {
        
        const Vector3& vertex_0 = model.getVertices()[face.vertexIndex[0] -1]; // -1 to correct 0
        const Vector3& vertex_1 = model.getVertices()[face.vertexIndex[1] -1];
        const Vector3& vertex_2 = model.getVertices()[face.vertexIndex[2] -1];
        Color& color = face.face_material.diffuseColor;
        SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
        SDL_RenderDrawPointF(renderer,vertex_0.x, vertex_0.y);
        SDL_RenderDrawPointF(renderer,vertex_1.x, vertex_1.y);
        SDL_RenderDrawPointF(renderer,vertex_2.x, vertex_2.y);
    }
}
void Screen::draw_wireframe(const Model& model) {
    for (const auto& face : model.getFaces()) {
        const Vector3& vertex_0 = model.getVertices()[face.vertexIndex[0] -1]; // -1 to correct 0
        const Vector3& vertex_1 = model.getVertices()[face.vertexIndex[1] -1];
        const Vector3& vertex_2 = model.getVertices()[face.vertexIndex[2] -1];
        Color& color = face.face_material.diffuseColor;
        SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

        draw_line(vertex_0, vertex_1);
        draw_line(vertex_1, vertex_2);
        draw_line(vertex_2, vertex_0);
    }
}

void Screen::draw_face(const Model& model) {
    // Iterate over each face
    for (const auto& face : model.getFaces()) {
        // Retrieve the vertices of the face
        const Vector3& vertex_0 = model.getVertices()[face.vertexIndex[0] -1]; // -1 to correct 0
        const Vector3& vertex_1 = model.getVertices()[face.vertexIndex[1] -1];
        const Vector3& vertex_2 = model.getVertices()[face.vertexIndex[2] -1];

        Color& color = face.face_material.diffuseColor;
        SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
        
        Vector3 top = vertex_0, middle = vertex_1, bottom = vertex_2;
        if (top.y > middle.y) std::swap(top, middle);
        if (top.y > bottom.y) std::swap(top, bottom);
        if (middle.y > bottom.y) std::swap(middle, bottom);

        float slope_top_middle = (middle.x - top.x) / (middle.y - top.y);
        float slope_top_bottom = (bottom.x - top.x) / (bottom.y - top.y);
        float slope_middle_bottom = (bottom.x - middle.x) / (bottom.y - middle.y);

        float left_x = bottom.x;
        float right_x = bottom.x;

        for(float height = bottom.y; height >= middle.y; height-- ){
            if(height >= middle.y){
                draw_horizontal_line(left_x, right_x, height);
                left_x -= slope_middle_bottom;
                right_x -= slope_top_bottom;
            }
        }

        left_x = top.x;
        right_x = top.x;

        for(float height = top.y; height <= middle.y; height++ ){
            if(height <= middle.y){
                draw_horizontal_line(left_x, right_x, height);
                left_x += slope_top_middle;
                right_x += slope_top_bottom;
            }
        }

        
    }
}


void Screen::draw_horizontal_line(float left_x, float right_x, float height){
    if(left_x > right_x){
        float temp = left_x;
        left_x = right_x;
        right_x = temp;
    }
    if(left_x >= 0 && left_x < 680 && right_x > 0 && right_x < 680){
        for(float start = left_x; start <= right_x; start++){
            SDL_RenderDrawPointF(renderer,start, height);
        }
    }else{
        std::cout << left_x << "  " << right_x << std::endl;
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
        SDL_RenderDrawPointF(renderer,start_x, start_y);

        if (start_x == end_x && start_y == end_y){
            break;
        }

        int twice_deviation = 2 * deviation_from_line;
        if (twice_deviation > -abs_distance_y) { deviation_from_line -= abs_distance_y; start_x += direction_x; }
        if (twice_deviation < abs_distance_x) { deviation_from_line += abs_distance_x; start_y += direction_y; }
    }
}

void Screen::clear_display() {
    SDL_SetRenderDrawColor(renderer, 115,155,155, 255);
    SDL_RenderClear(renderer);
}

void Screen::input() {
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
    }
}
