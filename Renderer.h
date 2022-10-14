#pragma once

#include <SDL2/SDL.h>
#include <iostream>

#include "Vec2.h"
#include "DistanceConstraint.h"

class Renderer {
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_index;
    int m_flags;

public:
    Renderer()
      : m_window(nullptr), m_renderer(nullptr), m_index(-1), m_flags(SDL_RENDERER_ACCELERATED) 
    {
    }

    Renderer(const Renderer & renderer) = delete;
    void operator=(const Renderer & renderer) = delete;

    ~Renderer()
    {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    bool create(std::string title, int x, int y, int w, int h, SDL_WindowFlags window_flags)
    {

        m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, window_flags);
        if(m_window == nullptr) {
            std::cout << "Could not create window " << SDL_GetError() << '\n';
            return false;
        }
        m_renderer = SDL_CreateRenderer(m_window, m_index, m_flags);
        if(m_renderer == nullptr) {
            std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
            return false;
        }
        return true;
    }

    bool create()
    {
        m_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
        if(m_window == nullptr) {
            std::cout << "Could not create window " << SDL_GetError() << '\n';
            return false;
        }
        m_renderer = SDL_CreateRenderer(m_window, m_index, m_flags);
        if(m_renderer == nullptr) {
            std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
            return false;
        }
        return true;
    }

    SDL_Texture* texture_from_bmp(std::string path)
    {
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if(!surface) {
            std::cout << "Could not load surface: " << SDL_GetError() << '\n';
            return nullptr;
        }

        SDL_Texture* sdltex = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);
        if(!sdltex) {
            std::cout << "Could not create texture from surface: " << SDL_GetError() << '\n';
            return nullptr;
        }

        return sdltex; 
    }

    // Assume draw to entire texture

    void clear()
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);
    }

    void copy(SDL_Texture * texture)
    {
        SDL_RenderCopy(m_renderer, texture, nullptr, nullptr);
    }

    void copy(SDL_Texture * texture, SDL_Rect rect)
    {
        SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
    }

    void copy(SDL_Texture * texture, int x, int y, int w, int h)
    {
        SDL_Rect r {x, y, w, h};
        SDL_RenderCopy(m_renderer, texture, nullptr, &r);
    }

    void present()
    {
        SDL_RenderPresent(m_renderer);
    }

    int draw_line(int x1, int y1, int x2, int y2)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        return SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
    }

    int draw_line(Vec2 a, Vec2 b)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        return SDL_RenderDrawLine(m_renderer, a.x, a.y, b.x, b.y);
    }

    int draw_line(const DistanceConstraint & dc)
    {
        Vec2 p = dc.p_pos();
        Vec2 q = dc.q_pos();
        return draw_line(p, q);
    }

    int draw_point(Vec2 p)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        return SDL_RenderDrawPoint(m_renderer, p.x, p.y);
    }

    int w() const  
    { 
        int w;
        SDL_GetWindowSize(m_window, &w, nullptr);
        return w; 
    }

    int h() const
    { 
        int h;
        SDL_GetWindowSize(m_window, nullptr, &h);
        return h; 
    }
};
