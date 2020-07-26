/*  
    GUI elements - Master
    Author: adri711
    Language: C++
    Written on: 29/04/2020
 */
#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class guielement {
protected:
    int coordinates[2];
    int width, height;
    SDL_Rect rectangle;
    std::string type;
    std::string target;
public:
    guielement(int, int, int, int, std::string);
    ~guielement();
};

class guicon : guielement {
private:
    SDL_Texture* icon_texture;
    const char* iconlocation;
    std::string text;
public:
    guicon(int,int,int,int, const char*, std::string,std::string);
    ~guicon();
    void moveIcon(int, int);
};

#endif