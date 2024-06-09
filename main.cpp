#include "Screen.h"
#include "Model.h"

int main(){

    Screen screen;
    std::string object_path = "./assets/test.obj";
    std::string material_path = "./assets/test.mtl";
    Model model(object_path,material_path);

    while(true){
        screen.clear_display();
        model.rotate(0.002,0.001,0.003);
        //screen.draw_wireframe(model);
        //screen.draw_face_rasterization(model);
        screen.draw_face_phong_shading(model);
        SDL_RenderPresent(screen.renderer);
        screen.input();
        //SDL_Delay(30);
    }

    return 0;
}