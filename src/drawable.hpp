// Drawable.hpp
#pragma once

#include <SDL.h>

class Drawable {
public:
    virtual void draw(SDL_Renderer* renderer) const = 0;
    virtual bool isSelected(int x, int y) const = 0; // optional, depending on use
    virtual ~Drawable() = default;
};
