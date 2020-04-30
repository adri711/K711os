/*
    Program.cpp - Master
    Author: adri711
    Language: C++
    Written on: 26/04/2020
*/

#include "program.h"

/* Just a couple of constants... */
#define TASKBAR_HEIGHT 40

std::vector<folder> program_folders;

/* colors in rgb format */
#define LIGHT_BLUE 62, 92, 172, 255
#define LIGHT_RED 150, 12, 12, 125
#define TRANSPARENT_GREEN 52, 160, 80, 125
#define DARK_GREY 30,30,30,255
#define GREY 15,15,15,255
#define GREEN 32, 200, 62, 255
#define WHITE 255,255,255,255
#define BLACK 0,0,0,255
#define TRANSPARENT_GREY 52,52,90,80
#define TRANSPARENT_LIGHT_BLUE 62, 92, 172, 80

program::program(std::string programname) {
    this->program_name = programname;
    std::cout << this->program_name << " " << "operating system is being loaded..." << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_GetDesktopDisplayMode(0, &this->program_display);
    const char* window_name = this->program_name.c_str();
    this->program_window = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        this->program_display.w,
        this->program_display.h,
        SDL_WINDOW_FULLSCREEN
    ); std::cout << "Screen width: " << this->program_display.w << std::endl;
    std::cout << "Screen height: " << this->program_display.h << std::endl;

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
    SDL_SetRenderDrawBlendMode(this->program_renderer, SDL_BLENDMODE_BLEND);
    SDL_ShowCursor(SDL_DISABLE);
    this->running = true;
    this->task_bar_active = true;
    this->task_bar_gfx.w = this->program_display.w;
    this->task_bar_gfx.h = TASKBAR_HEIGHT;
    this->task_bar_gfx.x = 0;
    this->task_bar_gfx.y = this->program_display.h - this->task_bar_gfx.h;
}

void program::draw() {
    int x,y;
    SDL_SetRenderDrawColor(this->program_renderer, DARK_GREY);
    SDL_RenderClear(this->program_renderer);
    /* CURSOR GRAPHICS GOES HERE */
    SDL_GetMouseState(&x,&y);
    SDL_SetRenderDrawColor(this->program_renderer, TRANSPARENT_GREEN);
    SDL_Rect CURSOR_RECT;
    CURSOR_RECT.w = 24, CURSOR_RECT.h = 24,CURSOR_RECT.x = x, CURSOR_RECT.y =y;
    /* SELECTION BOX GOES HERE */
    if(this->cursor_hold) {
        int x1,y1,x2,y2;
        x1 = this->click_position[0];
        y1 = y;
        x2 = x;
        y2 = this->click_position[1];
        rectangle select_rect;
        select_rect.d[0].coordinates[0] = this->click_position[0];
        select_rect.d[0].coordinates[1] = this->click_position[1];
        select_rect.d[1].coordinates[0] = x;
        select_rect.d[1].coordinates[1] = y;
        select_rect.d[2].coordinates[0] = x1;
        select_rect.d[2].coordinates[1] = y1;
        select_rect.d[3].coordinates[0] = x2;
        select_rect.d[3].coordinates[1] = y2;

        int boxwidth, boxheight;
        boxheight = (select_rect.d[0].coordinates[1] - select_rect.d[1].coordinates[1])*-1;
        boxwidth = (select_rect.d[0].coordinates[0] - select_rect.d[1].coordinates[0])*-1;
        SDL_Rect select_box;
        select_box.x = this->click_position[0];
        select_box.y = this->click_position[1];
        select_box.h = boxheight;
        select_box.w = boxwidth;
        std::cout << "X: " << this->click_position[0] << " Y: " << this->click_position[1] << std::endl;
        std::cout << "Height: " << boxheight << " Width: " << boxwidth << std::endl;
        SDL_RenderFillRect(this->program_renderer, &select_box);
    }
    /* TASK BAR GRAPHICS */
    if(this->task_bar_active) {
        SDL_Texture *taskbartexture;
        SDL_Rect taskbariconrect;
        taskbariconrect.w = TASKBAR_HEIGHT - 2;
        taskbariconrect.h = TASKBAR_HEIGHT - 2;
        taskbariconrect.x = 2;
        taskbariconrect.y = this->program_display.h - TASKBAR_HEIGHT + 2;
        SDL_SetRenderDrawColor(program_renderer, GREY);
        SDL_RenderFillRect(program_renderer, &task_bar_gfx);
        taskbartexture = IMG_LoadTexture(program_renderer, "assets/icons/taskbaricon.png");
        SDL_RenderCopy(program_renderer, taskbartexture, NULL, &taskbariconrect);
        SDL_DestroyTexture(taskbartexture);
    }
    std::time_t date = time(0);
    std::string today_date = ctime(&date);
    TTF_Font* myFont = TTF_OpenFont("assets/fonts/arial.TTF", 12);
    SDL_Color color = { 55, 150, 150 };
    int texW = 0;
    int texH = 0;
    SDL_Surface * text_surface = TTF_RenderText_Solid(myFont, today_date.c_str(), color);
    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(this->program_renderer, text_surface);
    SDL_QueryTexture(text_texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { this->program_display.w - 178, this->program_display.h - TASKBAR_HEIGHT + 10, texW, texH };
    SDL_SetRenderDrawColor(program_renderer, WHITE);
    SDL_RenderFillRect(program_renderer, &dstrect);
    SDL_RenderCopy(this->program_renderer, text_texture, NULL, &dstrect);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    TTF_CloseFont(myFont);
    int rowscount = 0;
    if(program_folders.size() != 0 ) {
        for(std::vector<folder>::iterator i = program_folders.begin(); i != program_folders.end(); i++) 
        {
            if(i->getlocation() == "%root%") {
                int i_index = std::distance(program_folders.begin(), i);
                i->folder_icon.w = 50, i->folder_icon.h = 50;
                if(rowscount != 0) {
                    if(this->program_display.h / (rowscount + 1) < i_index * i->folder_icon.h) {
                        rowscount++;
                    }
                }
                i->folder_icon.x = 50 * rowscount + 5, i->folder_icon.y = (50 + 20) * i_index;
                SDL_Texture *folder_icon_texture = IMG_LoadTexture(program_renderer, i->icon_location);
                SDL_RenderCopy(program_renderer, folder_icon_texture, NULL, &i->folder_icon);
                if(i->selected) {
                    SDL_SetRenderDrawColor(program_renderer, TRANSPARENT_LIGHT_BLUE);
                    SDL_RenderFillRect(program_renderer, &i->folder_icon);
                }
                SDL_DestroyTexture(folder_icon_texture);
                color = {255,255,255};
                myFont = TTF_OpenFont("assets/fonts/arial.ttf",12);
                SDL_Surface *icon_text_surface = TTF_RenderText_Solid(myFont, i->getname().c_str(), color);
                SDL_Texture *icon_text_texture = SDL_CreateTextureFromSurface(this->program_renderer, icon_text_surface);
                SDL_QueryTexture(icon_text_texture, NULL, NULL, &texW, &texH);
                dstrect = { i->folder_icon.x, i->folder_icon.y + i->folder_icon.h -2, texW, texH };
                SDL_RenderCopy(this->program_renderer, icon_text_texture, NULL, &dstrect);
                if(i->folder_icon.x <= x && i->folder_icon.x + i->folder_icon.w >= x)
                {
                    if(i->folder_icon.y <= y && i->folder_icon.y + i->folder_icon.h >= y) {
                        if(!i->selected) {
                            SDL_SetRenderDrawColor(program_renderer, TRANSPARENT_GREY);
                            SDL_RenderFillRect(program_renderer, &i->folder_icon);
                        }
                    }
                }
                SDL_DestroyTexture(icon_text_texture);
                SDL_FreeSurface(icon_text_surface);
                TTF_CloseFont(myFont);
            }
        }
    }
    if(tasks_running.size() != 0) {
        for(std::vector<tasks>::iterator i = tasks_running.begin(); i != tasks_running.end(); i++) {
            SDL_SetRenderDrawColor(this->program_renderer, WHITE);
            SDL_RenderFillRect(this->program_renderer, &i->task_window);
        }
    }
    SDL_Texture *CURSOR_TEXTURE = IMG_LoadTexture(program_renderer, "assets/icons/cursor-icon.png");
    SDL_RenderCopy(program_renderer, CURSOR_TEXTURE, NULL, &CURSOR_RECT);
    SDL_DestroyTexture(CURSOR_TEXTURE);
    SDL_RenderPresent(this->program_renderer);
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
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                for(std::vector<folder>::iterator i = program_folders.begin(); i != program_folders.end(); i++) {
                    if(i->folder_icon.x <= this->click_position[0] && i->folder_icon.x + i->folder_icon.w >= this->click_position[0]){
                        if(i->folder_icon.y <= this->click_position[1] && i->folder_icon.y + i->folder_icon.h >= this->click_position[1]) {
                            if(!i->selected){
                                i->selected = true;
                            } 
                            else {
                                tasks new_task;
                                new_task.task_name = "FOLDER_PROCESS:" + i->getname();
                                new_task.task_type = "FOLDER_PROCESS";
                                new_task.task_origin = i->getlocation();
                                new_task.task_priority = 1;
                                new_task.task_window.x = 100;
                                new_task.task_window.y = 100;
                                new_task.task_window.h = (int)(this->program_display.h * 3/4);
                                new_task.task_window.w = (int)(this->program_display.w * 1/2);
                                this->tasks_running.push_back(new_task);
                                i->selected = false;
                            }
                        }
                        else {
                            i->selected = false;
                        }
                    }
                    else {
                        i->selected = false;
                    }
                }
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

bool program::isTaskBarEnabled() {
    return this->task_bar_active;
}