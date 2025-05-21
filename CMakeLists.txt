cmake_minimum_required(VERSION 3.25)   # or any 3.2x you have
project(my_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)

add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2::SDL2_gfx
)

# rpath so the bundled libSDL2*.dylib is found beside the executable
if(APPLE)
    set_target_properties(my_app PROPERTIES INSTALL_RPATH "@loader_path")
elseif(UNIX)
    set_target_properties(my_app PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()
