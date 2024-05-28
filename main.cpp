#include "Screen.h"
#include "Model.h"

int main(){

    Screen screen;

    Model model;

    // Draw the model onto the screen
    screen.drawModel(model);
    
    while(true){
        screen.clear_display();
        model.rotate(0.002,0.001,0.003);
        screen.drawModel(model);
        screen.display();
        screen.input();
        SDL_Delay(30);
    }

    return 0;
}