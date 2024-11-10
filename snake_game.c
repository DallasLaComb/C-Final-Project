#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BLOCK_SIZE 20
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Point;

typedef enum {
    MENU_START,
    MENU_SETTINGS,
    MENU_HOWTOPLAY,
    MENU_EXIT
} MenuOption;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Define a Color structure to hold color information
typedef struct {
    const char *name;
    SDL_Color color;
} ColorOption;

// Predefined colors
ColorOption colors[] = {
    {"Red",    {255, 0,   0,   255}},
    {"Green",  {0,   255, 0,   255}},
    {"Blue",   {0,   0,   255, 255}},
    {"Yellow", {255, 255, 0,   255}},
    {"Purple", {128, 0,   128, 255}},
    {"Orange", {255, 165, 0,   255}},
    {"Cyan",   {0,   255, 255, 255}},
    {"White",  {255, 255, 255, 255}},
    {"Black",  {0,   0,   0,   255}}
};
int numColors = sizeof(colors) / sizeof(colors[0]);

SDL_Color snakeColor = {0, 255, 0, 255}; // Default: Green
SDL_Color foodColor = {255, 0, 0, 255};  // Default: Red

void DrawText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color);
MenuOption ShowMenu(SDL_Renderer *renderer);
void ShowSettings(SDL_Renderer *renderer);
void ShowHowToPlay(SDL_Renderer *renderer);
void StartGame(SDL_Renderer *renderer, SDL_Window *window);
void ChooseColor(SDL_Renderer *renderer, SDL_Color *targetColor, const char *itemName);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Snake Game",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int running = 1;
    while (running) {
        MenuOption choice = ShowMenu(renderer);
        switch (choice) {
            case MENU_START:
                StartGame(renderer, window);
                break;
            case MENU_SETTINGS:
                ShowSettings(renderer);
                break;
            case MENU_HOWTOPLAY:
                ShowHowToPlay(renderer);
                break;
            case MENU_EXIT:
                running = 0;
                break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void DrawText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color) {
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Failed to create surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

MenuOption ShowMenu(SDL_Renderer *renderer) {
    int selected = 0;
    const char *options[] = {"Start Game", "Settings", "How To Play", "Exit"};
    int numOptions = 4;
    SDL_Event e;

    while (1) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return MENU_EXIT;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP) {
                    selected = (selected - 1 + numOptions) % numOptions;
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    selected = (selected + 1) % numOptions;
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    return (MenuOption)selected;
                }
            }
        }
        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < numOptions; i++) {
            SDL_Color color = {255, 255, 255, 255};
            if (i == selected) {
                color.r = 100;
                color.g = 100;
                color.b = 255;
            }
            DrawText(renderer, options[i], 220, 150 + i * 60, color);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

void ShowSettings(SDL_Renderer *renderer) {
    int selected = 0;
    const char *options[] = {"Change Snake Color", "Change Food Color", "Back"};
    int numOptions = 3;
    SDL_Event e;

    while (1) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP) {
                    selected = (selected - 1 + numOptions) % numOptions;
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    selected = (selected + 1) % numOptions;
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    if (selected == 0) {
                        // Choose snake color
                        ChooseColor(renderer, &snakeColor, "Snake");
                    } else if (selected == 1) {
                        // Choose food color
                        ChooseColor(renderer, &foodColor, "Food");
                    } else if (selected == 2) {
                        return;
                    }
                }
            }
        }
        // Rendering
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < numOptions; i++) {
            SDL_Color color = {255, 255, 255, 255};
            if (i == selected) {
                color.r = 100;
                color.g = 255;
                color.b = 100;
            }
            DrawText(renderer, options[i], 220, 150 + i * 60, color);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

void ChooseColor(SDL_Renderer *renderer, SDL_Color *targetColor, const char *itemName) {
    int selected = 0;
    SDL_Event e;

    while (1) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_UP) {
                    selected = (selected - 1 + numColors) % numColors;
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    selected = (selected + 1) % numColors;
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    // Set the selected color
                    *targetColor = colors[selected].color;
                    return;
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return;
                }
            }
        }
        // Rendering
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        char title[50];
        snprintf(title, sizeof(title), "Choose %s Color", itemName);
        DrawText(renderer, title, 200, 100, (SDL_Color){255, 255, 255, 255});
        for (int i = 0; i < numColors; i++) {
            SDL_Color color = colors[i].color;
            if (i == selected) {
                // Highlight selected color
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Rect rect = {180, 150 + i * 40, 280, 35};
                SDL_RenderDrawRect(renderer, &rect);
            }
            DrawText(renderer, colors[i].name, 220, 150 + i * 40, color);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

void ShowHowToPlay(SDL_Renderer *renderer) {
    SDL_Event e;
    while (1) {
        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_RETURN) {
                    return;
                }
            }
        }
        // Rendering
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        SDL_Color color = {255, 255, 255, 255};
        DrawText(renderer, "Use arrow keys to move the snake.", 100, 200, color);
        DrawText(renderer, "Avoid hitting the walls or yourself.", 100, 240, color);
        DrawText(renderer, "Eat food to grow longer.", 100, 280, color);
        DrawText(renderer, "Press ESC or Enter to go back.", 100, 320, color);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
}

void StartGame(SDL_Renderer *renderer, SDL_Window *window) {
    // Set the window to full screen
    if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
        printf("Failed to set full screen: %s\n", SDL_GetError());
    }

    Point snake[MAX_SNAKE_LENGTH];
    int snakeLength = 5;
    Direction dir = DIR_RIGHT;
    int running = 1;
    SDL_Event e;
    int frameDelay = 100;
    int windowWidth = SDL_GetWindowSurface(window)->w;
    int windowHeight = SDL_GetWindowSurface(window)->h;
    int maxX = windowWidth / BLOCK_SIZE;
    int maxY = windowHeight / BLOCK_SIZE;
    Point food = {rand() % maxX, rand() % maxY};

    // Initialize snake position
    for (int i = 0; i < snakeLength; i++) {
        snake[i].x = maxX / 2 - i;
        snake[i].y = maxY / 2;
    }

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        // Event handling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (dir != DIR_DOWN) dir = DIR_UP;
                        break;
                    case SDLK_DOWN:
                        if (dir != DIR_UP) dir = DIR_DOWN;
                        break;
                    case SDLK_LEFT:
                        if (dir != DIR_RIGHT) dir = DIR_LEFT;
                        break;
                    case SDLK_RIGHT:
                        if (dir != DIR_LEFT) dir = DIR_RIGHT;
                        break;
                    case SDLK_ESCAPE:
                        running = 0; // Exit the game
                        break;
                }
            }
        }

        // Move snake
        for (int i = snakeLength - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        if (dir == DIR_UP) snake[0].y -= 1;
        if (dir == DIR_DOWN) snake[0].y += 1;
        if (dir == DIR_LEFT) snake[0].x -= 1;
        if (dir == DIR_RIGHT) snake[0].x += 1;

        // Check collisions with walls
        if (snake[0].x < 0 || snake[0].x >= maxX ||
            snake[0].y < 0 || snake[0].y >= maxY) {
            running = 0; // Game over
        }

        // Check collisions with self
        for (int i = 1; i < snakeLength; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                running = 0; // Game over
            }
        }

        // Check if food is eaten
        if (snake[0].x == food.x && snake[0].y == food.y) {
            if (snakeLength < MAX_SNAKE_LENGTH) snakeLength++;
            food.x = rand() % maxX;
            food.y = rand() % maxY;
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw border
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect borderRect = {0, 0, windowWidth, windowHeight};
        SDL_RenderDrawRect(renderer, &borderRect);

        // Draw food
        SDL_SetRenderDrawColor(renderer, foodColor.r, foodColor.g, foodColor.b, 255);
        SDL_Rect foodRect = {food.x * BLOCK_SIZE, food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &foodRect);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, snakeColor.r, snakeColor.g, snakeColor.b, 255);
        for (int i = 0; i < snakeLength; i++) {
            SDL_Rect snakeRect = {snake[i].x * BLOCK_SIZE, snake[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
            SDL_RenderFillRect(renderer, &snakeRect);
        }

        SDL_RenderPresent(renderer);

        // Frame delay
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Set the window back to windowed mode
    if (SDL_SetWindowFullscreen(window, 0) != 0) {
        printf("Failed to exit full screen: %s\n", SDL_GetError());
    }
}
