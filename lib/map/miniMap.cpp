//
//  miniMap.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#include "miniMap.h"
#include <SDL.h>
                                                                    //1536
MiniMap::MiniMap(SDL_Renderer* renderer, float pX, float pY) : sizeX{192}, sizeY{192}
{
    rect.w=sizeX/mapWidth;
    rect.h=sizeY/mapHeight;
    rect.x=0;
    rect.y=0;

    show=false;

    float w=rect.w;
    float h=rect.h;
    int scalar=4; //else we wont see the player on the minimap (it will be way to small)
    int scalarMinimap=8;//it was 2 because our minimap size was 192x192 and the world size was 384x384 (2x relation)
                        //since i changed world size to 1536 and still 192 minimap so 1536/192=8 now

    //debuging stuff
    //int scalar=4;
    //int scalarMinimap=1;

    float pw=w/blockSize;
    float ph=h/blockSize;

    playerRect.w=std::ceil(pw)*scalar; //since doing it 1x1 will result in a way to small representation impossible to see in the minimap
    playerRect.h=std::ceil(ph)*scalar;  //this create a problem we need to scale the (x,y) coordinates also accouting for this scalar number

    playerRect.x=pX/scalarMinimap;           //we need to find what the player (x,y) coordinates maps to in our minimap world
    playerRect.y=pY/scalarMinimap;

    //creating the texture of the walls here
    texture=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 87, 23, 5, 255);   //redish color for walls in minimap
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);//set rendering target to the whole windows instead of any texture

    minimapFloor=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, minimapFloor);
    SDL_SetRenderDrawColor(renderer, 222, 191, 182, 255);   //whitish color for walls in minimap
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);//set rendering target to the whole windows instead of any texture

    //creating the player texture here
    player=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, playerRect.w, playerRect.h);
    SDL_SetRenderTarget(renderer, player);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   //green color for our player
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);//set rendering target to the whole windows instead of any texture
}

void MiniMap::draw(SDL_Renderer *renderer, float pX, float pY)
{
    for(int row=0;row<mapHeight;row++)//lets traverse the whole worldMap array to draw the minimap, each time we find a value
    {                                 //different than 0 (0 represent an empty space we could move trhough)
        for(int col=0;col<mapWidth;col++)
        {

            if(WorldMap[row][col]>0)    //greater than 0 means a wall is in that position
            {
                rect.x=col*rect.w;    //updating the starting drawing position of our rectangles position
                rect.y=row*rect.h;    //where we will draw o the screen the either wall or floor minimap texture
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            }
            else
            {
                rect.x=col*rect.w;
                rect.y=row*rect.h;
                SDL_RenderCopy(renderer, minimapFloor, NULL, &rect);
            }
        }
    }

    int scalar=8; //minimapScalar remember 192x192 and world is 384x384
                  // we went from 16 block size to 64 (for better quality in textures)
                  //so now its 8

    //debuging stuff
    //int scalar=1;

    float x=pX/scalar;
    float y=pY/scalar;

    playerRect.x=x; playerRect.y=y;

    SDL_RenderCopy(renderer, player, NULL, &playerRect);    //drawing lastly the player on the minimap
}
