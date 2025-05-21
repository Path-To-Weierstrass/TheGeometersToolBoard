#pragma once

#include <SDL.h>

//-------------------------------------------------------------------------
// Global configuration & color palette
//-------------------------------------------------------------------------

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

// Circle radius limits
constexpr int DEFAULT_RADIUS = 10;
constexpr int MIN_RADIUS     = 3;
constexpr int MAX_RADIUS     = 60;

// Background & UI colors
constexpr SDL_Color BG_COLOR         {30,  30,  30,  255};
constexpr SDL_Color BUTTON_COLOR     {0,   120, 215, 255};
constexpr SDL_Color BUTTON_ACTIVE    {0,   180, 120, 255};
constexpr SDL_Color BUTTON_LINE      {120, 60,  200, 255};

// Drawing colors
constexpr SDL_Color CIRCLE_COLOR     {255, 255, 255, 255};
constexpr SDL_Color SELECTED_COLOR   {255, 255, 0,   255};
constexpr SDL_Color SELECTED_LINE_COLOR {255, 1, 0,   255};
constexpr SDL_Color BORDER_COLOR     {255, 255, 255, 255};
constexpr SDL_Color DRAWN_LINE_COLOR {200, 200, 255, 255};

constexpr float SELECTION_RADIUS = 10.0f;