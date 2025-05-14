//
//  rayCaster.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#ifndef rayCaster_hpp
#define rayCaster_hpp

#include <SDL.h>
#include "../player/player.h"
#include "../map/miniMap.h"
#include "../world/walls.h"
#include "../engine/ray.h"
#include "../world/floor.h"
#include "../world/ceiling.h"
#include <algorithm>
#include <unordered_map>

class RayCaster
{
public:
    RayCaster();
    void drawBackground();
    void eventLoop();
    void rayDirectionSetUp();//set ups the ray direction(angle) of our initial ray to be casted
    void drawScreen();
    void castRays();
    void loadImages();

    std::unordered_map<float, float> sinDirection;
    std::unordered_map<float, float> cosDirection;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Surface* spritePlayer;
    SDL_Texture* playerSprite;
    SDL_Rect playerFrame;   //our image is 312x89 and 9 pics so a pic every 40 pixels (almost)
    SDL_Rect playerSpritePosition;

    SDL_Texture* screen;
    SDL_Surface* wallImage[6];
    SDL_Surface* floorTexture[4];
    SDL_Surface* ceilingTexture[4];
    const Uint8 *keyState;  //for our input handler methods

    //SDL_Surface* background;
    //std::unordered_map<float, float> sinA;
    //std::unordered_map<float, float> cosA;
    //SDL_Rect halfScreen;

    Player* player;
    MiniMap* miniMap;
    Wall* wall;
    Ray* ray;
    Floor* floor;
    Ceiling* ceiling;


    float fElapsedTime;     //hack for FPS for now CHECK LATER!!!
    bool running;
    int mouseX;
    int mouseY;
    int counterFrames;

    int pitch;         //pitch and pixel for manipulating pixels in
    void* pixel;            //our screen usind sdl_locktexture
    uint8_t rMask, gMask, bMask; //note we not using (at least not yet) in the rgba format, the 'a' mask
};


#endif /* rayCaster_hpp */
