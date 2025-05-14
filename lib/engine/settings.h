//
//  settings.h
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#ifndef settings_h
#define settings_h

#include <stdint.h>
#include <cmath>
#include <iostream>

const float PI = 3.1415926535897;
const float twoPI= PI * 2;
const float piOver2= PI / 2;
const uint16_t screenWidth = 1280;
const uint16_t screenHeight = 720;
const uint16_t halfScreenHeight = screenHeight / 2;
const uint16_t halfScreenWidth = screenWidth / 2;
const uint16_t blockSize = 64;  //size of each block in our world 16x16x16 (a cube basically)
                                //note since we using trig and equivalent triangles this only affetcs
                                //the distance to wall values but not how tall they are on the screen
                                //a world of blocksize 1 will look the same but will have shorter distances values
                                //this may be important for color less color in the pixel at 'x' distance etc
const uint16_t mapWidth = 12;//24;
const uint16_t mapHeight = 12;//24;


//worldMap
const int WorldMap[mapHeight][mapWidth]=
{
  {1, 5, 1, 1, 1, 4, 1, 2, 2, 2, 2, 2},
  {5, 0, 0, 3, 0, 0, 1,-3,-3,-3,-3, 2},
  {1, 0, 0, 0, 0, 0, 6,-3,-3,-3,-3, 2},
  {1, 0, 0, 3, 0, 0, 1,-3,-3,-3,-3, 2},
  {1, 5, 0, 3, 0, 0, 1,-3,-3,-3,-3, 2},
  {1, 0, 0, 3, 0, 0, 0,-3,-3,-3,-3, 2},
  {6, 0, 0, 3, 0, 0, 1,-3,-3,-2,-3, 2},
  {1, 0, 0, 0, 0, 0, 1,-3,-2,-2,-2, 2},
  {1, 0, 0, 3, 0, 0, 1,-3,-2,-2,-2, 2},
  {1, 0, 1, 1, 1, 1, 1,-3,-2,-3,-3, 2},
  {2,-1,-1,-1,-1,-1,-1,-3,-3,-3,-3, 2},
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};

#endif /* settings_h */
