#include "Engine.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include<iostream>

Engine::Engine(): window(nullptr),renderer(nullptr),_running(false){

}

bool Engine::init(){
    if(SDL_Init(SDL_INIT_VIDEO)==0){
        //do stuff
        window=SDL_CreateWindow("Raycaster",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);//the last flag  makes sure we see it instantly
        if(window==nullptr){
            std::cout<<"Failed to create window"<<SDL_GetError()<<std::endl;
            return false;
        }
        renderer=SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);//-1 means "just initialize the first one that supports the requested flags."
        if(renderer==nullptr){
            std::cout<<"Failed to create renderer"<<SDL_GetError()<<std::endl;
            return false;
        }
        _running=true;
        return true;

    }
    else{
        cout<<"Failed to initialise SDL"<<endl;
        return false;
    }
}

void Engine::stop(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Engine::isRunning(){
    return _running;
}

void Engine::handleInput(Player &player,const Map &map){
    SDL_Event e;
    while(SDL_PollEvent(&e)!=0){
        if(e.type==SDL_QUIT){
            //quit
        _running=false;
        }

    }
   const Uint8* currentKeyState=SDL_GetKeyboardState(NULL);
    if(currentKeyState[SDL_SCANCODE_W])player.moveForward(map);
    if(currentKeyState[SDL_SCANCODE_S])player.moveBackward(map);
    if(currentKeyState[SDL_SCANCODE_A])player.moveLeft(map);
    if(currentKeyState[SDL_SCANCODE_D])player.moveRight(map);
    if(currentKeyState[SDL_SCANCODE_LEFT])player.turn(player.turn_speed);
    if(currentKeyState[SDL_SCANCODE_RIGHT])player.turn(-player.turn_speed);


}

