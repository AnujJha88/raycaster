#include "Engine.h"
#include "Map.h"
#include "Player.h"
#include<iostream>
int main(){
    Engine engine;
    Player player;
    Map map(false);
    if(!engine.init()){return 1;}
    while(engine.isRunning()){
        std::cout << "Pos: " << player.posX << ", " << player.posY
          << " | Dir: " << player.dirX << ", " << player.dirY << std::endl;
        engine.handleInput(player,map);

        engine.render(map,player);
    }
return 0;
}
