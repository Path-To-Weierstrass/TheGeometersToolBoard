#include <SDL.h>
#include <vector>

#include "constants.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "ui.hpp"

int main(int /*argc*/, char* /*argv*/[])
{
    // ---------------- SDL init ----------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 - Circles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // ------------- UI buttons -------------
    const int btnW = 130, btnH = 40, pad = 10;
    std::vector<Button> buttons = {
        { SDL_Rect{ WINDOW_WIDTH - btnW - pad, pad, btnW, btnH}, "Place", ButtonId::PLACE },
        { SDL_Rect{ WINDOW_WIDTH - 2*btnW - 2*pad, pad, btnW, btnH}, "Clear", ButtonId::CLEAR },
        { SDL_Rect{ WINDOW_WIDTH - 3*btnW - 3*pad, pad, btnW, btnH}, "Draw Line", ButtonId::LINE }
    };

    // ------------- state -------------
    bool  running        = true;
    bool  multiPlaceMode = false;
    int   currentRadius  = DEFAULT_RADIUS;
    std::vector<Circle> circles;
    std::vector<Line>   lines;
    SDL_Event e;

    // ------------- main loop -------------
    while (running) {
        // -------- event handling --------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { running = false; }
            else if (e.type == SDL_MOUSEWHEEL) {
                adjustRadius(currentRadius, e.wheel.y > 0 ? 1 : -1);
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_PLUS: case SDLK_EQUALS: case SDLK_KP_PLUS: case SDLK_UP:
                        adjustRadius(currentRadius, 1); break;
                    case SDLK_MINUS: case SDLK_KP_MINUS: case SDLK_DOWN:
                        adjustRadius(currentRadius, -1); break;
                    case SDLK_ESCAPE:
                        multiPlaceMode = false; break;
                    default: break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                bool handled = false;

                // handle buttons
                for (auto& b : buttons) {
                    if (pointInRect(mx, my, b.rect)) {
                        handled = true;
                        switch (b.id) {
                            case ButtonId::PLACE:
                                multiPlaceMode = !multiPlaceMode;
                                break;
                            case ButtonId::CLEAR:
                                circles.clear();
                                lines.clear();
                                break;
                            case ButtonId::LINE: {
                                // draw line if two circles selected
                                std::vector<const Circle*> sel;
                                for (const auto& c : circles) if (c.selected) sel.push_back(&c);
                                if (sel.size() == 2) {
                                    auto [p1, p2] = lineThroughWindow(sel[0]->center, sel[1]->center);
                                    lines.push_back({ p1, p2 });
                                    for (auto& c : circles) c.selected = false;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                if (handled) continue;

                // canvas click
                if (multiPlaceMode) {
                    circles.push_back({ {mx, my}, currentRadius, false });
                } else {
                    for (auto& c : circles) {
                        if (pointInCircle(mx, my, c)) {
                            c.selected = !c.selected;
                            break;
                        }
                    }
                }
            }
        }

        int selectedCount = static_cast<int>(std::count_if(circles.begin(), circles.end(), [](const Circle& c){return c.selected;}));

        // -------- rendering --------
        SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
        SDL_RenderClear(renderer);

        for (const auto& b : buttons) drawButton(renderer, b, (b.id==ButtonId::PLACE && multiPlaceMode));
        drawCircles(renderer, circles);
        drawLines(renderer, lines);
        drawHUD(renderer, currentRadius, multiPlaceMode, selectedCount, static_cast<int>(lines.size()));

        SDL_RenderPresent(renderer);
    }

    // -------- cleanup --------
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}