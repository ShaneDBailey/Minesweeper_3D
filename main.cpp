#include "Screen.h"
#include "Model.h"

int main(){

    Screen screen;
    std::string object_path = "./assets/test.obj";
    std::string material_path = "./assets/test.mtl";
    Model model(object_path,material_path);
    std::cout << "Object debug" << model.getVertices().size() << std::endl;
    while(true){
        screen.clear_display();
        model.rotate(0.02,0.01,0.03);
        screen.draw_wireframe(model);
        screen.display();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}