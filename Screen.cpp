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



void Screen::draw_face_flat_shading(const Model& model) {
    Vector3 light_direction{-1, 0, 1}; //must be between -1 and 1
    float length = std::sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
    light_direction = normalize(light_direction);

    for (const auto& face : model.getFaces()) {
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0] - 1];
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1] - 1];
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2] - 1];

        const Vector3& face_normal = model.getNormals()[face.normalIndex[0] - 1];

        float brightness = dot_product(light_direction, face_normal);
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

// Ka + Kd * (N dot L) + Ks * (N dot ( L + V / 2))^n
//Ka ambient light color values
//Kd diffuse light color values
//Ks specular light color value
//N face normal
//L vector direction from point to light source
//V vector direction from point to view source
//n speculareExponent
void Screen::draw_face_phong_shading(const Model& model) {
    Vector3 light_direction{-1, 0, 1}; // Light direction vector
    float length = std::sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
    light_direction = {light_direction.x / length, light_direction.y / length, light_direction.z / length};

    Vector3 view_direction{0, 0, 1}; // Assuming the viewer is looking along the z-axis
    view_direction = normalize(view_direction);
    for (const auto& face : model.getFaces()) {
        const Vector3& v0 = model.getVertices()[face.vertexIndex[0] - 1];
        const Vector3& v1 = model.getVertices()[face.vertexIndex[1] - 1];
        const Vector3& v2 = model.getVertices()[face.vertexIndex[2] - 1];

        // Assuming the same normal for the entire face (flat shading)
        const Vector3& face_normal = model.getNormals()[face.normalIndex[0] - 1];

        Color ambient_color = face.face_material.ambientColor;
        Color diffuse_color = face.face_material.diffuseColor;//kd
        Color specular_color = face.face_material.specularColor;//ks
        float shininess = face.face_material.specularExponent;//n

        int min_x = std::max(0, std::min(static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x))), SCREEN_WIDTH - 1));
        int min_y = std::max(0, std::min(static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y))), SCREEN_HEIGHT - 1));
        int max_x = std::min(SCREEN_WIDTH - 1, std::max(static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x))), 0));
        int max_y = std::min(SCREEN_HEIGHT - 1, std::max(static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y))), 0));
        // Ka + Kd * (N dot L) + Ks * (N dot ( L + V / 2))^n
        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                if (is_point_inside_triangle(x, y, v0, v1, v2)) {
                    float z = barycentric_interpolation(x, y, v0, v1, v2);
                    if (z > zBuffer[x][y]) {
                        zBuffer[x][y] = z;
                        Vector3 point = {x,y,z};
                        Vector3 L = light_direction - point;
                        Vector3 V = view_direction - point;
                        Color Final{
                            diffuse_color.r * dot_product(face_normal,L) + specular_color.r * std::pow(dot_product(N,(L + V/2)),shininess);
                            diffuse_color.g * dot_product(face_normal,L) + specular_color.g * std::pow(dot_product(N,(L + V/2)),shininess);
                            diffuse_color.b * dot_product(face_normal,L) + specular_color.b * std::pow(dot_product(N,(L + V/2)),shininess);
                        };
                        SDL_RenderDrawPointF(renderer, x, y);
                    }
                }
            }
        }
    }
}

