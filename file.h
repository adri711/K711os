#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class file {
private:
    std::string filename;
    std::string filelocation;
    std::string filetype;
public:
    std::string content;

    bool selected;

    SDL_Rect file_icon;
    const char* icon_location;
    
    file(std::string,std::string, std::string, const char*);
    
    ~file();
    
    void renamefile(std::string);
    
    std::string getname();
    
    std::string getlocation();
    
    std::string gettype();
    
    std::string getcontent();

    void setcontent(std::string); // for text files only as of now...
    
    bool isSelected();
};

#endif