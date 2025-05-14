//
//  ray.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#include "ray.h"

//NOTES ON THIS PROCEDURES:
//
//HORIZONTAL WALL CHECK                 VERTICAL WALL CHECK
//        /|                                       /|
// ray-> / |                                ray-> / |
//      /  | Ya=blocksize                        /  | Ya
//     /a__|                                    /a__|
//      Xa     a=ray.angle                    Xa=blocksize   a=ray.angle
//
//Thus: tan(a)=blocksize/Xa and       Thus: tan(a)=Ya/blocksize
//          Xa=blocksize/tan(a)                 Ya=tan(a)*blocksize
//
//iam using another formula here using the sin, cos but its same thing, think about it!

void Ray::verticalWallCheck(float playerPosX, float playerPosY, float sinA, float cosA)
{
    //VERTICAL WALLS CHECK
    //====================                             Y ^90            in unit circle:
    //                                                   |              cos(angle)=x
    //.X(or cos(angle) is negative on this side  180 ----------> X 0    and sin(angle)=y
    //                                                   |
    //                                                  270
    //                              .Y(or sin(angle) is negative down here
    //
    //     Important:
    //     y increments down so we need to tweak our sin cos
    //    (0,0)
    //     .---------------------> x (increases)
    //     |
    //     |
    //     |
    //     |
    //     |
    //     |
    //     y
    //    (increases)

    //    IMPORTANT!!!!!!!!!!!
    //    DAMN it took a while to understand but the angle set up being used looks like this:
    //
    //          270 ^
    //              |
    //       180---------> 0    WHY???? just WHY.... wherever..
    //              |
    //              90
    //

    uint16_t col = playerPosX/blockSize;     //col index in our worldMap where our player curretly is

    float Xa=0.0f,          //'x' coordinate of first grid hit by our casted ray
          dx=0.0f,          //change in 'x' of our first casted ray when hitting the first grid
          hypotenuse2=0.0f; //basically the change in depth of our casted ray after the first grid hit
                            //note this values are different for the first grid hit then because of equivalent triangles
                            //they become a constant (hypothenuse2 and dx)

    bool found=false;                   //becomes true as soon as we hit a wall

    if(cosA>0)
    {
        Xa=(col+1)*blockSize; //Xa represent the X coordinates of the first intercep with an vertical grid
        dx=blockSize;
    }
    else if(cosA<0)
    {
        Xa=(col)*blockSize; //Xa represent the X coordinates of the first intercep with an vertical grid
        dx=(-blockSize);
    }

    if(sinA<0)                      //because we looking down and down is increment in Y axis in SDL
        sinA=std::abs(sinA);
    else                                //because we looking up and up is decrement in Y axis in SDL
        sinA=(-std::abs(sinA)); //NOTE: we need to use 'sinA' insted of 'sinAngle' variable cuz this
                                //step changes the value needed for next (horizontal) checks

    float hypotenuse=(Xa-playerPosX)/cosA;//basically we using a rght triangle so we can use this: cos(a) = (Xa-PosX(initial)) / hypotenuse
                                        //and for next one sin(a) = (Ya - posY(initial)) / hypotenuse and then solve for Ya
    float Ya=playerPosY+hypotenuse*sinA;  //Ya represent the Y coordinates of the first intercep with an vertical wall

    uint16_t wallRow=Ya/blockSize;           //so we can perform a check for wall in the very first interception
    uint16_t wallCol=Xa/blockSize;

    if(cosA<0)
        wallCol--;

    if(wallCol >= 0 && wallCol < 24 && wallRow >= 0 && wallRow < 24)//alwasy securing the range of our array cuz weird cases can crash the program
    {
        if(WorldMap[wallRow][wallCol]>0)
            found=true;
    }

    if(!found)//if not hit on very first wall we need to keep checking btw "dx" is now blocksize
    {
        hypotenuse2=dx/cosA;  //this hypotenuse is the one we can safely keep adding to find more intersection since after the first point
                                    //the hypotenuse length starting from (Xa,Ya) will alwasy be the same
                                    //here again we use trigonometry: cos(a) = blocksize(now our dx) / hypotenuse2 and solve for hypotenuse 2
                                    //next we will use sin(a) = dy / hypotenuse 2 and we will solve for "dy" to find the value we will alwasy add

        if(hypotenuse<1536)
        {
            float dy=hypotenuse2*sinA;  //from now on just like "dx" is alwasy +blocksize "dy" will alwasy be +this value we will get now

            int counter=0;  //a counter to guarantee stop in the case we never found an interception
            while(!found && counter<25)
            {
                if(hypotenuse<1536)
                {
                    hypotenuse+=hypotenuse2;            //our distance to wall from player length
                    Xa+=dx;
                    Ya+=dy;

                    wallRow=Ya/blockSize;
                    wallCol=Xa/blockSize;

                    if(cosA<0)//since when looking to the left we need to check "the next the left square"
                        wallCol--;

                    if(wallCol >= 0 && wallCol < 24 && wallRow >= 0 && wallRow < 24)//alwasy securing the range of our array cuz weird cases can crash the program
                    {
                        if(WorldMap[wallRow][wallCol]>0)
                            found=true;
                    }

                    counter++;
                }
                else
                    break;
            }
        }
    }

    verticalDistanceToWall=hypotenuse;

    wallColPos=wallCol;
    wallRowPos=wallRow;
    wallHitposX=Xa;
    wallHitposY=Ya;
}

void Ray::horizontalWallCheck(float playerPosX, float playerPosY, float sinA, float cosA)
{

    uint16_t row = playerPosY/blockSize;     //row index in our worldMap where our player currently is

    float Ya=0.0f,          //'y' coordinate of first grid hit by our casted ray
          dy=0.0f,          //change in 'y' of our first casted ray when hitting the first grid
          hypotenuse2=0.0f; //basically the change in depth of our casted ray after the first grid hit
                            //note this values are different for the first grid hit then because of equivalent triangles
                            //they become a constant (hypothenuse2 and dy)

    bool found=false;                   //becomes true as soon as we hit a wall

    //Up-Down
    if(sinA<0)//if we are looking down in other words if the angle is between (180 - 360) easy to see in unit circle btw
    {
        Ya=(row+1)*blockSize; //Xa represent the X coordinates of the first intercep with an horizonytal wall
        dy=blockSize;
    }
    else if(sinA>0)//else we are are looking up, btw while looking up we next to check "the next square up" which translate to row-1
    {
        Ya=(row)*blockSize; //Ya represent the Y coordinates of the first intercep with an horizonytal wall
        dy=(-blockSize);
    }

    if(cosA<0)                      //because we looking left and left is decrement in X axis in SDL
        cosA=std::abs(cosA);
    else                                //because we looking right and right is increment in Y axis in SDL
        cosA=-std::abs(cosA);


    float hypotenuse=(Ya-playerPosY)/sinA;//basically we using a rght triangle so we can use this: sin(a) = (Ya-PosY(initial)) / hypotenuse
                                        //and for next one cos(a) = (Xa - posX(initial)) / hypotenuse and then solve for Xa
    float Xa=playerPosX+hypotenuse*cosA;  //Xa represent the X coordinates of the first intercep with an horizonytal wall

    int wallRow=Ya/blockSize;           //so we can perform a check for wall in the very first interception
    int wallCol=Xa/blockSize;

    if(sinA>0)
        wallRow--;

    if(wallCol >= 0 && wallCol < 24 && wallRow >= 0 && wallRow < 24)//alwasy securing the range of our array cuz weird cases can crash the program
    {
        if(WorldMap[wallRow][wallCol]>0)
            found=true;
    }

    if(!found)//if not hit on very first wall we need to keep checking btw "dy" is now blocksize
    {

        hypotenuse2=dy/sinA;  //this hypotenuse is the one we can safely keep adding to find more intersection since after the first point
                                    //the hypotenuse length starting from (Xa,Ya) will alwasy be the same
                                    //here again we use trigonometry: sin(a) = blocksize(now our dy) / hypotenuse2 and solve for hypotenuse 2
                                    //next we will use cos(a) = dx / hypotenuse 2 and we will solve for "dx" to find the value we will alwasy add
        if(std::abs(hypotenuse)<1536)
        {
            float dx=hypotenuse2*cosA;  //from now on just like "dy" is alwasy +blocksize "dx" will alwasy be +this value we will get now

            int counter=0;  //a counter to guarantee stop in the case we never found an interception
            while(!found && counter<25)
            {
                if(std::abs(hypotenuse)<1536)    //why check bigger values if the map is not that big eanyways (1536 will be block of 64 in size since 24*64=1536)
                {
                    hypotenuse+=hypotenuse2;            //our distance to wall from player length
                    Xa+=dx;
                    Ya+=dy;

                    wallRow=Ya/blockSize;
                    wallCol=Xa/blockSize;

                    if(sinA>0)
                        wallRow--;

                    if(wallCol >= 0 && wallCol < 24 && wallRow >= 0 && wallRow < 24)//alwasy securing the range of our array cuz weird cases can crash the program
                    {
                        if(WorldMap[wallRow][wallCol]>0)
                            found=true;
                    }

                    counter++;
                }
                else
                    break;
            }
        }
    }

    horizontalDistanceToWall=std::abs(hypotenuse);

    if(horizontalDistanceToWall < verticalDistanceToWall)
    {
        wallColPos=wallCol;
        wallRowPos=wallRow;
        wallHitposX=Xa;
        wallHitposY=Ya;
    }
}
