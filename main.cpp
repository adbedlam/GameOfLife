#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <vector>
#include <chrono>
#include <string>
using std::vector;
using std::string;


void parseArguments(int argc, char* argv[], int& cols, int& rows, int& interval) {
    cols = 80; rows = 60; interval = 100;
    for(int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if(arg == "-w" && i+1 < argc) {
            cols = std::stoi(argv[++i]);
        }
        else if(arg == "-h" && i+1 < argc) {
            rows = std::stoi(argv[++i]);
        }
        else if(arg == "-t" && i+1 < argc) {
            interval = std::stoi(argv[++i]);
        }
    }
}

int main(int argc, char* argv[]) {
    int cols, rows, interval_ms;
    parseArguments(argc, argv, cols, rows, interval_ms);

    const int cellSize = 10;
    const int window_width = cols * cellSize;
    const int window_height = rows * cellSize;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    vector<vector<bool>> grid(rows, vector<bool>(cols, false));
    bool running = true;
    bool paused = true;

    auto last_update = std::chrono::steady_clock::now();

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    paused = !paused;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {

                int x = e.button.x / cellSize;
                int y = e.button.y / cellSize;

                if (x >= 0 && x < cols && y >= 0 && y < rows) {
                    grid[y][x] = !grid[y][x];
                }
            }
        }

        auto now = std::chrono::steady_clock::now();

        if (!paused && std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update).count() >= interval_ms) {

            last_update = now;
            vector<vector<bool>> newGrid = grid;

            for (int y = 0; y < rows; ++y) {
                for (int x = 0; x < cols; ++x) {
                    int count = 0;
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dx==0 && dy==0) {
                                continue;
                            }
                            int nx = x + dx, ny = y + dy;
                            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows && grid[ny][nx]) {
                                count++;
                            }
                        }
                    }
                    if (grid[y][x]) {
                        newGrid[y][x] = (count == 2 || count == 3);
                    }
                    else {
                        newGrid[y][x] = (count == 3);
                    }
                }
            }
            grid.swap(newGrid);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 74, 76, 202, 255);
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (grid[y][x]) {
                    SDL_Rect cell{ x*cellSize, y*cellSize, cellSize, cellSize };
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
        for (int x = 0; x <= cols; ++x) {
            SDL_RenderDrawLine(renderer, x*cellSize, 0, x*cellSize, window_height);
        }
        for (int y = 0; y <= rows; ++y) {
            SDL_RenderDrawLine(renderer, 0, y*cellSize, window_width, y*cellSize);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}