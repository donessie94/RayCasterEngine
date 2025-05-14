//
//  rayCaster.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/12/24.
//

#include "rayCaster.h"
#include <SDL2/SDL_image.h>

//engine construct to createb the window and the renderer of ur engine
RayCaster::RayCaster()
{
    //initialization of our "window"
    SDL_Init(SDL_INIT_VIDEO);
    window=SDL_CreateWindow("Ray Caster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED ,
                                        screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    screen=SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    SDL_RenderSetVSync(renderer, true);

    fElapsedTime=0.0f; //cheat for now CHECK THIS!!!!
    running=true;
    pitch=0;
    pixel=NULL;
    rMask=0; gMask=0; bMask=0; counterFrames=0;

    keyState=SDL_GetKeyboardState(NULL);//may not need it here CHECK LATER!

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, halfScreenWidth, halfScreenHeight);

    SDL_GetMouseState(&mouseX, &mouseY);//capture the mouse so we can compare it value to the next movement of mouse end coordinates
                                        //that way we can rotate in that direction

    player=new Player;    //initializing our data structures
    ray=new Ray;
    miniMap=new MiniMap(renderer, player->posX, player->posY);
    wall=new Wall;
    floor=new Floor;
    ceiling=new Ceiling;

    ray->deltaRay = player->FOV / screenWidth;
    ray->direction = player->angle - player->halfFOV + 0.001;

    playerSpritePosition.h=600;
    playerSpritePosition.w=200;
    playerSpritePosition.x=screenWidth/4;
    playerSpritePosition.y=screenHeight/2;

    playerFrame.h=89;
    playerFrame.w=39.5;
    playerFrame.x=0;
    playerFrame.y=0;

    /*
    const char *ceiling_path ="../../../textures/skyBackground.png";
    background = IMG_Load(ceiling_path);

    if (!background) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
        std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
        return;
    }

    halfScreen.h=halfScreenHeight;
    halfScreen.w=screenWidth;
    halfScreen.x=0; halfScreen.y=0;*/
}

//engine function to draw a background (currently just black background)
void RayCaster::drawBackground()
{


}

//loop to catch events and process them such as key presses etc..
void RayCaster::eventLoop()
{
    keyState=SDL_GetKeyboardState(NULL);
    //SDL_PumpEvents();
    int newMouseX=0, newMouseY=0;
    bool mouseMoved=false;
    SDL_WarpMouseInWindow(window, halfScreenWidth, halfScreenHeight);

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {

        if(event.type==SDL_QUIT || keyState[SDL_SCANCODE_ESCAPE])
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            running=false;
        }
        else if(event.type==SDL_MOUSEMOTION)
        {
            SDL_GetMouseState(&newMouseX, &newMouseY);
            mouseMoved=true;
        }
        else if (event.type==SDL_KEYDOWN)  //if M is pressed show minimap if press again hidde it minimap default is hiding tho
        {
            if(event.key.keysym.scancode == SDL_SCANCODE_M)
                miniMap->toggle();
            if(event.key.keysym.scancode == SDL_SCANCODE_C)
                player->crouch();
            if(event.key.keysym.scancode == SDL_SCANCODE_F)
                player->fly();
        }
    }

    //this code handles the movement in a smooth way using the keystates of our keyboard (if the pieces are pressed or released)
    if(keyState[SDL_SCANCODE_W]==SDL_PRESSED)
    {
        player->moveForward(fElapsedTime);

        /*
        counterFrames++;
        if(counterFrames==8)
        {
            if(playerFrame.x==273)
                playerFrame.x=0;
            else
                playerFrame.x+=39;
            counterFrames=0;
        }


    }
    else
    {
        counterFrames=0;
        playerFrame.x=0;
    */}


    if(keyState[SDL_SCANCODE_S]==SDL_PRESSED)
    {
        player->moveBackward(fElapsedTime);
    }

    if(keyState[SDL_SCANCODE_A]==SDL_PRESSED)
    {
        player->moveLeft(fElapsedTime);
    }

    if(keyState[SDL_SCANCODE_D]==SDL_PRESSED)
    {
        player->moveRight(fElapsedTime);
    }

    if(keyState[SDL_SCANCODE_UP]==SDL_PRESSED)
    {
        player->lookUp(fElapsedTime);
    }

    if(keyState[SDL_SCANCODE_DOWN]==SDL_PRESSED)
    {
        player->lookDown(fElapsedTime);
    }

    if(keyState[SDL_SCANCODE_LEFT]==SDL_PRESSED)
    {
        player->rotateLeft(fElapsedTime);
        rayDirectionSetUp();
    }

    if(keyState[SDL_SCANCODE_RIGHT]==SDL_PRESSED)
    {
        player->rotateRight(fElapsedTime);
        rayDirectionSetUp();
    }



    /*
    if(mouseMoved)
    {
        if(mouseX>newMouseX)
        {
            player->rotateLeft(fElapsedTime);
            rayDirectionSetUp();
        }

        else
        {
            player->rotateRight(fElapsedTime);
            rayDirectionSetUp();
        }

        //moving with mouse super hard without an actual mouse so comented out for now
        //if(mouseY>newMouseY)                    //looking up and down trick, its achievied by
        //    player->lookUp(fElapsedTime);   //moving the projection plane UP or Down respectively
        //else
        //    player->lookDown(fElapsedTime);
    }*/
}

void RayCaster::rayDirectionSetUp() //set ups the ray direction of our initial ray, the +0.0001 s to avoid division by 0
{
    ray->direction = player->angle - player->halfFOV + 0.001; //ray.direction holds the starting angle for casting ray
    if (ray->direction < 0.0f)                           //wrap around whn values are negative we just substract 360+(-angle) to get the "wrap around" value
        ray->direction += twoPI;
    if (ray->direction > twoPI)                          //else we wrap around for values bigger than 360, we want to make the values stay in 0-360 range
        ray->direction -= twoPI;
}

void RayCaster::drawScreen()
{
    drawBackground();

    for(int r=0; r<screenWidth; r++)
    {
        /*
        if(cosA[ray->direction]==0)
            cosA[ray->direction]=std::cos(ray->direction);
        if(sinA[ray->direction]==0)
            sinA[ray->direction]=std::sin(ray->direction);

        ray->cosAngle=cosA[ray->direction];
        ray->sinAngle=sinA[ray->direction];*/
        ray->cosAngle=std::cos(ray->direction);
        ray->sinAngle=std::sin(ray->direction);

        ray->verticalWallCheck(player->posX, player->posY,  //find and store distance to closest vertical wall and the interception point
                               ray->sinAngle, ray->cosAngle);
        ray->horizontalWallCheck(player->posX, player->posY, //find and store distance to closest horizontal wall and the interception point
                                 ray->sinAngle, ray->cosAngle);//also this last one compares so we can get a final (smaller) interception point

        float depth=std::min(ray->horizontalDistanceToWall, ray->verticalDistanceToWall);

        //fixing the fishbowl effect
        depth*=std::cos(player->angle - ray->direction);   //cos(angle with respect to player direction) = correct D / distorted D (what we got above)
                                                           //we want the straight dept to the wall from player position to be the depth if that makes sense

        //NOTE FOR LATER!!
        //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   //i need to find a way to show this rays on the minimap
        //SDL_RenderDrawLineF(renderer, player->posX, player->posY, ray->wallHitposX, ray->wallHitposY);

        bool hitDirection=1;    //assume vertical hit
        if(ray->horizontalDistanceToWall < ray->verticalDistanceToWall)
            hitDirection=0;     //horizontal hit detected so change to it

        int wallType=WorldMap[ray->wallRowPos][ray->wallColPos]-1;

        //*
        wall->draw(r, depth, player->screenDistance, renderer, screen,
                   pitch, ray->wallHitposX, ray->wallHitposY, pixel, player->horizont,
                   hitDirection, wallImage[wallType], player->pitch);

        float angle=std::abs(player->angle-ray->direction);


        floor->draw(wall->maxHeight+1, player->screenDistance, angle,
                    ray->sinAngle, ray->cosAngle, player->posX, player->posY, floorTexture,
                    r, pitch, pixel, player->height, player->horizont);

        ceiling->draw(wall->minHeight-1, player->screenDistance, angle,
                    ray->sinAngle, ray->cosAngle, player->posX, player->posY, ceilingTexture,
                    r, pitch, pixel, player->height, player->horizont, player->angle);
         //*/

        ray->direction+=ray->deltaRay;
    }
}

void RayCaster::castRays()
{

    rayDirectionSetUp();    //raydirection = newplayerangle-halfFOV+0.0001 and wrapup if needed
                            //it basically setup the intial ray to be casted angle (a.k.a direction)

    SDL_LockTexture(screen, NULL, (void**) &pixel, &pitch);

    loadImages();

    drawScreen();           //calculate ray closes distance to wall, and (x,y) coordinates of intersection
                            //then draws the screen

    SDL_UnlockTexture(screen);
    SDL_RenderCopy(renderer, screen, NULL, NULL);


    //drawing player on screen now
    //SDL_RenderCopy(renderer, playerSprite, &playerFrame, &playerSpritePosition);
}

void RayCaster::loadImages()
{
    if(wallImage[0]==NULL)
    {
        const char *image_path ="../../textures/default_acacia_wood.png";
        wallImage[0] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[0]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    if(wallImage[1]==NULL)
    {
        const char *image_path ="../../textures/bricksx64.png";
        wallImage[1] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[1]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    if(wallImage[2]==NULL)
    {
        const char *image_path ="../../textures/default_aspen_wood.png";
        wallImage[2] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[2]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    if(wallImage[3]==NULL)
    {
        const char *image_path ="../../textures/default_furnace_front.png";
        wallImage[3] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[3]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    if(wallImage[4]==NULL)
    {
        const char *image_path ="../../textures/default_bookshelf.png";
        wallImage[4] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[4]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    if(wallImage[5]==NULL)
    {
        const char *image_path ="../../textures/doors_door_wood.png";
        wallImage[5] = IMG_Load(image_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!wallImage[5]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
        //std::cout<<"bytes per pixel is: "<<wallImage->format->BitsPerPixel<<'\n';
    }

    //*
    if(floorTexture[0]==NULL)
    {
        const char *floorTexture_path ="../../textures/default_stone_block.png";
        floorTexture[0] = IMG_Load(floorTexture_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!floorTexture[0]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(floorTexture[1]==NULL)
    {
        const char *floorTexture_path ="../../textures/default_rainforest_litter.png";
        floorTexture[1] = IMG_Load(floorTexture_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!floorTexture[1]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(floorTexture[2]==NULL)
    {
        const char *floorTexture_path ="../../textures/default_river_water.png";
        floorTexture[2] = IMG_Load(floorTexture_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!floorTexture[2]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(floorTexture[3]==NULL)
    {
        const char *floorTexture_path ="../../textures/default_rainforest_litter.png";
        floorTexture[3] = IMG_Load(floorTexture_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!floorTexture[3]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    //*
    if(ceilingTexture[0]==NULL)
    {
        const char *ceiling_path ="../../textures/default_acacia_wood.png";
        ceilingTexture[0] = IMG_Load(ceiling_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!ceilingTexture[0]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(ceilingTexture[1]==NULL)
    {
        const char *ceiling_path ="../../textures/default_aspen_leaves.png";
        ceilingTexture[1] = IMG_Load(ceiling_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!ceilingTexture[1]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(ceilingTexture[2]==NULL)
    {
        const char *ceiling_path ="../../textures/skyBackground.png";
        ceilingTexture[2] = IMG_Load(ceiling_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!ceilingTexture[2]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(ceilingTexture[3]==NULL)
    {
        const char *ceiling_path ="../../textures/skyBackground.png";
        ceilingTexture[3] = IMG_Load(ceiling_path);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!ceilingTexture[3]) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }
    }

    if(spritePlayer==NULL)
    {
        const char *playerImg ="../../textures/basic female charcter template.png";
        spritePlayer = IMG_Load(playerImg);

        //NOTE WE NEED TO FIGURE OUT HOW TO MAKE I SO IT CAN READ PICTURES OTHER THANIN THE "PWD"
        //Let the user know if the file failed to load
        if (!spritePlayer) { //ASLO NOTICE WE ONLY CAN WORK IF THE TEXTURE IS LOADED AFTER THE LOCKTEXTURE COMMAND ELSE WE GOT AN ERROR
            std::cout<<"PLOP"<<'\n'<<SDL_GetError()<<'\n';
            return;
        }

        playerSprite=SDL_CreateTextureFromSurface(renderer, spritePlayer);
    }
}
