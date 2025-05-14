//
//  walls.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#include "walls.h"

Wall::Wall()
{
    textureSize=128;//1024;16;
    textureSizeHalf=textureSize/2;
    projection=0.0f;
    difference=0;
    minHeight=0.0f;
    maxHeight=0.0f;
}

void Wall::draw(int rayNum, float depth, float screenDistance, SDL_Renderer* renderer,
                SDL_Texture* screen, int pitch, int rayX, int rayY, void* pixel, float horizont,
                bool hitDirection, SDL_Surface* wallImage, int playerPitch)
{
    projection=(blockSize*screenDistance)/(depth+0.0001);

    float fadingRatio=(1 + std::pow(depth/4, 2) * 0.00004);

    //textureSize=wallImage->w;

    //float wallScale=1;
    maxHeight=std::floor(horizont+projection/2);  //from the center of the screen we need to draw our walls so we do half up and half down duh
    minHeight=std::floor(horizont-projection/2);

    //maxHeight+=playerPitch;
    //minHeight+=playerPitch;

    rayNum=screenWidth-rayNum;  //honestly i dont know why the minimap is alwasy inverted but inverting the rays solve this

    //SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    //SDL_RenderDrawLine(renderer, rayNum, minHeight, rayNum, maxHeight);

    difference=maxHeight-minHeight;
    //std::cout<<"difference is: "<<difference<<'\n';

    //TEXTURE MAPPING INFO:
    // A basic relation/proportion is: for horizontal hit:
    //
    //    rayX(offset)    (texture_X_Offset)   <-----our unknown so we can map the texture,
    //   -------------- = ---------------            we need to solve for it, (texture array
    //    blockSize        textureWdith              column basically in this case)
    //
    // NOTE WE ALSO NEED TO FIND THE PROPORTION IN THE Y AXIS USING THE SAME REASONING
    //    currentPixelDrawn/projectedWallHeight = unknwon(our wanted roe value)/textureHeight
    //
    //
    // tricky but note that on vertical hits y is the offset width of texture(the array col value) and it invertes on horizontal hits (x becomes the ofset width of texture(the array col value))
    // therefor one of the things we need to know is if we had a vertical or a horizontal hit (by checking which depth is smaller)
    // our hitDirection varible will give us this info: 0 -> horizontal and 1 -> vertical
    // btw we can calculate the rayX(offset) easily using rayX(offset) = rayX % blockSize duh!
    // rayX: is just an abreviation of x coordinate of the ray once intersect the wall
    //
    //=================================================================================================
    //
    //IMPORTANT PIXEL DATA INFORMATION!!!!
    //
    //           columns:1 2 3 4 5 6 7 8 9 10 11 12
    //EXAMPLE:           r g b a r g b a r  g  b  a  row 1
    //                   r g b a r g b a r  g  b  a  row 2
    //
    //here we have 2 rows (since array "grow" in width but not in height)
    //so pitch(lenght of one row in bytes) is: 12
    //pixel width is: 3
    //pixel height is: 2
    //number of bytes per pixel is: 4
    //and thast why we multiply by 4 our 'x' coordinate but not our 'y' one
    //also note that will give us the adress of the pixel (the first byte of the pixel in fact) and
    //thast why we also do +0 (first one), +1 (second byte in same pixel) and +3 etc..
    //
    //btw in this format the pixel consist of 4 bytes of information BGRA (idk why blue first people say this format is used someitmes)
    //
    //HYPOTETYCAL EXAMPLE CODE FOR REFERENCE: (since i tend to forget stuff lol), (btw here 'x' and 'y' are the screen pixel coordinates)
    //((uint8_t*)pixel) [(y)*pitch+x*4+0]=bMask; //we need to cast it to work to int since remember we declared this point
    //((uint8_t*)pixel) [(y)*pitch+x*4+1]=gMask; //as void in the begining so it can work with SDL_lockTexture() fucntion (it is hard requirement)
    //((uint8_t*)pixel) [(y)*pitch+x*4+2]=rMask; //btw the formula used here is the one for mapping 1D to 2D array (i love school)

    //
    //int textureID=0;        //we can play with this values if we have more textures stored in our array
    uint16_t xTextureOffset=0, yTextureOffset=0; //the offset represent the row and col of our texture array we need to draw
    if(hitDirection) //if hit direction is true (aka 1) then we got a vertical hit
    {
        xTextureOffset=(((rayY%blockSize)*wallImage->w)/blockSize);

        if(difference<=10000) //5000 is arbitrary it could have been 1000 or wherever a number bigger than screenHeight and then optimize for performance
        {
            for(int i=0;i<difference;i++)
            {
                yTextureOffset=((i*wallImage->h)/(int)projection);
                //y = minHeight and increments the value of 'difference' times
                //x is the ray number
                if(minHeight+i>=0 && minHeight+i<=screenHeight)
                {
                    //NOTE IF PIC HAS AN ALPHA CHANNEL instead of multiplying by 3 we multiply by 4 (since it will be 4 bytes per pixel)
                    uint8_t r=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+0];
                    uint8_t g=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+1];
                    uint8_t b=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+2];
                    //uint8_t a=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+3];

                    r=r / (fadingRatio + 0); //adding fading effect depending on distance
                    g=g / (fadingRatio + 0);
                    b=b / (fadingRatio + 0);

                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+0]=b;
                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+1]=g;
                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+2]=r;
                    //alpha chanel
                    //((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+3]=a;
                }//*/
            }
        }
    }
    else            //else we got a horizontal hit
    {
        xTextureOffset=(((rayX%blockSize)*wallImage->w)/blockSize);//texture size because we using a NxN texture so width=height

        if(difference<=10000)
        {
            for(int i=0;i<difference;i++)
            {
                yTextureOffset=((i*wallImage->h)/(int)projection);

                if(minHeight+i>=0 && minHeight+i<=screenHeight)
                {
                    uint8_t r=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+0];
                    uint8_t g=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+1];
                    uint8_t b=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+2];

                    r=r / fadingRatio; //adding fading effect depending on distance
                    g=g / fadingRatio;
                    b=b / fadingRatio;
                    //uint8_t a=((uint8_t*)wallImage->pixels)[yTextureOffset*wallImage->pitch+xTextureOffset*4+3];

                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+0]=b;
                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+1]=g;
                    ((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+2]=r;
                    //alpha chanel
                    //((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+3]=a;

                    //((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+0]=(((uint8_t*)wallImage->pixels)[((i*textureSize)/(int)projection)*wallImage->pitch+(((rayX%blockSize)*textureSize)/blockSize)*4+2])/(1 + (depth/4)*(depth/4) * 0.00004);
                    //((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+1]=(((uint8_t*)wallImage->pixels)[((i*textureSize)/(int)projection)*wallImage->pitch+(((rayX%blockSize)*textureSize)/blockSize)*4+1])/(1 + (depth/4)*(depth/4) * 0.00004);
                    //((uint8_t*)pixel) [((int)minHeight+i)*pitch+rayNum*4+2]=(((uint8_t*)wallImage->pixels)[((i*textureSize)/(int)projection)*wallImage->pitch+(((rayX%blockSize)*textureSize)/blockSize)*4+0])/(1 + (depth/4)*(depth/4) * 0.00004);
                }//*/
            }
        }
    }
}
