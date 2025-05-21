#pragma once

#include <SDL.h>
#include <cstdint>

//-------------------------------------------------------------------------
// Plain data structures and lightweight enums used throughout the app.
// No function logic lives here—just POD‑style definitions that any
// translation unit can include without dragging in heavy dependencies.
//-------------------------------------------------------------------------

/// All UI buttons are identified by a simple enum.
enum class ButtonId : std::uint8_t { PLACE, CLEAR, LINE, PARALLEL };

/// Rectangle + label + semantic ID.
struct Button {
    SDL_Rect   rect;   ///< Pixel bounds on screen
    const char* label; ///< Null‑terminated text shown on the button
    ButtonId   id;     ///< What this button triggers
};

/// A drawable/interactive circle on the canvas.
struct Circle {
    SDL_Point center;   ///< Centre coordinate in window space
    int       radius;   ///< Pixels
    bool      selected = false; ///< Highlighted by user?
};

/// Pre‑calculated infinite line (already clipped to window edges).
struct Line {
    SDL_Point p1; ///< One end of the rendered segment
    SDL_Point p2; ///< Other end
    bool selected = false;
};
