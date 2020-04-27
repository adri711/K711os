#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class file {
private:
    std::string filename;
    std::string filelocation;
    std::string iconlocation;
public:
    file(std::string, std::string, std::string);
    ~file();
    void renamefile(std::string);
};

#endif