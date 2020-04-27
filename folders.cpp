#include "folders.h"

folder::folder(std::string name, std::string location) {
    this->foldername = name;
    this->folderlocation = location;
    std::cout << this->foldername << " has been created." << std::endl;
    folders folder;
    folder.name = this->foldername;
    folder.location = this->folderlocation;
}