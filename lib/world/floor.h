//
//  floor.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/13/24.
//

#ifndef floor_hpp
#define floor_hpp

#include "../engine/settings.h"
#include <SDL.h>

class Floor
{
public:
    //NOTES ABOUT CASTING FLOORS:
    //
    //  (PJ) is the point P projection on the screen
    //  NOTE THAT we know this row value on the screen since its just->   maxHeight of wall + 1 = row value of PJ point on screen
    //  its maxHeight + 1 cuz remember our 'y' increments downwards                                     (0,0)------------> x increments
    //  NOTE THAT triangle (P H F) is similar to triangle (PJ H PJ') THUS:                                  |
    //                                                                                                      |
    //  straightDistance from player's feets to point P      player's height                   y increments |           <-- our coordinate system
    // ------------------------------------------------- = ----------------------
    //           player's distance to screen                   line(PJ' H)
    //
    //
    //  AND: line(PJ' H) = line(PJ Horizont) = PJ row number - Horizont(screen center row number)   REMEMEBER: 'y' grows downward!
    //  THUS: straightDistance to point P = player's heihgt(screen center) * player's distance to screen / line(PJ' H)
    //
    //           Projection Plane (screen)
    //                    ||
    //                    ||
    //                    ||
    //                    ||
    //                    ||
    //                    ||
    //                    ||
    //   (screen center)  ||                                  (H)
    //      Horizont  --->||----------------------------------.|<-(player Height and also CAMERA HEIHGT)
    //                    ||                      .            |
    //                    ||(PJ)       .                       |
    //(point P            ||._________________________________.|(PJ')
    //on floor)   .       ||                                   | <--------Player feets
    // -.-----------------||------------------------------------(F) <-floor
    //(P)                 ||------player distance to screen-----
    //                    ||
    //                    ||
    //                    ||
    //                    ||
    //  --------straight distance from player to point P--------
    //
    //
    //===============================================================================================================================
    //HOW TO MAP FLOORS TO TEXTURES
    //
    //                      Lets assume the player angle (b) is 90 degree here so its easier to see the math
    //                      We know (C) which is the casted ray angle (ray direction)
    //                      Then we can calculate (a) by solving for it
    //                      now its trivial to find the real distance since we know the straight distance value and the angle (a)
    //                      using trig we can see that:
    //
    //                      cos(a) = straigthDistance / realDistance and then we solve for actual Distance                                          actual    L ... middle ...   R
    //                      NOTE: since floor is perfectly plane this 'real distance' repeats from the middle in the same screen row, imagine->    distance = 10 8 6   4    6 8 10   <-like so (this may be helpful for optimization i need to think about it, stack?)
    // (Point on
    // the floor).P       ^
    //            \-------|
    //             \      | <-- straight distance to point P
    //    real ---> \     |
    //  distance    ========================= screen(Projection Plane)
    //                \   |
    //                 \  |  C=a+b
    //                  \a|
    //                   \| b
    //                 Player--------------
    //
    // Once we got point P this is the point in the mapped texture wee need to draw!

    Floor() : straightDistance(0.0), realDistance(0.0f),
    textureSize(128) {textureSizeHalf=textureSize/2;}

    void draw(float, float, float, float, float, float, float, SDL_Surface**, int, int, void*, int, float);

    float straightDistance;
    float realDistance;
    uint16_t textureSize;
    uint16_t textureSizeHalf;
};

#endif /* floor_hpp */
