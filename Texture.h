#pragma once

#include <SDL2/SDL.h>
#include <string>

class Texture {
private: 
    SDL_Texture* m_texture;

public:

    Texture(SDL_Texture* tex) 
        : m_texture(tex)
    {
    }

    ~Texture()
    {
        SDL_DestroyTexture(m_texture);
    }

    SDL_Texture* ptr() const { return m_texture; }
};
