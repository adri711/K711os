/*
    Files.cpp - Master
    Author: adri711
    Language: C++
    Written on: 26/04/2020
*/

#include "files.h"

file::file(std::string name, std::string location, std::string iconlocation){
    this->filename = name;
    this->filelocation = location;
    this->iconlocation = iconlocation;
    std::cout << "File_name: " << this->filename << " has been created." << std::endl;
    std::cout << "Additional information:" << std::endl;
    std::cout << "File location: " << this->filelocation << std::endl;
    std::cout << "Icon location: " << this->iconlocation << std::endl;
}

file::~file() {
    std::cout << "File name: " << this->filename << " has been destroyed." << std::endl;
}

void file::renamefile(std::string newname) {
    std::cout << this->filename << " is being renamed..." << std::endl;
    std::cout << "Additional information: " << std::endl;
    std::cout << "new_name: " << newname << std::endl;
    this->filename = newname;
}