#pragma once

#include <SDL.h>
#include <algorithm>   // std::clamp
#include <cmath>       // std::round
#include <vector>

#include "constants.hpp"
#include "types.hpp"

//-------------------------------------------------------------------------
// Small, self‑contained helper functions that don’t hold state. All are
// declared `inline` so this header can be included anywhere without
// requiring a separate utils.cpp translation unit.
//-------------------------------------------------------------------------

/// Clamp the radius within [MIN_RADIUS, MAX_RADIUS].
/// @return true if the value actually changed.
inline bool adjustRadius(int& radius, int delta)
{
    int newVal = std::clamp(radius + delta, MIN_RADIUS, MAX_RADIUS);
    if (newVal != radius) {
        radius = newVal;
        return true;
    }
    return false;
}

/// Simple AABB hit‑test.
inline bool pointInRect(int x, int y, const SDL_Rect& r)
{
    return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

/// Circle hit‑test using squared distance to avoid sqrt.
inline bool pointInCircle(int x, int y, const Circle& c)
{
    int dx = x - c.center.x;
    int dy = y - c.center.y;
    return dx * dx + dy * dy <= c.radius * c.radius;
}

/// Given two arbitrary points, return the two endpoints of the line
/// segment where the infinite line intersects the window borders. This
/// lets us draw an "infinite" line with a single SDL_RenderDrawLine.
inline std::pair<SDL_Point, SDL_Point> lineThroughWindow(const SDL_Point& p1,
                                                        const SDL_Point& p2)
{
    // Vertical line special‑case
    if (p1.x == p2.x) {
        return { SDL_Point{ p1.x, 0 }, SDL_Point{ p1.x, WINDOW_HEIGHT } };
    }

    const double m = static_cast<double>(p2.y - p1.y) /
                     static_cast<double>(p2.x - p1.x);
    const double b = p1.y - m * p1.x;

    std::vector<SDL_Point> pts;
    // Intersections with vertical borders
    int y_left  = static_cast<int>(std::round(b));
    if (y_left >= 0 && y_left <= WINDOW_HEIGHT) pts.push_back({ 0, y_left });

    int y_right = static_cast<int>(std::round(m * WINDOW_WIDTH + b));
    if (y_right >= 0 && y_right <= WINDOW_HEIGHT) pts.push_back({ WINDOW_WIDTH, y_right });

    // Intersections with horizontal borders
    int x_top = static_cast<int>(std::round(-b / m));
    if (x_top >= 0 && x_top <= WINDOW_WIDTH) pts.push_back({ x_top, 0 });

    int x_bottom = static_cast<int>(std::round((WINDOW_HEIGHT - b) / m));
    if (x_bottom >= 0 && x_bottom <= WINDOW_WIDTH) pts.push_back({ x_bottom, WINDOW_HEIGHT });

    if (pts.size() < 2) return { p1, p2 };          // Fallback (parallel?)
    return { pts[0], pts[1] };                      // First two valid endpoints
}
