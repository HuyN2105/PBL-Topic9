//
// Created by HuyN on 30/04/2025.
//
#pragma once

#include <stdio.h>

#include "SDL.h"
#include "SDL_ttf.h"

#ifndef HuyN_PBL_GRAPHIC_H
#define HuyN_PBL_GRAPHIC_H

typedef struct {
    int x;
    int y;
} SDL_Pos;


int SDL_RenderDrawCircle(SDL_Renderer *renderer,const int x,const int y,const int radius) {
    int offsetX = 0;
    int offsetY = radius;
    int d = radius - 1;
    int status = 0;
    while (offsetY >= offsetX) {
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);
        if (status < 0) {
            status = -1;
            break;
        }
        if (d >= 2*offsetX) {
            d -= 2*offsetX + 1;
            offsetX +=1;
        }
        else if (d < 2 * (radius - offsetY)) {
            d += 2 * offsetY - 1;
            offsetY -= 1;
        }
        else {
            d += 2 * (offsetY - offsetX - 1);
            offsetY -= 1;
            offsetX += 1;
        }
    }
    return status;
};


void SDLGraphic_ErrorHandler(char *message, char *getErrorMessage){
    printf("%s failed: %s\n", message, getErrorMessage);
    exit(1);
}

void SDLGraphic_DrawNode(SDL_Renderer *renderer, SDL_Pos _p) {
    SDL_RenderDrawCircle(renderer, _p.x, _p.y, 10);
}

#endif //PBL_GRAPHIC_H
