//
// Created by HuyN on 15/05/2025.
//
#pragma once

#include "HuyN_PBL_Graphic.h"
#include "SDL.h"

#ifndef HUYN_SDL_BUTTON_H
#define HUYN_SDL_BUTTON_H


// typedef struct {
//     Uint32 x;
//     Uint32 y;
// } SDL_Pos;


typedef struct
{
    int x;
    int y;
    int w;
    int h;

    char *text;

    SDL_Color bgColor;
    SDL_Color fgColor;

} HuyN_SDL_Button;


bool SDLButton_isContain(const SDL_Pos _p, HuyN_SDL_Button _button)
{
    return (
        _p.x >= _button.x &&
        _p.x <= _button.x + _button.w &&
        _p.y >= _button.y &&
        _p.y <= _button.y + _button.h
        );
};

void SDLButton_draw(SDL_Renderer *renderer, const HuyN_SDL_Button _button)
{
    SDL_SetRenderDrawColor(renderer, _button.bgColor.r, _button.bgColor.g, _button.bgColor.b, _button.bgColor.a);
    const SDL_Rect rect = {_button.x, _button.y, _button.w, _button.h};
    SDL_RenderFillRect(renderer, &rect);
    // text
    SDL_SetRenderDrawColor(renderer, _button.fgColor.r, _button.fgColor.g, _button.fgColor.b, _button.fgColor.a);
    SDL_Texture *textTexture = getTextTexture(renderer, _button.text, (_button.text == "CREDIT" ? 15 : 25), _button.fgColor);
    int textRectW = rect.w - 6, textRectH = rect.h - 6;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRectW, &textRectH);
    SDL_Rect textRect = {rect.x + 3, rect.y + 3, textRectW, textRectH};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
};


#endif //HUYN_SDL_BUTTON_H
