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
        std::cout<<"Failed to initialise SDL"<<std::endl;
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

void Engine::handleInput(Player &player, Map &map){
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
    if(currentKeyState[SDL_SCANCODE_LEFT])player.turn(-player.turn_speed);
    if(currentKeyState[SDL_SCANCODE_RIGHT])player.turn(player.turn_speed);
    if(currentKeyState[SDL_SCANCODE_SPACE])player.Interact(map);


}

void Engine::render(  Map& map,const Player& player){
    SDL_SetRenderDrawColor(renderer,30,30,30,255);
    SDL_Rect ceilRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT/2};
    SDL_RenderFillRect(renderer,&ceilRect);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Light Gray
    SDL_Rect floorRect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);

    for(int x=0;x<SCREEN_WIDTH;x++){
       double cameraX= -1+double(2*x)/(double(SCREEN_WIDTH));
        double rayX= player.dirX+player.planeX*cameraX;
        double rayY= player.dirY+player.planeY*cameraX;

        int mapX=int(player.posX);
        int mapY=int(player.posY);

        double deltaDistX=std::abs(1/rayX);
        double deltaDistY=std::abs(1/rayY);

        int stepX=rayX>0?1:-1;
        int stepY=rayY>0?1:-1;

        double sideDistX=(rayX<0?player.posX-mapX:-player.posX+mapX+1.0)*deltaDistX;
        double sideDistY=(rayY<0?player.posY-mapY:-player.posY+mapY+1.0)*deltaDistY;

        int hit=0;
        int side=0;
        while(hit==0){
           if(sideDistX<sideDistY){
                sideDistX+=deltaDistX;
                mapX+=stepX;
                side=0;
            }

            else{
                sideDistY+=deltaDistY;
                mapY+=stepY;
                side=1;
            }

            int tile=map.getTile(mapX,mapY);
            if(tile==99)
            {
                //door
                Door* door= map.getDoor(mapX,mapY);
                if(!door)hit=tile;
                if(side==0){
                    double Y_hit=player.posY+(sideDistX-deltaDistX)/rayX*rayY;
                    double Y_frac=Y_hit-int(Y_hit);
                    if(Y_frac<door->openAmount){
                        hit=0;
                    }
                }
                if(side==1){
                    double X_hit=player.posX+(sideDistY-deltaDistY)/rayY*rayX;
                    double X_frac=X_hit-int(X_hit);
                    if(X_frac<door->openAmount){
                        hit=0;
                    }
                }


            }
            else if (tile>0){
                hit=tile;
            }

        }

        double perpWallDist=side==0?sideDistX-deltaDistX:sideDistY-deltaDistY;

        int lineHeight= int(SCREEN_HEIGHT/perpWallDist);

        int drawStart=std::max(0,(SCREEN_HEIGHT-lineHeight)/2);
        int drawEnd=std::min(SCREEN_HEIGHT-1,(SCREEN_HEIGHT+lineHeight)/2);

        int tile=map.getTile(mapX,mapY);
        Uint8 r,g,b;
        switch(tile){
            case 1:
                r=255;
                g=0;
                b=0;
                break;
            case 2:
                r=0;
                g=255;
                b=0;
                break;
            case 3:
                r=0;
                g=100;
                b=255;
                break;
            default:
                r=255;
                g=255;
                b=255;
                break;
        }

        if(side==1){
            r/=2;
            g/=2;
            b/=2;
        }
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }


    SDL_RenderPresent(renderer);
}

Engine::~Engine(){
                 Engine::stop();

}

void Engine::update(Map& map){
 static int32_t lastTime=0;
    uint32_t currTime=SDL_GetTicks();
    float deltaTime=(currTime-lastTime)/1000.0f;
    lastTime=currTime;

    for (auto& door: map.getDoors()){
        switch(door.state){
            case DoorState::OPENING:
              door.openAmount+=1.0*deltaTime;
            if(door.openAmount>=1){
                    door.state=DoorState::OPEN;
                    door.openAmount=1;
                    break;
                }
            break;
            case DoorState::OPEN:
                if(door.timer>0){
                    door.timer-=1.0*deltaTime;
                    if(door.timer<0){
                        door.state=DoorState::CLOSING;
                        door.timer=0;
                    }
                }

                break;
            case DoorState::CLOSING:
                door.openAmount-=1.0*deltaTime;
                if(door.openAmount<=0){
                    door.state=DoorState::CLOSED;
                    door.openAmount=0;
                    break;
                }
                break;
            case DoorState::CLOSED:
                break;

        }
    }
}
