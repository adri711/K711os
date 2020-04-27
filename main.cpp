/*
    K_Operating_system_711 - Master
    Author: adri711
    Language: C++
    Written in: 25/04/2020
*/

#include <iostream>
#include <SDL2/SDL.h>
#include "program.h"
//#include "folders.h"
//#include "files.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[]) {

    program *K711_OS = new program("K711_OS");
    while(K711_OS->isRunning()) {
        K711_OS->handle_events();
        K711_OS->draw();
    }    
    K711_OS->shutdown();
    return 0;
}