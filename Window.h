#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <cstring>
#include <iostream>

class Window {
friend class Renderer;
private: 
    SDL_Window* m_window;

public: 
    Window() 
        : m_window(nullptr)
    {
    }

    ~Window()
    {
        SDL_DestroyWindow(m_window);
    }

    bool create(std::string title, int x, int y, int w, int h, SDL_WindowFlags flags)
    {
        m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
        if(m_window == nullptr) {
            std::cout << "Could not create window " << SDL_GetError() << '\n';
            return false;
        }
        return true;
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
