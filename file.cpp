#include "file.h"

extern std::vector<file> program_files;

file::file(std::string name, std::string type, std::string location, const char* iconlocation) {
    this->filename = name;
    this->filelocation = location;
    this->icon_location = iconlocation;
    this->filetype = type;
    this->setcontent("I am god.");
    std::cout << "FILE: "  << this->filename << " has been successfuly created." << std::endl;
    program_files.push_back(*this);
    this->selected = false;
}

file::~file() {
    std::cout << "File has been destroyed" << std::endl;
}

void file::renamefile(std::string newname) {
    this->filename = newname;
    std::cout << "File has been renamed." << std::endl;
}

std::string file::getname() {
    return this->filename;
}

std::string file::getlocation() {
    return this->filelocation;
}

std::string file::gettype() {
    return this->filetype;
}

std::string file::getcontent() {
    return this->content;
}

void file::setcontent(std::string newcontent) {
    this->content = newcontent;
    for(std::vector<file>::iterator i = program_files.begin(); i != program_files.end(); i++)
    {
        if (i->filename == this->filename && i->gettype() == this->gettype() && i->getlocation() == this->getlocation())
        {
            i->content = newcontent;
            break;
        }
    }
}

bool file::isSelected() {
    return this->selected;
}