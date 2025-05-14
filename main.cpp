//
//  main.cpp
//  Doom Clone
//
//  Created by Ernesto Dones Sierra on 3/7/24.
//

//#include"RayCasterEngine.hpp"
#include<chrono>
#include "lib/engine/rayCaster.h"

int main(int argc, const char * argv[]) {

    RayCaster* engine = new RayCaster;

    auto time1=std::chrono::system_clock::now();
    auto time2=std::chrono::system_clock::now();    //we use this time for smoother movement and FPS counter to check performance

    while(engine->running)
    {

        time2=std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = time2 - time1;
        time1=time2;
        float fElapsedTime = elapsedTime.count();   //calculating the elapsded time so we can multiply for smoother movement
        std::cout<<"FPS: "<<1.0f/fElapsedTime<<'\n'; //FPS counter
        engine->fElapsedTime=fElapsedTime;

        engine->eventLoop();
        engine->drawBackground();
        engine->castRays(); //cast the rays and draw the screen also

        if(engine->miniMap->show)
            engine->miniMap->draw(engine->renderer, engine->player->posX, engine->player->posY); //draws the minimap

        SDL_RenderPresent(engine->renderer);

        //idk why clearinf the render (when is set to screen texture) to black does not work so...
        //CHECH!
        SDL_DestroyTexture(engine->screen);
        engine->screen=SDL_CreateTexture(engine->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    }

    //system("pwd");
    return 0;
}
