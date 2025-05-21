#include <SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>

// -------------------- constants --------------------
constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int DEFAULT_RADIUS = 10;
constexpr int MIN_RADIUS = 3;
constexpr int MAX_RADIUS = 60;

constexpr SDL_Color BG_COLOR          {30,  30,  30,  255};
constexpr SDL_Color BUTTON_COLOR      {0,   120, 215, 255};
constexpr SDL_Color BUTTON_ACTIVE     {0,   180, 120, 255};
constexpr SDL_Color BUTTON_LINE       {120, 60,  200, 255};
constexpr SDL_Color CIRCLE_COLOR      {255, 255, 255, 255};
constexpr SDL_Color SELECTED_COLOR    {255, 255, 0,   255};
constexpr SDL_Color BORDER_COLOR      {255, 255, 255, 255};
constexpr SDL_Color DRAWN_LINE_COLOR  {200, 200, 255, 255};

// -------------------- data types -------------------
struct Circle {
    SDL_Point center;
    int radius;
    bool selected = false;
};

struct Line {
    SDL_Point p1;
    SDL_Point p2;
};

enum class ButtonId { PLACE, CLEAR, LINE };
struct Button {
    SDL_Rect   rect;
    const char* label;
    ButtonId   id;
};

// -------------------- helpers ----------------------
bool adjustRadius(int &radius, int delta) {
    int newVal = std::clamp(radius + delta, MIN_RADIUS, MAX_RADIUS);
    if (newVal != radius) { radius = newVal; return true; }
    return false;
}

bool pointInRect(int x, int y, const SDL_Rect& r) {
    return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

bool pointInCircle(int x, int y, const Circle& c) {
    int dx = x - c.center.x;
    int dy = y - c.center.y;
    return dx*dx + dy*dy <= c.radius * c.radius;
}

// compute two edge‑points so that the segment spans the whole window
std::pair<SDL_Point, SDL_Point> lineThroughWindow(const SDL_Point& p1, const SDL_Point& p2) {
    if (p1.x == p2.x) { // vertical line
        return { SDL_Point{p1.x, 0}, SDL_Point{p1.x, WINDOW_HEIGHT} };
    }
    double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
    double b = p1.y - m * p1.x;
    std::vector<SDL_Point> pts;
    int y_left  = static_cast<int>(std::round(b));
    if (y_left >= 0 && y_left <= WINDOW_HEIGHT) pts.push_back({0, y_left});
    int y_right = static_cast<int>(std::round(m * WINDOW_WIDTH + b));
    if (y_right >= 0 && y_right <= WINDOW_HEIGHT) pts.push_back({WINDOW_WIDTH, y_right});
    int x_top    = static_cast<int>(std::round(-b / m));
    if (x_top >= 0 && x_top <= WINDOW_WIDTH) pts.push_back({x_top, 0});
    int x_bottom = static_cast<int>(std::round((WINDOW_HEIGHT - b) / m));
    if (x_bottom >= 0 && x_bottom <= WINDOW_WIDTH) pts.push_back({x_bottom, WINDOW_HEIGHT});
    if (pts.size() < 2) return {p1, p2};
    return {pts[0], pts[1]};
}

// -------------------- main -------------------------
int main(int /*argc*/, char* /*argv*/[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 - Circles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) { std::cerr << SDL_GetError() << '\n'; SDL_Quit(); return 1; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) { std::cerr << SDL_GetError() << '\n'; SDL_DestroyWindow(window); SDL_Quit(); return 1; }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // ----- UI buttons -----
    const int btnW = 130, btnH = 40, pad = 10;
    std::vector<Button> buttons = {
        { SDL_Rect{ WINDOW_WIDTH - btnW - pad, pad, btnW, btnH}, "Place", ButtonId::PLACE },
        { SDL_Rect{ WINDOW_WIDTH - 2*btnW - 2*pad, pad, btnW, btnH}, "Clear", ButtonId::CLEAR },
        { SDL_Rect{ WINDOW_WIDTH - 3*btnW - 3*pad, pad, btnW, btnH}, "Draw Line", ButtonId::LINE }
    };

    // ----- state -----
    bool running        = true;
    bool multiPlaceMode = false;
    int  currentRadius  = DEFAULT_RADIUS;
    std::vector<Circle> circles;
    std::vector<Line>   lines;          // store drawn lines
    SDL_Event e;

    while (running) {
        // ---------- input ----------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_MOUSEWHEEL) adjustRadius(currentRadius, e.wheel.y > 0 ? 1 : -1);
            else if (e.type == SDL_KEYDOWN) {
                SDL_Keycode k = e.key.keysym.sym;
                if (k==SDLK_PLUS||k==SDLK_EQUALS||k==SDLK_KP_PLUS||k==SDLK_UP)   adjustRadius(currentRadius, 1);
                if (k==SDLK_MINUS||k==SDLK_KP_MINUS||k==SDLK_DOWN)               adjustRadius(currentRadius,-1);
                if (k==SDLK_ESCAPE) multiPlaceMode = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x, my = e.button.y;
                bool handled = false;
                // --- check buttons ---
                for (auto& b : buttons) {
                    if (pointInRect(mx,my,b.rect)) {
                        handled = true;
                        switch (b.id) {
                            case ButtonId::PLACE: multiPlaceMode = !multiPlaceMode; break;
                            case ButtonId::CLEAR: circles.clear(); lines.clear(); break;
                            case ButtonId::LINE: {
                                // attempt to draw line if exactly 2 selected circles
                                std::vector<const Circle*> sel;
                                for (const auto& c : circles) if (c.selected) sel.push_back(&c);
                                if (sel.size() == 2) {
                                    auto [p1,p2] = lineThroughWindow(sel[0]->center, sel[1]->center);
                                    lines.push_back({p1,p2});
                                    // deselect all circles
                                    for (auto& c : circles) c.selected = false;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                if (handled) continue;

                if (multiPlaceMode) {
                    circles.push_back({ {mx,my}, currentRadius, false });
                } else {
                    // selection toggle
                    for (auto& c : circles) {
                        if (pointInCircle(mx,my,c)) {
                            c.selected = !c.selected;
                            break;
                        }
                    }
                }
            }
        }

        int selectedCount = std::count_if(circles.begin(), circles.end(), [](const Circle& c){return c.selected;});

        // ---------- drawing ----------
        SDL_SetRenderDrawColor(renderer, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, BG_COLOR.a);
        SDL_RenderClear(renderer);

        // buttons
        for (const auto& b : buttons) {
            SDL_Color col = BUTTON_COLOR;
            if (b.id == ButtonId::PLACE && multiPlaceMode) col = BUTTON_ACTIVE;
            if (b.id == ButtonId::LINE) col = BUTTON_LINE;
            SDL_SetRenderDrawColor(renderer, col.r,col.g,col.b,col.a);
            SDL_RenderFillRect(renderer,&b.rect);
            SDL_SetRenderDrawColor(renderer, BORDER_COLOR.r,BORDER_COLOR.g,BORDER_COLOR.b,BORDER_COLOR.a);
            SDL_RenderDrawRect(renderer,&b.rect);
            int tx = b.rect.x + (b.rect.w - static_cast<int>(std::strlen(b.label))*8)/2;
            stringRGBA(renderer, tx, b.rect.y+12, b.label, 255,255,255,255);
        }

        // circles
        for (const auto& c : circles) {
            const SDL_Color& cc = c.selected ? SELECTED_COLOR : CIRCLE_COLOR;
            aacircleRGBA(renderer, c.center.x, c.center.y, c.radius, cc.r, cc.g, cc.b, cc.a);
        }

        // drawn lines
        SDL_SetRenderDrawColor(renderer, DRAWN_LINE_COLOR.r, DRAWN_LINE_COLOR.g, DRAWN_LINE_COLOR.b, DRAWN_LINE_COLOR.a);
        for (const auto& ln : lines) {
            SDL_RenderDrawLine(renderer, ln.p1.x, ln.p1.y, ln.p2.x, ln.p2.y);
        }

        // HUD
        std::string hud = "Radius:"+std::to_string(currentRadius)+ (multiPlaceMode?" (placing)":"") +
                          "  Sel:" + std::to_string(selectedCount)+"  Lines:"+std::to_string(lines.size());
        stringRGBA(renderer, 10, 10, hud.c_str(), 255,255,255,255);

        SDL_RenderPresent(renderer);
    }

    // ---- cleanup ----
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}