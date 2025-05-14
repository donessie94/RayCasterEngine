//
//  ray.hpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#ifndef ray_hpp
#define ray_hpp

#include "settings.h"

class Ray
{
public:
    Ray():  direction(0.0f), deltaRay(0.0f), verticalDistanceToWall(0.0f),          //initiazling
            horizontalDistanceToWall(0.0f), wallHitposX(0.0f), wallHitposY(0.0f),   //all
            cosAngle(0.0f), sinAngle(0.0f), wallRowPos(0), wallColPos(0) {}                                       //variables
    
    void verticalWallCheck(float playerPosX, float playerPosY, float sinA, float cosA);
    void horizontalWallCheck(float playerPosX, float playerPosY, float sinA, float cosA);
    
    float direction;
    float deltaRay;
    float verticalDistanceToWall;
    float horizontalDistanceToWall;
    float wallHitposX;
    float wallHitposY;
    float cosAngle;
    float sinAngle;
    int wallRowPos;
    int wallColPos;
};

#endif /* ray_hpp */
