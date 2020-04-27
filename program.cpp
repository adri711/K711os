/*
    Program.cpp - Master
    Author: adri711
    Language: C++
    Written on: 26/04/2020
*/

#include "program.h"
#define LIGHT_BLUE 62, 92, 172, 200
#define LIGHT_RED 150, 12, 12, 100


program::program(std::string programname) {
    this->program_name = programname;
    std::cout << this->program_name << " " << "operating system is being loaded..." << std::endl;
    SDL_DisplayMode display;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GetDesktopDisplayMode(0, &display);
    const char* window_name = this->program_name.c_str();
    this->program_window = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        display.w,
        display.h,
        SDL_WINDOW_FULLSCREEN
    ); std::cout << "Screen width: " << display.w << std::endl;
    std::cout << "Screen height: " << display.h << std::endl;

    if (this->program_window == NULL) {
        std::cout << "Could not create window due to: " << SDL_GetError() << std::endl;
        running = false;
        SDL_Quit();
    }
    this->program_renderer = SDL_CreateRenderer(this->program_window, -1, 0);
    if(this->program_renderer == NULL) {
        std::cout << "Could not create the renderer due to: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    SDL_ShowCursor(SDL_DISABLE);
    running = true;
}

void program::draw() {
    int x,y;
    SDL_SetRenderDrawColor(this->program_renderer, LIGHT_BLUE);
    SDL_RenderClear(this->program_renderer);
    /* CURSOR GRAPHICS GOES HERE */
    SDL_GetMouseState(&x,&y);
    SDL_SetRenderDrawColor(this->program_renderer, LIGHT_RED);
    SDL_Rect r;
    r.w = 10, r.h = 10,r.x = x, r.y =y;
    SDL_RenderFillRect(this->program_renderer, &r);
    /* SELECTION BOX GOES HERE */
    if(this->cursor_hold) {
        float center_point[2];
        float half_diagonal[2];
        center_point[0] = (this->click_position[0] + x) /2;
        center_point[1] = (this->click_position[1] + y) /2;
        half_diagonal[0] = (this->click_position[0] - x) /2;
        half_diagonal[1] = (this->click_position[1] - y) /2;
        int x3 = center_point[0] - half_diagonal[1];
        int y3 = center_point[1] + half_diagonal[0];
        int x4 = center_point[0] + half_diagonal[1];
        int y4 = center_point[1] - half_diagonal[0];
        /* I WILL WRITE THE SOLUTION DOWN HERE ONCE I WAKE UP*/
        rectangle select_rect;
        select_rect.d[0].coordinates[0] = this->click_position[0];
        select_rect.d[0].coordinates[1] = this->click_position[1];
        select_rect.d[1].coordinates[0] = x;
        select_rect.d[1].coordinates[1] = y;
        select_rect.d[2].coordinates[0] = x3;
        select_rect.d[2].coordinates[1] = y3;
        select_rect.d[3].coordinates[0] = x4;
        select_rect.d[3].coordinates[1] = y4;
        std::cout << "X3 " << x3 << std::endl;
        std::cout << "Y3 " << y3 << std::endl;
        std::cout << "X4 " << x4 << std::endl;
        std::cout << "Y4 " << y4 << std::endl;
        int boxwidth, boxheight;

        if (select_rect.d[0].coordinates[0] == select_rect.d[1].coordinates[0]) {
                boxheight = abs(select_rect.d[0].coordinates[1] - select_rect.d[1].coordinates[1]);
            }
        else if(select_rect.d[0].coordinates[1] == select_rect.d[3].coordinates[1]) {
            boxwidth = abs(select_rect.d[0].coordinates[0] - select_rect.d[3].coordinates[0]);
        }
        SDL_Rect select_box;
        select_box.x = this->click_position[0];
        select_box.y = this->click_position[1];
        select_box.h = boxheight;
        select_box.w = boxwidth;
        std::cout << "X: " << this->click_position[0] << " Y: " << this->click_position[1] << std::endl;
        std::cout << "Height: " << boxheight << " Width: " << boxwidth << std::endl;
        SDL_RenderFillRect(this->program_renderer, &select_box);
    }

    SDL_RenderPresent(this->program_renderer); //updates the program
}

void program::shutdown() {
    std::cout << "Shutting down..." << std::endl;
    SDL_DestroyWindow(this->program_window);
    SDL_Quit();
}

bool program::isRunning() {
    return this->running;
}

void program::GetCursorPosition(int* x, int* y){
    *x = this->cursor_position[0];
    *y = this->cursor_position[1];
}

void program::handle_events() {
    SDL_Event e;
    SDL_PumpEvents();
    SDL_PollEvent(&e);
    switch(e.type) {
        case  SDL_QUIT:
            SDL_Quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(!this->cursor_hold) {
                SDL_GetMouseState(&this->click_position[0], &this->click_position[1]);
                this->cursor_hold = true;
                std::cout << "Cursor is being held..." << std::endl;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(this->cursor_hold)
            {
                this->cursor_hold = false;
                std::cout << "Cursor is no longer being held..." << std::endl;
            }
            break;
    }
}

bool program::isCursorHeld(){
    return this->cursor_hold;
}