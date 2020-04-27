#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>
#include <string>

struct point {
    int coordinates[2];
};

struct rectangle {
    point d[4];
};

struct tasks {
    std::string task_name;
    std::vector<char> task_origin;
    int task_priority;
};

class program {
private:
    std::string program_name;
    std::vector<tasks> tasks_running;
    SDL_Window *program_window;
    SDL_Renderer *program_renderer;
    bool running;
    int cursor_position[2];
    bool cursor_hold;
    int click_position[2];
public:
    program(std::string);
    ~program();
    void shutdown();
    bool isRunning();
    void draw();
    void GetCursorPosition(int*, int*);
    void handle_events();
    bool isCursorHeld();
};


#endif