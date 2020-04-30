#ifndef FOLDERS_H
#define FOLDERS_H
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "ui_elements.h"

class folder {
protected:
    std::string foldername;
    std::string folderlocation;
public:
    bool selected;

    SDL_Rect folder_icon;
    const char* icon_location;
    
    folder(std::string, std::string, const char*);
    ~folder();
    void renamefolder(std::string);
    std::string getname();
    std::string getlocation();
    bool isSelected();
};

#endif