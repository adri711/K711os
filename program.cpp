/*
    Program.cpp - Master
    Author: adri711
    Language: C++
    Written on: 26/04/2020
*/

#include "program.h"

/* Just a couple of constants... */
#define TASKBAR_HEIGHT 40

std::vector<file> program_files;

/* colors in rgb format */
#define LIGHT_BLUE 62, 92, 172, 255
#define LIGHT_RED 200, 12, 12, 255
#define TRANSPARENT_LIGHT_RED 200, 12, 12, 125
#define TRANSPARENT_GREEN 52, 160, 80, 125
#define DARK_GREY 30,30,30,255
#define GREY 120,120,120,255
#define GREEN 32, 200, 62, 255
#define WHITE 255,255,255,255
#define BLACK 0,0,0,255
#define TRANSPARENT_GREY 52,52,90,80
#define TRANSPARENT_LIGHT_BLUE 62, 92, 172, 80
#define ORANGE 255,147,0,150

void sort_tasks(std::vector<tasks> &tasks_list)
{
    if(!tasks_list.empty()){
        bool result = false;
        do
        {
            result = false;
            for(std::vector<tasks>::iterator i = tasks_list.begin(); i < tasks_list.end(); i++)
            {
                auto j = i + 1;
                if(i->task_priority < j->task_priority) 
                {
                    auto temp = *i;
                    *i = *j;
                    *j = temp;
                    result=true;
                    break;
                }
            }
        } while(result);
    }
}

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
    this->draggedwindow = nullptr;
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
        
        /*std::cout << select_rect.d[0].coordinates[0] << std::endl;
        std::cout << select_rect.d[0].coordinates[1] << std::endl;
        std::cout << select_rect.d[1].coordinates[0] << std::endl;
        std::cout << select_rect.d[1].coordinates[1] << std::endl;*/

        int boxwidth, boxheight;
        boxheight = (select_rect.d[0].coordinates[1] - select_rect.d[1].coordinates[1])*-1;
        boxwidth = (select_rect.d[0].coordinates[0] - select_rect.d[1].coordinates[0])*-1;
        SDL_Rect select_box;
        select_box.x = this->click_position[0];
        select_box.y = this->click_position[1];
        select_box.h = boxheight;
        select_box.w = boxwidth;
        /*
        std::cout << "X: " << this->click_position[0] << " Y: " << this->click_position[1] << std::endl;
        std::cout << "Height: " << boxheight << " Width: " << boxwidth << std::endl;*/
     
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
        SDL_SetRenderDrawColor(program_renderer, BLACK);
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
    if(program_files.size() != 0 ) {
        for(std::vector<file>::iterator i = program_files.begin(); i != program_files.end(); i++) 
        {
            if(i->getlocation() == "%root%") {
                int i_index = std::distance(program_files.begin(), i);
                i->file_icon.w = 50, i->file_icon.h = 50;
                if(this->program_display.h / (rowscount + 1) < i_index * i->file_icon.h) {
                    rowscount++;
                }
                i->file_icon.x = 50 * rowscount + 5, i->file_icon.y = (50 + 20) * i_index;
                SDL_Texture *folder_icon_texture = IMG_LoadTexture(program_renderer, i->icon_location);
                SDL_RenderCopy(program_renderer, folder_icon_texture, NULL, &i->file_icon);
                if(i->selected) {
                    SDL_SetRenderDrawColor(program_renderer, TRANSPARENT_LIGHT_BLUE);
                    SDL_RenderFillRect(program_renderer, &i->file_icon);
                }
                SDL_DestroyTexture(folder_icon_texture);
                color = {255,255,255};
                myFont = TTF_OpenFont("assets/fonts/arial.ttf",12);
                SDL_Surface *icon_text_surface = TTF_RenderText_Solid(myFont, i->getname().c_str(), color);
                SDL_Texture *icon_text_texture = SDL_CreateTextureFromSurface(this->program_renderer, icon_text_surface);
                SDL_QueryTexture(icon_text_texture, NULL, NULL, &texW, &texH);
                dstrect = { i->file_icon.x, i->file_icon.y + i->file_icon.h -2, texW, texH };
                SDL_RenderCopy(this->program_renderer, icon_text_texture, NULL, &dstrect);
                if(i->file_icon.x <= x && i->file_icon.x + i->file_icon.w >= x)
                {
                    if(i->file_icon.y <= y && i->file_icon.y + i->file_icon.h >= y) {
                        if(!i->selected) {
                            SDL_SetRenderDrawColor(program_renderer, TRANSPARENT_GREY);
                            SDL_RenderFillRect(program_renderer, &i->file_icon);
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
            if(i->task_type == "folder") 
            {
                SDL_SetRenderDrawColor(this->program_renderer, GREY);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.window_rectangle);
                i->task_window.upperBar.x = i->task_window.window_rectangle.x;
                i->task_window.upperBar.y = i->task_window.window_rectangle.y;
                i->task_window.upperBar.w = i->task_window.window_rectangle.w;
                i->task_window.upperBar.h = (int)(i->task_window.window_rectangle.h*1/6);
                i->task_window.lowerBar.x = i->task_window.window_rectangle.x;
                i->task_window.lowerBar.h = (int)(i->task_window.window_rectangle.h * 1/9);
                i->task_window.lowerBar.w = i->task_window.window_rectangle.w;
                i->task_window.lowerBar.y = i->task_window.window_rectangle.y + i->task_window.window_rectangle.h - i->task_window.lowerBar.h;
                i->task_window.sideBar.x = i->task_window.window_rectangle.x;
                i->task_window.sideBar.y = i->task_window.window_rectangle.y + i->task_window.upperBar.h;
                i->task_window.sideBar.w = (int)(i->task_window.window_rectangle.w * 1/6);
                i->task_window.sideBar.h = i->task_window.lowerBar.y - (i->task_window.window_rectangle.y + i->task_window.upperBar.h);
                i->task_window.closeButton.w = 35;
                i->task_window.closeButton.h = 35; 
                i->task_window.closeButton.x = i->task_window.upperBar.x + (int)(i->task_window.upperBar.w) - i->task_window.closeButton.w;
                i->task_window.closeButton.y = i->task_window.upperBar.y + 2;
                SDL_SetRenderDrawColor(this->program_renderer, LIGHT_BLUE);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.upperBar);
                SDL_SetRenderDrawColor(this->program_renderer, GREY);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.lowerBar);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.sideBar);
                if(i->task_window.closeButton.x <= x && i->task_window.closeButton.x+i->task_window.closeButton.w >= x) {
                    if(i->task_window.closeButton.y <= y && i->task_window.closeButton.y+i->task_window.closeButton.h >= y) {
                        SDL_SetRenderDrawColor(this->program_renderer, LIGHT_RED);
                    }
                    else {
                        SDL_SetRenderDrawColor(this->program_renderer, TRANSPARENT_LIGHT_RED);
                    }
                }
                else {
                    SDL_SetRenderDrawColor(this->program_renderer, TRANSPARENT_LIGHT_RED);
                }
                SDL_RenderFillRect(this->program_renderer, &i->task_window.closeButton);
                color = {0,0,0};
                myFont = TTF_OpenFont("assets/fonts/arial.ttf",12);
                std::string text = "FODLER: " + i->task_name;
                SDL_Surface *folder_text_surface = TTF_RenderText_Solid(myFont, text.c_str(), color);
                SDL_Texture *folder_text_texture = SDL_CreateTextureFromSurface(this->program_renderer, folder_text_surface);
                SDL_QueryTexture(folder_text_texture, NULL, NULL, &texW, &texH);
                dstrect = { i->task_window.upperBar.x + 10, i->task_window.upperBar.y + (int)(i->task_window.upperBar.h/2) - 10, texW, texH };
                SDL_RenderCopy(this->program_renderer, folder_text_texture, NULL, &dstrect);
                SDL_DestroyTexture(folder_text_texture);
                SDL_FreeSurface(folder_text_surface);
                TTF_CloseFont(myFont);
            }
            else if(i->task_type == "text")
            {
                SDL_SetRenderDrawColor(this->program_renderer, GREY);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.window_rectangle);
                i->task_window.upperBar.x = i->task_window.window_rectangle.x;
                i->task_window.upperBar.y = i->task_window.window_rectangle.y;
                i->task_window.upperBar.w = i->task_window.window_rectangle.w;
                i->task_window.upperBar.h = (int)(i->task_window.window_rectangle.h*1/12);
                i->task_window.lowerBar.x = i->task_window.window_rectangle.x;
                i->task_window.lowerBar.h = (int)(i->task_window.window_rectangle.h * 1/12);
                i->task_window.lowerBar.w = i->task_window.window_rectangle.w;
                i->task_window.lowerBar.y = i->task_window.window_rectangle.y + i->task_window.window_rectangle.h - i->task_window.lowerBar.h;
                i->task_window.sideBar.x = i->task_window.window_rectangle.x;
                i->task_window.sideBar.y = i->task_window.window_rectangle.y + i->task_window.upperBar.h;
                i->task_window.sideBar.w = (int)(i->task_window.window_rectangle.w * 1/12);
                i->task_window.sideBar.h = i->task_window.lowerBar.y - (i->task_window.window_rectangle.y + i->task_window.upperBar.h);
                i->task_window.closeButton.w = 35;
                i->task_window.closeButton.h = 35; 
                i->task_window.closeButton.x = i->task_window.upperBar.x + (int)(i->task_window.upperBar.w) - i->task_window.closeButton.w;
                i->task_window.closeButton.y = i->task_window.upperBar.y + 2;
                SDL_SetRenderDrawColor(this->program_renderer, GREEN);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.upperBar);
                SDL_SetRenderDrawColor(this->program_renderer, GREY);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.lowerBar);
                SDL_RenderFillRect(this->program_renderer, &i->task_window.sideBar);
                if(i->task_window.closeButton.x <= x && i->task_window.closeButton.x + i->task_window.closeButton.w >= x) {
                    if(i->task_window.closeButton.y <= y && i->task_window.closeButton.y + i->task_window.closeButton.h >= y) 
                    {
                        SDL_SetRenderDrawColor(this->program_renderer, LIGHT_RED);
                    }
                    else {
                        SDL_SetRenderDrawColor(this->program_renderer, TRANSPARENT_LIGHT_RED);
                    }
                }
                else {
                    SDL_SetRenderDrawColor(this->program_renderer, TRANSPARENT_LIGHT_RED);
                }
                SDL_RenderFillRect(this->program_renderer, &i->task_window.closeButton);
                color = {0,0,0};
                myFont = TTF_OpenFont("assets/fonts/arial.ttf",12);
                std::string text = "TEXT FILE: " + i->task_name;
                SDL_Surface *folder_text_surface = TTF_RenderText_Solid(myFont, text.c_str(), color);
                SDL_Texture *folder_text_texture = SDL_CreateTextureFromSurface(this->program_renderer, folder_text_surface);
                SDL_QueryTexture(folder_text_texture, NULL, NULL, &texW, &texH);
                dstrect = { i->task_window.upperBar.x + 10, i->task_window.upperBar.y + (int)(i->task_window.upperBar.h/2) - 10, texW, texH };
                SDL_RenderCopy(this->program_renderer, folder_text_texture, NULL, &dstrect);
                SDL_DestroyTexture(folder_text_texture);
                SDL_FreeSurface(folder_text_surface);
                /* */
                SDL_Surface *file_text_surface = TTF_RenderText_Blended_Wrapped(myFont, i->task_content.c_str(), color, 1000);
                SDL_Texture *file_text_texture = SDL_CreateTextureFromSurface(this->program_renderer, file_text_surface);
                SDL_QueryTexture(file_text_texture, NULL, NULL, &texW, &texH);
                dstrect = {(int)(i->task_window.sideBar.x + i->task_window.sideBar.w + 5), (int)(i->task_window.upperBar.y + i->task_window.upperBar.h), texW, texH};
                SDL_RenderCopy(this->program_renderer, file_text_texture, NULL, &dstrect);
                
                SDL_DestroyTexture(file_text_texture);
                SDL_FreeSurface(file_text_surface);
                /* */
                TTF_CloseFont(myFont);
            }
            else {
                std::cout << "Trying to open a file with unknown format..." << std::endl;
            }
        }
    }
    SDL_Texture *CURSOR_TEXTURE = IMG_LoadTexture(program_renderer, "assets/icons/cursor-icon.png");
    SDL_RenderCopy(program_renderer, CURSOR_TEXTURE, NULL, &CURSOR_RECT);
    SDL_DestroyTexture(CURSOR_TEXTURE);
    SDL_RenderPresent(this->program_renderer);
    /*
        Messing with this shit
    */
   if(draggedwindow != nullptr) {
       SDL_GetMouseState(&cursor_position[0],&cursor_position[1]);
       auto var = draggedwindow->window_rectangle.x - click_position[0];
       auto var2 = draggedwindow->window_rectangle.y - click_position[1];
       draggedwindow->window_rectangle.x=cursor_position[0] + var;
       draggedwindow->window_rectangle.y=cursor_position[1] + var2;
       click_position[0] = cursor_position[0];
       click_position[1] = cursor_position[1];
   }
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
        case SDL_KEYDOWN:
            switch( e.key.keysym.sym ){
                case SDLK_9:
                    this->running = false;
                    break;
            }
            break;
        case  SDL_QUIT:
            SDL_Quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(!this->cursor_hold) {
                this->cursor_hold = true;
                //std::cout << "Cursor is being held..." << std::endl;
                SDL_GetMouseState(&this->click_position[0], &this->click_position[1]);
                int xy = 0;
                int x = this->click_position[0];
                int y = this->click_position[1];
                for(std::vector<tasks>::iterator i = tasks_running.begin(); i != tasks_running.end(); i++) 
                {
                    if(i->task_window.closeButton.x <= x && i->task_window.closeButton.x + i->task_window.closeButton.w >= x && i->task_window.closeButton.y <= y && i->task_window.closeButton.y + i->task_window.closeButton.h >= y)
                    {
                        tasks_running.erase(tasks_running.begin()+xy);
                        break;
                    }
                    else if(i->task_window.upperBar.x <= x && i->task_window.upperBar.x + i->task_window.upperBar.w >= x && i->task_window.upperBar.y <= y && i->task_window.upperBar.y + i->task_window.upperBar.h >= y) {
                        this->draggedwindow = &i->task_window;
                        std::cout << "Dragged window has been assigned." << std::endl;
                    }
                    xy++;
                }
            }
            if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
                for(std::vector<file>::iterator i = program_files.begin(); i != program_files.end(); i++) {
                    if(i->file_icon.x <= this->click_position[0] && i->file_icon.x + i->file_icon.w >= this->click_position[0]){
                        if(i->file_icon.y <= this->click_position[1] && i->file_icon.y + i->file_icon.h >= this->click_position[1]) {
                            if(!i->selected){
                                i->selected = true;
                            } 
                            else {
                                tasks new_task;
                                this->taskidentificator++;
                                new_task.task_name = i->getname();
                                new_task.task_type = i->gettype();
                                new_task.task_content = i->getcontent();
                                new_task.task_origin = i->getlocation() + i->getname();
                                new_task.task_priority = taskidentificator;
                                new_task.task_window.window_rectangle.x = 100;
                                new_task.task_window.window_rectangle.y = 100;
                                new_task.task_window.window_rectangle.h = (int)(this->program_display.h * 3/4);
                                new_task.task_window.window_rectangle.w = (int)(this->program_display.w * 1/2);
                                this->tasks_running.push_back(new_task);
                                i->selected = false;
                                //sort_tasks(this->tasks_running);
                            }
                            break;
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
                //std::cout << "Cursor is no longer being held..." << std::endl;

                if(this->draggedwindow != nullptr) {
                    draggedwindow = nullptr;
                }
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