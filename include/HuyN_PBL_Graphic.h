//
// Created by HuyN on 30/04/2025.
//
#pragma once

#include <stdio.h>

#include "SDL.h"
#include "SDL_ttf.h"
#include <math.h>

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


TTF_Font *Font(const int size)
{
    TTF_Font *font = TTF_OpenFont("Roboto.ttf", size);
    if (!font)
{
        SDLGraphic_ErrorHandler("TTF_OpenFont", TTF_GetError());
    }
    return font;
}


SDL_Texture *getTextTexture(SDL_Renderer *renderer, const char *text, int fontSize, SDL_Color fgColor)
{

    TTF_Font *font = Font(fontSize);

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, fgColor);
    if (!textSurface)
    {
        SDLGraphic_ErrorHandler("TTF_RenderText", TTF_GetError());
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

int SDL_RenderFillCircle(SDL_Renderer *renderer, const int x, const int y, const int radius) {
    int offsetX = 0;
    int offsetY = radius;
    int d = radius - 1;
    int status = 0;
    while (offsetY >= offsetX) {
        status += SDL_RenderDrawLine(renderer, x - offsetY, y + offsetX, x + offsetY, y + offsetX);
        status += SDL_RenderDrawLine(renderer, x - offsetX, y + offsetY, x + offsetX, y + offsetY);
        status += SDL_RenderDrawLine(renderer, x - offsetX, y - offsetY, x + offsetX, y - offsetY);
        status += SDL_RenderDrawLine(renderer, x - offsetY, y - offsetX, x + offsetY, y - offsetX);
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

void SDLGraphic_DrawNode(SDL_Renderer *renderer, const SDL_Pos _p, int _id, bool flag)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillCircle(renderer, _p.x, _p.y, 20);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawCircle(renderer, _p.x, _p.y, 20);
    int textWidth = 16, textHeight = 16;
    char buffer[20];
    sprintf(buffer, "%d", _id);
    SDL_Texture *textTexture = getTextTexture(renderer, buffer, 25, (SDL_Color){0xFF, 0xFF, 0x00, 0xFF});
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {_p.x - textWidth / 2, _p.y - textHeight / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    if (flag) SDL_RenderPresent(renderer);
}

// void SDLGraphic_ConnectNode(SDL_Renderer *renderer, const SDL_Pos _p_node1, const SDL_Pos _p_node2, const int cost, const bool selected, int id1, int id2)
// {
//
//     // Set draw color
//     (selected ? SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF) : SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));
//
//     int x_start = _p_node1.x + (id1 < id2? -10 : 10 ),
//     y_start = _p_node1.y + (id1 < id2? -10 : 10 ),
//     x_dest = _p_node2.x + (id1 < id2? -10 : 10 ),
//     y_dest = _p_node2.y + (id1 < id2? -10 : 10 );
//
//     const int x_midPoint = x_start + (x_dest - x_start) / 2;
//     const int y_midPoint = y_start + (y_dest - y_start) / 2;
//
//     SDL_RenderDrawLine(renderer, x_start, y_start, x_dest, y_dest);
//
//     // TODO: USE SDL_TTF TO ADD COST NUMBER IN BETWEEN INCLUDED CASE WHEN COST IS NULL ( -1 )
//     if (cost != -1)
//     {
//         int textWidth = 60, textHeight = 60;
//
//         // int to string
//         char buffer[20];
//         sprintf(buffer, "%d", cost);
//
//         SDL_Texture *textTexture = getTextTexture(renderer, buffer, 20, (SDL_Color){0x00, 0xFF, 0xFF, 0xFF});
//
//         SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
//         SDL_Rect textRect = {x_midPoint, y_midPoint, textWidth, textHeight};
//         SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
//         SDL_RenderFillRect(renderer, &textRect);
//         SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
//         SDL_DestroyTexture(textTexture);
//     }
//
// }


void SDLGraphic_ConnectNode(SDL_Renderer *renderer, const SDL_Pos _p_node1, const SDL_Pos _p_node2, const int cost, const bool selected, int id1, int id2)
{
    // Node radius
    const int nodeRadius = 20;
    const int arrowLength = 10; // Length of the arrowhead

    // Set draw color
    (selected ? SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF) : SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF));

    int offset = 5; // Offset to differentiate overlapping lines
    int x_start = _p_node1.x + (id1 < id2 ? -10 : 10) + (id1 * offset) % 10;
    int y_start = _p_node1.y + (id1 < id2 ? -10 : 10) + (id1 * offset) % 10;
    int x_dest = _p_node2.x + (id1 < id2 ? -10 : 10) + (id2 * offset) % 10;
    int y_dest = _p_node2.y + (id1 < id2 ? -10 : 10) + (id2 * offset) % 10;

    // Calculate direction and back off by radius
    double angle = atan2(y_dest - y_start, x_dest - x_start);
    x_dest -= nodeRadius * cos(angle);
    y_dest -= nodeRadius * sin(angle);

    // Draw the main line
    SDL_RenderDrawLine(renderer, x_start, y_start, x_dest, y_dest);

    // Calculate arrowhead points
    int arrow_x1 = x_dest - arrowLength * cos(angle - M_PI / 6);
    int arrow_y1 = y_dest - arrowLength * sin(angle - M_PI / 6);
    int arrow_x2 = x_dest - arrowLength * cos(angle + M_PI / 6);
    int arrow_y2 = y_dest - arrowLength * sin(angle + M_PI / 6);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

    // Draw the arrowhead
    SDL_RenderDrawLine(renderer, x_dest, y_dest, arrow_x1, arrow_y1);
    SDL_RenderDrawLine(renderer, x_dest, y_dest, arrow_x2, arrow_y2);

    // Render cost if provided
    if (cost > 0)
    {
        int textWidth = 60, textHeight = 60;

        // int to string
        char buffer[20];
        sprintf(buffer, "%d", cost);

        SDL_Texture *textTexture = getTextTexture(renderer, buffer, 20, (SDL_Color){0x00, 0xFF, 0xFF, 0xFF});

        // Find midpoint for cost display
        const int x_midPoint = x_start + (x_dest - x_start) / 2;
        const int y_midPoint = y_start + (y_dest - y_start) / 2;

        SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
        SDL_Rect textRect = {x_midPoint - textWidth / 2, y_midPoint - textHeight / 2, textWidth, textHeight};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &textRect);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
}



#endif //HuyN_PBL_GRAPHIC_H
