#include <SDL.h>
#include <iostream>

void drawCircle(SDL_Renderer* renderer, int x0, int y0, int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);

        y += 1;
        if (err <= 0) {
            err += 2 * y + 1;
        } else {
            x -= 1;
            err += 2 * (y - x + 1);
        }
    }
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "My SDL2 Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
     SDL_Rect buttonRect = {
        800 - 110, // x position (right side - width - margin)
        10,        // y position (top margin)
        100,       // width
        40         // height
    };
    bool placeNextClick = false;
    std::vector<SDL_Point> circles; // store circle centers


    while (running) {
       while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
        running = false;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;

        if (event.button.button == SDL_BUTTON_LEFT) {
            // First: if button was clicked, activate circle mode
            if (mx >= buttonRect.x && mx <= buttonRect.x + buttonRect.w &&
                my >= buttonRect.y && my <= buttonRect.y + buttonRect.h) {
                std::cout << "Button clicked - place a circle on next click\n";
                placeNextClick = true;
            }
            // Otherwise, if in placement mode and click is outside button
            else if (placeNextClick) {
                circles.push_back({ mx, my });
                std::cout << "Placed circle at: (" << mx << ", " << my << ")\n";
                placeNextClick = false;
            }
        }
    }
}


        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // dark gray background
        SDL_RenderClear(renderer);

        // Set draw color (e.g., red)
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

// Draw a line from center to right edge
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    SDL_RenderDrawLine(renderer, width / 2, height / 2, width, height / 2);
    // Draw the button background (blue)
SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255); // Windows-style blue
SDL_RenderFillRect(renderer, &buttonRect);
// Draw previously placed circles
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
for (const auto& point : circles) {
    drawCircle(renderer, point.x, point.y, 10);
}

// Optional: draw button border
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
SDL_RenderDrawRect(renderer, &buttonRect);


        // You can draw things here later

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
