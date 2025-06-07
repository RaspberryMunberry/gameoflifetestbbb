#include "../hdr/main.hpp"

void checkArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        int value = 0;
        bool valid = false;

        try {
            value = stoi(arg.substr(4));
            valid = true;
        }
        catch (std::invalid_argument&) { printf("Недопустимый формат параметра %s\n", arg.c_str()); }
        catch (std::out_of_range&) { printf("Выход за допустимые границы int  %s\n", arg.c_str()); }
        if (not valid) continue;

        if ((arg.rfind("--w=", 0) == 0) and (value >= config::MIN_GRIDLINE_SIZE and value <= config::MAX_GRIDLINE_SIZE))
            Grid::setWidth(value);
        else if ((arg.rfind("--h=", 0) == 0) and (value >= config::MIN_GRIDLINE_SIZE and value <= config::MAX_GRIDLINE_SIZE))
            Grid::setHeight(value);
        else if ((arg.rfind("--s=", 0) == 0) and (value >= config::MIN_SPEED and value <= config::MAX_SPEED))
            Grid::setSpeed(value);
        else printf("Недопустимый параметр %s - игнорируется\n", arg.c_str());
    }
}

void showParams() {
    printf("Запущено с параметрами:\ngrid_width: %d\ngrid_height: %d\ngame_speed: %d",
        Grid::getWidth(),
        Grid::getHeight(),
        Grid::getSpeed());
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    checkArgs(argc, argv);
    Grid::init();
    showParams();

    bool running = true;
    bool dragging = false;
    bool paused = true;

    int x_offset = 0, y_offset = 0;
    int x_drag_start = 0, y_drag_start = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Game of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    Uint32 last_step_time = SDL_GetTicks();

    while (running) {
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        dragging = true;
                        x_drag_start = event.button.x - x_offset;
                        y_drag_start = event.button.y - y_offset;
                    }
                    else if (event.button.button == SDL_BUTTON_LEFT and paused) {
                        int mouse_x = event.button.x;
                        int mouse_y = event.button.y;

                        // Вычисляем координаты клетки в сетке
                        int grid_x = (mouse_x - x_offset) / (config::CELL_SIZE + config::CELL_GAP);
                        int grid_y = (mouse_y - y_offset) / (config::CELL_SIZE + config::CELL_GAP);

                        // Проверка на выход за границы
                        if (grid_x >= 0 and grid_x < Grid::getWidth() and
                            grid_y >= 0 and grid_y < Grid::getHeight()) {

                            int index = (grid_y + 1) * (Grid::getWidth() + 2) + (grid_x + 1);
                            uint32_t current = Grid::getCurrentGrid()[index];

                            Grid::switchCell(index);
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        dragging = false;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (dragging) {
                        x_offset = event.motion.x - x_drag_start;
                        y_offset = event.motion.y - y_drag_start;
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        paused = not paused;
                    }
                    break;

                default: break;
            }
        }

        Uint32 now_time = SDL_GetTicks();
        if (not paused and (now_time - last_step_time >= Grid::step_interval_ms)) {
            Grid::step();
            last_step_time += Grid::step_interval_ms;
        }

        const std::vector<uint32_t>& grid = Grid::getCurrentGrid();
        int width = Grid::getWidth();
        int height = Grid::getHeight();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint32_t color = grid[(y + 1) * (width + 2) + (x + 1)];

                SDL_SetRenderDrawColor(renderer,
                    (color >> 16) & 0xFF,
                    (color >> 8) & 0xFF,
                    color & 0xFF,
                    255
                );

                SDL_Rect cell_rect;
                cell_rect.x = x_offset + x * (config::CELL_SIZE + config::CELL_GAP);
                cell_rect.y = y_offset + y * (config::CELL_SIZE + config::CELL_GAP);
                cell_rect.w = config::CELL_SIZE;
                cell_rect.h = config::CELL_SIZE;

                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}