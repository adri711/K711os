#include "ui_elements.h"

std::vector<guicon> gui_icons;

guielement::guielement(int newX, int newY, int newW, int newH, std::string newtarget) {
    this->coordinates[0] = newX;
    this->coordinates[1] = newY;
    this->width = newW;
    this->height = newH;
    this->target = newtarget;
    rectangle.x = this->coordinates[0], rectangle.y = this->coordinates[1], rectangle.w = this->width, rectangle.h = this->height; 
}

guielement::~guielement()
{}

guicon::guicon(int newX, int newY,int newW, int newH, const char* newicon, std::string icontext, std::string newTarget) : guielement(newX, newY, newW, newH, newTarget) {
    this->type = "guicon";
    this->iconlocation = newicon;
    this->text = icontext;
    gui_icons.push_back(*this);
}

guicon::~guicon() 
{}

void guicon::moveIcon(int newX, int newY) {
    this->coordinates[0] = newX;
    this->coordinates[1] = newY;
}