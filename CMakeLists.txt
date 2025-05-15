cmake_minimum_required(VERSION 3.30)
project(PBL C)

set(CMAKE_C_STANDARD 23)

set(SDL2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SDL2/include)
set(SDL2_LIB_DIR ${CMAKE_BINARY_DIR}/SDL2/lib)

set(HUYN_PBL_GRAPHIC ${CMAKE_SOURCE_DIR}/include)

include_directories(${SDL2_INCLUDE_DIR} ${HUYN_PBL_GRAPHIC})
link_directories(${SDL2_LIB_DIR})

add_executable(PBL ${CMAKE_SOURCE_DIR}/src/main.c
        include/HuyN_PBL_Graphic.h
        include/HuyN_SDL_Button.h)

target_link_libraries(PBL SDL2main SDL2 SDL2_ttf)

file(COPY ${SDL2_LIB_DIR}/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${SDL2_LIB_DIR}/SDL2_ttf.dll DESTINATION ${CMAKE_BINARY_DIR})