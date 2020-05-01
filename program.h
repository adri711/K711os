#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <vector>
#include <string>
#include <chrono>
#include "folders.h"

struct point {
    int coordinates[2];
}; struct rectangle {
    point d[4];
};

struct taskwindow {
    SDL_Rect window_rectangle;
    SDL_Rect upperBar;
    SDL_Rect lowerBar;
    SDL_Rect sideBar;
    SDL_Rect closeButton;
};

struct tasks {
    std::string task_name;
    std::string task_type;
    std::string task_origin;
    int task_priority;
    taskwindow task_window;
};

class program {
private:
    std::string program_name;
    std::vector<tasks> tasks_running;
    SDL_Window *program_window;
    SDL_Renderer *program_renderer;
    SDL_DisplayMode program_display;
    bool running;
    int cursor_position[2];
    bool cursor_hold;
    SDL_Rect cursor_rectangle;
    int click_position[2];
    SDL_Rect task_bar_gfx;
    bool task_bar_active;
public:
    program(std::string);
    ~program();
    void shutdown();
    bool isRunning();
    void draw();
    void GetCursorPosition(int*, int*);
    void handle_events();
    bool isCursorHeld();
    bool isTaskBarEnabled();
};


#endif