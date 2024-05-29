#include "Screen.h"
#include "Model.h"

int main(){

    Screen screen;
    Model model;
    
    while(true){
        screen.clear_display();
        model.rotate(0.002,0.001,0.003);
        screen.draw_wireframe(model);
        screen.display();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}