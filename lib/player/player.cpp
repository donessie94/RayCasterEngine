//
//  player.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#include "player.h"

//construct with parameters (in this case the "engine renderer" to create the player texture)
//it also initializate all the variables in Player class
Player::Player() : posX{100.0}, posY{100.0}, angle{5.0}, width{8}, height{32},
depth{8}, FOV{PI/3}
{
    halfFOV=FOV/2;
    screenDistance=(screenWidth/2)/std::tan(halfFOV);   //comes from trigonometry since tan(halfFOV)=halfscreenwidth/screenDistance
    horizont=screenHeight/2;
    crouchState=0; flyState=0; pitch=0;
}

bool Player::collisionDetection(float x2, float y2, float direction, float fElapsedTime)//x2 is new 'x' player position, y2 is new 'y' player position
{
    //lets cast a ray (the player direction ray) to the nearest wall to find how close we are

    for(int i=0;i<1;i++) //CHECK MY MATH HERE AND IN CHECHING WALLS I AM SURE THERE IS AN INVERTED SIN/COS SOMEWHERE
    {
        uint16_t row = y2/blockSize;
        uint16_t col = x2/blockSize;
        if(WorldMap[row][col]>0)
            return true;

        x2 = x2 + std::cos((direction)) * fElapsedTime * 15;
        y2 = y2 - std::sin((direction)) * fElapsedTime * 15;
    }

    return false;
}

void Player::crouch()
{
    /*
    if(!crouchState)
    {
        height=5;
        horizont-=50;
    }
    else
    {
        height=32;
        horizont+=50;
    }


    crouchState=!crouchState;
     */

    if(height>0)
    {
        height-=1;
        pitch-=4;
    }


}

void Player::fly()
{
    if(height<64)
    {
        height+=1;
        pitch+=4;
    }

}

void Player::lookUp(float fElapsedTime)
{
    if(horizont<screenHeight-5)
        horizont+=800*fElapsedTime;
}

void Player::lookDown(float fElapsedTime)
{
    if(horizont>5)
        horizont-=800*fElapsedTime;
}

void Player::rotateLeft(float fElapsedTime)
{
    angle+=2.5 * fElapsedTime;
    if (angle < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        angle += twoPI;
    if (angle > twoPI)                         //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        angle -= twoPI;
}

void Player::rotateRight(float fElapsedTime)
{
    angle-=2.5 * fElapsedTime;
    if (angle < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        angle += twoPI;
    if (angle > twoPI)                         //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        angle -= twoPI;
}

void Player::moveLeft(float fElapsedTime)
{
    float direction=angle+piOver2+0.001;    //direction angle
    if (direction < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        direction += twoPI;
    if (direction > twoPI)                          //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        direction -= twoPI;

    float x2 = posX + std::cos((direction)) * fElapsedTime * 60;//+90 here cuz a left move is basically moving forward to a perpendicular
    float y2 = posY - std::sin((direction)) * fElapsedTime * 60;//angle 90 greater than your currently player angle, EXAMPLE:
                                              //       ^  +90 grade forward from P(player) is moving to the left see it?
                                              //       |
                                              // ------P----> 0 grade

    if(!collisionDetection(x2, y2, direction, fElapsedTime)) //if no wall collision detected
    {
        posX=x2; posY=y2;
    }

}

void Player::moveRight(float fElapsedTime)
{
    float direction=angle-piOver2+0.001;
    if (direction < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        direction += twoPI;
    if (direction > twoPI)                          //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        direction -= twoPI;


    float x2 = posX + std::cos((direction)) * fElapsedTime * 60;//-90 here cuz a right move is basically moving forward to a perpendicular
    float y2 = posY - std::sin((direction)) * fElapsedTime * 60;//angle 90 less than your currently player angle

    if(!collisionDetection(x2, y2, direction, fElapsedTime)) //if no wall collision detected
    {
        posX=x2; posY=y2;
    }
}

void Player::moveForward(float fElapsedTime)
{
    float x2 = posX + std::cos((angle+0.001)) * fElapsedTime * 120;// * time;
    float y2 = posY - std::sin((angle+0.001)) * fElapsedTime * 120;// * time;

    if(!collisionDetection(x2, y2, angle, fElapsedTime)) //if no wall collision detected
    {
        posX=x2; posY=y2;
    }
}
void Player::moveBackward(float fElapsedTime)
{
    float direction=angle-PI+0.001; //direction angle
    if (direction < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        direction += twoPI;
    if (direction > twoPI)                          //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        direction -= twoPI;

    float x2 = posX + std::cos((direction)) * fElapsedTime * 120;// -PI here cuz we moving backwards
    float y2 = posY - std::sin((direction)) * fElapsedTime * 120;//

    if(!collisionDetection(x2, y2, direction, fElapsedTime)) //if no wall collision detected
    {
        posX=x2; posY=y2;
    }
}
