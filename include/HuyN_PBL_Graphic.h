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
        TTF_CloseFont(font);
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

    TTF_CloseFont(font);

    return textTexture;
}

void SDLGraphic_RenderText(SDL_Renderer *renderer, const char *text, int fontSize, SDL_Color fgColor, SDL_Rect textRect)
{
    SDL_Texture *textTexture = getTextTexture(renderer, text, fontSize, fgColor);

    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
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


void SDLGraphic_RenderDrawTick(SDL_Renderer *renderer, int x, int y)
{
    const int boxSize = 30;
    SDL_Rect box = {x, y, boxSize, boxSize};

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

    SDL_RenderDrawLine(renderer, x + 5, y + boxSize - boxSize / 3, x + boxSize / 2, y + boxSize - 5);
    SDL_RenderDrawLine(renderer, x + 5, y + boxSize - boxSize / 3 - 1, x + boxSize / 2, y + boxSize - 4);

    SDL_RenderDrawLine(renderer, x + boxSize / 2, y + boxSize - 5, x + boxSize - 5, y + 5);

    int dx = (x + boxSize - 5) - (x + boxSize / 2);
    int dy = (y + 5) - (y + boxSize - 5);
    float length = sqrt(dx * dx + dy * dy);
    dx = (int)(dx / length);
    dy = (int)(dy / length);

    int perp_dx = -dy;
    int perp_dy = dx;

    perp_dx = 1;
    perp_dy = 0;
    SDL_RenderDrawLine(renderer,
                       x + boxSize / 2 + perp_dx, y + boxSize - 5 + perp_dy,
                       x + boxSize - 5 + perp_dx, y + 5 + perp_dy);
}


void SDLGraphic_DrawNode(SDL_Renderer *renderer, const SDL_Pos _p, int _id, bool flag, SDL_Rect validZone)
{
    const int shadowOffset = 5;
    const int centerX = validZone.x + validZone.w / 2;

    // Decide direction of offset
    int offsetX = (_p.x < centerX) ? -shadowOffset : shadowOffset;
    int offsetY = shadowOffset;

    // Draw shadow
    SDL_SetRenderDrawColor(renderer, 0x58, 0x58, 0x58, 0xAA); // semi-transparent dark
    SDL_RenderFillCircle(renderer, _p.x + offsetX, _p.y + offsetY, 20);

    // Draw filled node
    SDL_SetRenderDrawColor(renderer, 0x35, 0x42, 0x59, 0xFF);
    SDL_RenderFillCircle(renderer, _p.x, _p.y, 20);

    // Draw outline
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawCircle(renderer, _p.x, _p.y, 20);

    // Draw text
    int textWidth = 16, textHeight = 16;
    char buffer[20];
    sprintf(buffer, "%d", _id);
    SDL_Texture *textTexture = getTextTexture(renderer, buffer, 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF});
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {_p.x - textWidth / 2, _p.y - textHeight / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    if (flag) SDL_RenderPresent(renderer);
}


void SDLGraphic_ConnectNode(SDL_Renderer *renderer, const SDL_Pos _p_node1, const SDL_Pos _p_node2, const int cost, const bool selected, int id1, int id2)
{
    if (cost < 0 && !selected) return;

    // Node radius
    const int nodeRadius = 20;
    const int arrowLength = 10; // Length of the arrowhead

    // Set draw color
    (selected ? SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF) : SDL_SetRenderDrawColor(renderer, 0x14, 0x1E, 0x61, 0xFF));

    // Detect problematic case - node1 is to the upper-left of node2
    bool problematicCase = (_p_node1.x < _p_node2.x && _p_node1.y < _p_node2.y);

    // Base offset - larger for problematic cases
    int baseOffset = problematicCase ? 20 : 10;

    // Use perpendicular offset for problematic cases
    int x_offset, y_offset;

    if (problematicCase) {
        // Calculate perpendicular direction for offset
        double dx = _p_node2.x - _p_node1.x;
        double dy = _p_node2.y - _p_node1.y;
        double len = sqrt(dx*dx + dy*dy);

        // Perpendicular vector
        double perpX = -dy / len;
        double perpY = dx / len;

        // Alternate direction based on IDs
        if ((id1 + id2) % 2 == 0) {
            perpX = -perpX;
            perpY = -perpY;
        }

        // Apply perpendicular offset
        x_offset = baseOffset * perpX + (id1 * 3) % 10;
        y_offset = baseOffset * perpY + (id1 * 3) % 10;
    } else {
        // Regular offset for non-problematic cases
        x_offset = (id1 < id2 ? -baseOffset : baseOffset) + (id1 * 5) % 10;
        y_offset = (id1 < id2 ? -baseOffset : baseOffset) + (id1 * 5) % 10;
    }

    int x_start = _p_node1.x + x_offset;
    int y_start = _p_node1.y + y_offset;
    int x_dest = _p_node2.x + x_offset;
    int y_dest = _p_node2.y + y_offset;

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

    if (cost < 0) return;

    // Find midpoint for cost display
    double x_midPoint = x_start + (x_dest - x_start) * 0.5;
    double y_midPoint = y_start + (y_dest - y_start) * 0.5;

    double offsetNum = 30.0;
    // Shift the label slightly forward or backward along the edge line
    double label_offset = ((id1 + id2) % 3 - 1) * offsetNum;  // -30, 0, or 30
    x_midPoint += label_offset * cos(angle);
    y_midPoint += label_offset * sin(angle);

    // int to string
    char buffer[20];
    sprintf(buffer, "%d", cost);
    SDL_Texture *textTexture = getTextTexture(renderer, buffer, 20, (SDL_Color){0x00, 0xFF, 0xFF, 0xFF});

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {x_midPoint - textWidth / 2, y_midPoint - textHeight / 2, textWidth, textHeight};
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &textRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
}

void hslToRgb(float h, float s, float l, int* r, int* g, int* b) {
    float c = (1 - fabsf(2 * l - 1)) * s;
    float x = c * (1 - fabsf(fmodf(h / 60.0, 2) - 1));
    float m = l - c / 2;
    float r1, g1, b1;

    if (h < 60)       { r1 = c; g1 = x; b1 = 0; }
    else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
    else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
    else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
    else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
    else              { r1 = c; g1 = 0; b1 = x; }

    *r = (int)((r1 + m) * 255);
    *g = (int)((g1 + m) * 255);
    *b = (int)((b1 + m) * 255);
}

void renderRainbowHover(SDL_Renderer *renderer, SDL_Rect rect, int frameOffset) {
    for (int x = 0; x < rect.w; x++) {
        float hue = fmodf((x + frameOffset) * 360.0f / rect.w, 360.0f);
        int r, g, b;
        hslToRgb(hue, 1.0f, 0.5f, &r, &g, &b);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, rect.x + x, rect.y, rect.x + x, rect.y + rect.h);
    }
}



#endif //HuyN_PBL_GRAPHIC_H