#include "Screen.h"
#include "Model.h"

Screen::Screen() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT; ++j) {
            zBuffer[i][j] = std::numeric_limits<float>::min();
        }
    }
}

Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen::clear_display() {
    SDL_SetRenderDrawColor(renderer, 115,155,155, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT; ++j) {
            zBuffer[i][j] = std::numeric_limits<float>::min();
        }
    }
}

void Screen::input() {
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
    }
}

bool is_point_inside_triangle(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2) { 
    //barycentric coordinates:: found on internet somewhere
    float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float gamma = 1.0f - alpha - beta;
    return alpha > 0 && beta > 0 && gamma > 0; 
}

float dot_product(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
//---------------------------------------draw_vertices------------------------
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
//---------------------------------------draw_wireframe------------------------
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
//---------------------------------------draw_triangles------------------------
void Screen::draw_face_rasterization(const Model& model) { 
    for (const auto& face : model.getFaces()) { 
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0] - 1]; 
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1] - 1]; 
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2] - 1]; 

        Color& color = face.face_material.diffuseColor; 
        SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255); 

        int min_x = std::min({v0.x, v1.x, v2.x}); 
        int min_y = std::min({v0.y, v1.y, v2.y}); 
        int max_x = std::max({v0.x, v1.x, v2.x}); 
        int max_y = std::max({v0.y, v1.y, v2.y}); 

        for (int y = min_y; y <= max_y; ++y) { 
            for (int x = min_x; x <= max_x; ++x) { 
                if (is_point_inside_triangle(x, y, v0, v1, v2)) { 
                    SDL_RenderDrawPointF(renderer, x, y); 
                } 
            }
        }
    } 
} 

//---------------------------------draw_triangles_flat_shading--------------------

void Screen::draw_face_rainbow(const Model& model) {
    for (const auto& face : model.getFaces()) {
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0] - 1];
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1] - 1];
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2] - 1];

        const Vector3& face_normal = model.getNormals()[face.normalIndex[0] - 1];
        Color color;
        color.r = (face_normal.x + 1.0) * 0.5 * 255;
        color.g = (face_normal.y + 1.0) * 0.5 * 255;
        color.b = (face_normal.z + 1.0) * 0.5 * 255;
        color.a = 255;

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        int min_x = std::min({v0.x, v1.x, v2.x});
        int min_y = std::min({v0.y, v1.y, v2.y});
        int max_x = std::max({v0.x, v1.x, v2.x});
        int max_y = std::max({v0.y, v1.y, v2.y});

        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                if (is_point_inside_triangle(x, y, v0, v1, v2)) {
                    float z = barycentric_interpolation(x, y, v0, v1, v2);
                    SDL_RenderDrawPointF(renderer, x, y);
                }
            }
        }
    }
}


Vector3 light_direction{-1, 0, 1}; //must be between -1 and 1

void Screen::draw_face_flat_shading(const Model& model) {
    // Normalize the light direction
    float length = std::sqrt(light_direction.x * light_direction.x +
                             light_direction.y * light_direction.y +
                             light_direction.z * light_direction.z);
    Vector3 normalized_light_direction = {
        light_direction.x / length,
        light_direction.y / length,
        light_direction.z / length
    };

    for (const auto& face : model.getFaces()) {
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0] - 1];
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1] - 1];
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2] - 1];

        const Vector3& face_normal = model.getNormals()[face.normalIndex[0] - 1];

        float brightness = dot_product(normalized_light_direction, face_normal);
        brightness = std::max(0.0f, brightness);  

        Color color = face.face_material.diffuseColor;
        color.r *= brightness;
        color.g *= brightness;
        color.b *= brightness;

        SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

        int min_x = std::max(0, std::min(static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x))), SCREEN_WIDTH - 1));
        int min_y = std::max(0, std::min(static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y))), SCREEN_HEIGHT - 1));
        int max_x = std::min(SCREEN_WIDTH - 1, std::max(static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x))), 0));
        int max_y = std::min(SCREEN_HEIGHT - 1, std::max(static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y))), 0));

        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                if (is_point_inside_triangle(x, y, v0, v1, v2)) {
                    float z = barycentric_interpolation(x, y, v0, v1, v2);
                    if (z > zBuffer[x][y]) {
                        zBuffer[x][y] = z;
                        SDL_RenderDrawPointF(renderer, x, y);
                    }
                }
            }
        }
    }
}



float barycentric_interpolation(int x, int y, const Vector3& v0, const Vector3& v1, const Vector3& v2) {
    float alpha = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float beta = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / ((v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y)); 
    float gamma = 1.0f - alpha - beta;
    return alpha * v0.z + beta * v1.z + gamma * v2.z;
}