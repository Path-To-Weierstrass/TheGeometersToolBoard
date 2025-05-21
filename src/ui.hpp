#pragma once

#include <SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string>
#include <vector>
#include <cstring>   // std::strlen

#include "constants.hpp"
#include "types.hpp"
#include "utils.hpp"

//-------------------------------------------------------------------------
// UI drawing helpers.  Keeping these in one header lets any translation
// unit render buttons, circles, lines or HUD text without repeating code.
// Implemented `inline` so there is no ui.cpp to link.
//-------------------------------------------------------------------------

/// Render an SDL button with optional active colour.
inline void drawButton(SDL_Renderer* r, const Button& b, bool active = false)
{
    const SDL_Color& fill = active ? BUTTON_ACTIVE
                                   : (b.id == ButtonId::LINE ? BUTTON_LINE : BUTTON_COLOR);
    SDL_SetRenderDrawColor(r, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderFillRect(r, &b.rect);

    SDL_SetRenderDrawColor(r, BORDER_COLOR.r, BORDER_COLOR.g, BORDER_COLOR.b, BORDER_COLOR.a);
    SDL_RenderDrawRect(r, &b.rect);

    int tx = b.rect.x + (b.rect.w - static_cast<int>(std::strlen(b.label))*8) / 2;
    stringRGBA(r, tx, b.rect.y + 12, b.label, 255, 255, 255, 255);
}

/// Draw all circles with colour depending on selection.
inline void drawCircles(SDL_Renderer* r, const std::vector<Circle>& circles)
{
    for (const auto& c : circles) {
        const SDL_Color& col = c.selected ? SELECTED_COLOR : CIRCLE_COLOR;
        aacircleRGBA(r, c.center.x, c.center.y, c.radius, col.r, col.g, col.b, col.a);
    }
}

/// Draw all stored infinite lines.
inline void drawLines(SDL_Renderer* r, const std::vector<Line>& lines)
{
    SDL_SetRenderDrawColor(r, DRAWN_LINE_COLOR.r, DRAWN_LINE_COLOR.g, DRAWN_LINE_COLOR.b, DRAWN_LINE_COLOR.a);
    for (const auto& ln : lines) {
        SDL_RenderDrawLine(r, ln.p1.x, ln.p1.y, ln.p2.x, ln.p2.y);
    }
}

/// Draw top‑left HUD overlay.
inline void drawHUD(SDL_Renderer* r, int radius, bool placing, int selCount, int lineCount)
{
    std::string txt = "Radius:" + std::to_string(radius);
    if (placing) txt += " (placing)";
    txt += "  Sel:" + std::to_string(selCount) + "  Lines:" + std::to_string(lineCount);
    stringRGBA(r, 10, 10, txt.c_str(), 255, 255, 255, 255);
}
