//
//  player.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#ifndef player_hpp
#define player_hpp

#include "../engine/settings.h"

class Player
{
public:
    Player();

    void moveLeft(float fElapsedTime);    //changes player angle
    void moveRight(float fElapsedTime);   //changes player angle
    void moveForward(float fElapsedTime); //move towards vector direction
    void moveBackward(float fElapsedTime);//move backwards on the same "vector direction line" tho
    void rotateLeft(float fElapsedTime);  //increment player angle
    void rotateRight(float fElapsedTime); //decrement player angle
    void lookUp(float);
    void lookDown(float);
    void crouch();
    void fly();
    bool collisionDetection(float x2, float y2, float direction, float fElapsedTime);//

    bool flyState;
    bool crouchState;
    float posX; //player X position
    float posY; //player Y position
    float angle;//vector direction basically
    float FOV;  //Field of View
    float halfFOV;
    float screenDistance;
    float horizont;    //to achieve the effect of looking up or down we change the horizont position up or down
                       //this is a trick and its not perspectively correct btw
    int pitch;

    //this stats are for the player hitBox purposes
    uint8_t width;  //shoulder to shoulder
    uint8_t height; //how tall
    uint8_t depth;  //how fat? LOL
};

#endif /* player_hpp */
