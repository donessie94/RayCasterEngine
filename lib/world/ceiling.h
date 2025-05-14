//
//  ceiling.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/15/24.
//

#ifndef ceiling_hpp
#define ceiling_hpp

#include "../engine/settings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Ceiling
{
public:
    Ceiling() : straightDistance(0.0), realDistance(0.0f)
    {
        const char *ceiling_path ="../../textures/textures/skyBackground.png";
        background = IMG_Load(ceiling_path);

        if (!background) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    void draw(float, float, float, float, float, float, float, SDL_Surface**, int, int, void*, int, float, float);

    SDL_Surface* background;
    float straightDistance;
    float realDistance;
};

#endif /* ceiling_hpp */
