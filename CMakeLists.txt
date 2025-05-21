cmake_minimum_required(VERSION 3.25)
project(the_board LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)

add_executable(the_board
    src/main.cpp
)

target_link_libraries(the_board PRIVATE
    SDL2::SDL2
    SDL2::SDL2_gfx
)


# rpath so bundled libs are found beside the executable
if(APPLE)
    set_target_properties(the_board PROPERTIES INSTALL_RPATH \"@loader_path\")
elseif(UNIX)
    set_target_properties(the_board PROPERTIES INSTALL_RPATH \"\$ORIGIN\")
endif()
