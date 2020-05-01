/*
    K_Operating_system_711 - Master
    Author: adri711
    Language: C++
    Written in: 25/04/2020
*/

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "program.h"
#include "folders.h"
//#include "files.h"

int main(int argc, char* argv[]) {

    program *K711_OS = new program("K711_OS");
    folder *DOCUMENT = new folder("Documents", "%root%", "assets/icons/folder-3.png");
    folder *THIS_PC = new folder("This PC", "%root%", "assets/icons/pc-icon-3.png");
    folder *recycle_bin = new folder("Recyle bin", "%root%", "assets/icons/recycle-bin.png");
    folder *images = new folder("Images", "%root%documents%", "images-folder-icon.png");
    while(K711_OS->isRunning()) {
        K711_OS->handle_events();
        K711_OS->draw();
    }
    K711_OS->shutdown();
    return 0;
}