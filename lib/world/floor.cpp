//
//  floor.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/13/24.
//

#include "floor.h"

void Floor::draw(float wallMaxHeight, float playerScreenDistance, float angle,
                 float raySin, float rayCos, float playerX, float playerY, SDL_Surface** floorTexture,
                 int rayNum, int pitch, void* pixel, int playerHeight, float horizont)
{

    if(wallMaxHeight>=screenHeight)
        wallMaxHeight=screenHeight;

    rayNum=screenWidth-rayNum;//

    int difference=screenHeight-wallMaxHeight;

    float floorX=0, floorY=0;
    //int floorPositionValue=0;
    int xTextureOffset=0, yTextureOffset=0;
    for(int i=0; i<difference; i++)
    {
        // Create distance and calculate it
        //straightDistance=(playerHeight*playerScreenDistance) / (wallMaxHeight+i-halfScreenHeight+0.001);
        straightDistance=(playerHeight*playerScreenDistance) / (wallMaxHeight+i-horizont+0.001);
        realDistance=straightDistance/std::cos(angle);

        float fadingRatio=(1 + std::pow(realDistance/4, 2) * 0.00004);

                     //here moving realdistance units along rayDirection
                    //      |      |            |
        floorX = playerX + (rayCos * realDistance); //we move "realDistance" units along the rayDirection vector and then
        floorY = playerY - (raySin * realDistance); //then we add those coordinates to our current player (x,y) position
                                                    // to find the (x,y) coordinates of that specific point in our world

        //if we want different textures LOOK AFTER FOR ADDING MORE STUFF
        int floorType = std::abs(WorldMap[(int)(floorY/blockSize)] [(int)(floorX/blockSize)]);

        //MIO
        int xOffsetFloor = (int)(floorX) % blockSize;//finding the x and y coordinates offsets of that floor square
        int yOffsetFloor = (int)(floorY) % blockSize;//is trivial now since we have the (x, y) coordinates
        xTextureOffset=xOffsetFloor*floorTexture[floorType]->w/blockSize;
        yTextureOffset=yOffsetFloor*floorTexture[floorType]->h/blockSize;

        uint8_t r=((uint8_t*)floorTexture[floorType]->pixels)[yTextureOffset*floorTexture[floorType]->pitch+xTextureOffset*4+0];
        uint8_t g=((uint8_t*)floorTexture[floorType]->pixels)[yTextureOffset*floorTexture[floorType]->pitch+xTextureOffset*4+1];
        uint8_t b=((uint8_t*)floorTexture[floorType]->pixels)[yTextureOffset*floorTexture[floorType]->pitch+xTextureOffset*4+2];
        //uint8_t a=((uint8_t*)floorTexture->pixels)[yTextureOffset*floorTexture->pitch+xTextureOffset*4+3];

        r=r / fadingRatio; //adding fading effect depending on distance
        g=g / fadingRatio;
        b=b / fadingRatio;

        ((uint8_t*)pixel) [((int)wallMaxHeight+i)*pitch+rayNum*4+0]=b;
        ((uint8_t*)pixel) [((int)wallMaxHeight+i)*pitch+rayNum*4+1]=g;
        ((uint8_t*)pixel) [((int)wallMaxHeight+i)*pitch+rayNum*4+2]=r;
        //((uint8_t*)pixel) [((int)wallMaxHeight+i)*pitch+rayNum*4+3]=a;
    }
}
