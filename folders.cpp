#include "folders.h"

extern std::vector<folder> program_folders;
std::vector<guicon>gui_icons;

folder::folder(std::string name, std::string location, const char* iconlocation) {
    this->foldername = name;
    this->folderlocation = location;
    this->icon_location = iconlocation;
    std::cout << "FOLDER: "  << this->foldername << " has been successfuly created." << std::endl;
    program_folders.push_back(*this);
    guicon *foldericon = new guicon(0,0,50,50, "assets/icons/folder-icon.png", name, "NONE");
    this->selected = false;
}

folder::~folder() {
    std::cout << "Folder has been destroyed" << std::endl;
}

void folder::renamefolder(std::string newname) {
    this->foldername = newname;
    std::cout << "Folder has been renamed." << std::endl;
}

std::string folder::getname() {
    return this->foldername;
}

std::string folder::getlocation() {
    return this->folderlocation;
}

bool folder::isSelected() {
    return this->selected;
}