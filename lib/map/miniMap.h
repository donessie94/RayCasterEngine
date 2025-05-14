//
//  miniMap.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#ifndef miniMap_hpp
#define miniMap_hpp

#include "../engine/settings.h"
#include <SDL.h>

class MiniMap
{
public:
    MiniMap();
    MiniMap(SDL_Renderer* renderer, float pX, float pY);
    void draw(SDL_Renderer *renderer, float pX, float pY);
    void toggle() {show=!show;}

    SDL_Texture* texture;
    SDL_Texture* minimapFloor;
    SDL_Texture* player;   //it contains the texture representing the player in the minimap (so far just a green color)
    SDL_Rect playerRect;   //representation (a square) of our player in the minimap
    SDL_Rect rect;

    int sizeX;
    int sizeY;
    bool show;

};

#endif /* miniMap_hpp */
