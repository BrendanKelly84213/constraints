#pragma once

#include <SDL2/SDL.h>
#include "Window.h"
#include "Texture.h"

class Renderer : Window {
private:
    SDL_Renderer* m_renderer;
    int m_index;
    int m_flags;

public:
    Renderer()
      : Window(), m_renderer(nullptr), m_index(-1), m_flags(SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC) 
    {
    }

    ~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
    }

    bool create(std::string title, int x, int y, int w, int h, SDL_WindowFlags window_flags)
    {
        Window::create(title.c_str(), x, y, w, h, window_flags);
        m_renderer = SDL_CreateRenderer(Window::m_window, m_index, m_flags);
        if(m_renderer == nullptr) {
            std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
            return false;
        }
        return true;
    }

    bool create()
    {
        Window::create("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
        m_renderer = SDL_CreateRenderer(Window::m_window, m_index, m_flags);
        if(m_renderer == nullptr) {
            std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
            return false;
        }
        return true;
    }

    Texture texture_from_bmp(std::string path)
    {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if(!surface) {
            std::cout << "Could not load surface: " << SDL_GetError() << '\n';
            return Texture(nullptr);
        }

        SDL_Texture* sdltex = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);
        if(!sdltex) {
            std::cout << "Could not create texture from surface: " << SDL_GetError() << '\n';
            return Texture(nullptr);
        }

        return Texture(sdltex); 
    }

    // Assume draw to entire texture
    void draw(Texture & texture)
    {
        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, texture.ptr(), NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }

    void draw(Texture & texture, int x, int y, int w, int h)
    {
        SDL_Rect rect = { x, y, w, h };
        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, texture.ptr(), NULL, &rect);
        SDL_RenderPresent(m_renderer);
    }

    int w() const { return Window::w(); }
    int h() const { return Window::h(); }
};
