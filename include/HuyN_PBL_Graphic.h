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
    Uint32 x;
    Uint32 y;
} SDL_Pos;

// ******************************************** ERROR HANDLER ******************************************** //

void SDLGraphic_ErrorHandler(char *message,...)
{
    SDL_Log("%s failed: %s\n", message);
    SDL_Quit();
    TTF_Quit();
    exit(1);
}

// *********************************************** SDL_TTF *********************************************** //


TTF_Font *Font()
{
    TTF_Font *font = TTF_OpenFont("Roboto.ttf", 30);
    if (!font)
{
        SDLGraphic_ErrorHandler("TTF_OpenFont", TTF_GetError());
    }
    return font;
}


SDL_Texture *getTextTexture(SDL_Renderer *renderer, const char *text)
{

    TTF_Font *font = Font();

    const SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF};

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface)
    {
        SDLGraphic_ErrorHandler("TTF_RenderText_Solid", TTF_GetError());
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture)
    {
        SDLGraphic_ErrorHandler("SDL_CreateTextureFromSurface", SDL_GetError());
    }

    return textTexture;
}


// ********************************************** SDL DRAW ********************************************** //


int SDL_RenderDrawCircle(SDL_Renderer *renderer,const int x,const int y,const int radius)
{
    int offsetX = 0;
    int offsetY = radius;
    int d = radius - 1;
    int status = 0;
    while (offsetY >= offsetX)
    {
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y + offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y + offsetX);
        status += SDL_RenderDrawPoint(renderer, x + offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x + offsetY, y - offsetX);
        status += SDL_RenderDrawPoint(renderer, x - offsetX, y - offsetY);
        status += SDL_RenderDrawPoint(renderer, x - offsetY, y - offsetX);
        if (status < 0)
        {
            status = -1;
            break;
        }
        if (d >= 2*offsetX)
        {
            d -= 2*offsetX + 1;
            offsetX +=1;
        }
        else if (d < 2 * (radius - offsetY))
        {
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

void SDLGraphic_DrawNode(SDL_Renderer *renderer, const SDL_Pos _p)
{
    SDL_RenderDrawCircle(renderer, _p.x, _p.y, 10);
}

void SDLGraphic_ConnectNode(SDL_Renderer *renderer, const SDL_Pos _p_node1, const SDL_Pos _p_node2, const int cost)
{
    int x_start = _p_node1.x, y_start = _p_node1.y, x_dest = _p_node1.x, y_dest = _p_node1.y;
    x_start /= 2 - 5;
    y_start /= 2 - 5;

    const int x_midPoint = (x_start + x_dest) / 2;
    const int y_midPoint = (y_start + y_dest) / 2;

    x_dest /= 2 - 5;
    y_dest /= 2 - 5;
    SDL_RenderDrawLine(renderer, x_start, y_start, x_midPoint - 5, y_midPoint - 5);


    // TODO: USE SDL_TTF TO ADD COST NUMBER IN BETWEEN

    int textWidth = 10, textHeight = 10;

    SDL_Texture *textTexture = getTextTexture(renderer, cost + "");

    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {x_midPoint - 5, y_midPoint - 5, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderDrawLine(renderer, x_midPoint + 5, y_midPoint + 5, x_dest, y_dest);
}



#endif //HuyN_PBL_GRAPHIC_H
