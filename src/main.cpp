#include "Engine.h"
#include "Map.h"
#include "Player.h"
#include<iostream>
int main(){
    Engine engine;
    Player player;
    Map map(true,true);
    if(!engine.init()){return 1;}
    while(engine.isRunning()){
               engine.handleInput(player,map);
        engine.update(map);
        engine.render(map,player);
    }
return 0;
}
