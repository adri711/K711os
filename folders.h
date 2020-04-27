#ifndef FOLDERS_H
#define FOLDERS_H
#include <iostream>
#include <string>
#include <vector>

struct folders {
    std::string name;
    std::string location;
}

class folder {
private:
    std::string foldername;
    std::string folderlocation;
public:
    folder(std::string, std::string);
    ~folder();
    void renamefolder(std::string);
};

std::vector<folders> program_folders;

#endif