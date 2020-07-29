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
#include <dirent.h>
#include "program.h"
#include "file.h"

int main(int argc, char* argv[]) {

    program *K711_OS = (program*) malloc(sizeof(program));
    new (K711_OS) program("K711 operating system");
    file *DOCUMENT = new file("Documents", "folder","%root%", "assets/icons/folder-3.png");
    file *THIS_PC = new file("This PC", "folder","%root%", "assets/icons/pc-icon-3.png");
    file *recycle_bin = new file("Recyle bin", "folder","%root%", "assets/icons/recycle-bin.png");
    file *story711 = new file("Story711", "text","%root%", "assets/icons/bn2.png");
    story711->setcontent("Once upon a time there was a charming prince named adri711.\nThis man was the best to ever step on earth!");
    while(K711_OS->isRunning()) {
        K711_OS->handle_events();
        K711_OS->draw();
    }
    K711_OS->shutdown();
    free(K711_OS);
    return 0;
}