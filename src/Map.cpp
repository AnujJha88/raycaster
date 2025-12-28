#include "Map.h"
#include <random>
#include<cstring>
#include<stack>

Map::Map(bool random,bool isMaze){
        if(!random){
        int tempMap[WIDTH][HEIGHT] = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,2,-1,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,0,0,0,5,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };
        memcpy(worldMap,tempMap,sizeof(worldMap));
        for(int x=0;x<WIDTH;x++){
            for(int y=0;y<HEIGHT;y++){
                if(worldMap[x][y]==-1){
                    worldMap[x][y]=99;
                    Door door{0.0,x,y,DoorState::CLOSED,3.0};
                    activeDoors.push_back(door);
                }
            }
        }
    }
    else{
        if(isMaze)generateMaze();
        else generateRandom();
    }
}

void Map::generateRandom(){
    std::random_device rd;//this gives us a non deterministic seed
    std::mt19937 gen(rd());//this is the Mersenne Twister
    std::uniform_int_distribution<> densityDist(0,100);//wall density
    std::uniform_int_distribution<> doorDist(0,100);
    std::uniform_int_distribution<> colorDist(1,4);//we have 4 choices for color of walls(0 being no wall,1-4 being walls)

    for(int x=0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++){
            // we must have the borders be walls otherwise the light escapes
            if(x==0||y==0||x==WIDTH-1||y==HEIGHT-1){
                worldMap[x][y]=colorDist(gen);
            }
            //make sure the spawn is not walled in and we are stuck
//spawn is 20,12
            else if(x>18 && y>10 && x<22 && y<14){
                worldMap[x][y]=0;
            }
            else{
                if(densityDist(gen)<=10)worldMap[x][y]=colorDist(gen);
                else worldMap[x][y]=0;
            }
        }
    }

    for(int x=1;x<WIDTH-1;x++){
        for(int y=1;y<HEIGHT-1;y++){
            if(x>18 && y>10 && x<22 && y<14) continue;
if(worldMap[x][y] == 0){

                bool solidX = (worldMap[x-1][y] > 0) && (worldMap[x+1][y] > 0);

                bool solidY = (worldMap[x][y-1] > 0) && (worldMap[x][y+1] > 0);

                if( (solidX && !solidY) || (solidY && !solidX) ){

                    if(doorDist(gen) < 10){
                        worldMap[x][y] = 99;
                        Door newDoor;
                        newDoor.locX = x;
                        newDoor.locY = y;
                        newDoor.state = DoorState::CLOSED;
                        newDoor.openAmount = 0.0;
                        newDoor.timer = 0.0;

                        activeDoors.push_back(newDoor);
                    }
                }
            }
        }
    }

}

void Map::generateMaze(){
    activeDoors.clear();
    //fill with walls then remove to make maze
    for(int x=0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++){
            worldMap[x][y]=1;
        }
    }

    std::stack<std::pair<int,int>> stack;
    stack.push({19,11});
    worldMap[19][11]=0;//starting point

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::pair<int,int>> dirs={{0,2},{0,-2},{2,0},{-2,0}};

    while (!stack.empty()) {
        std::pair<int, int> current = stack.top();
        std::vector<std::pair<int, int>> neighbors;

        for (auto d : dirs) {
            int nx = current.first + d.first;
            int ny = current.second + d.second;

            if (nx > 0 && nx < WIDTH - 1 && ny > 0 && ny < HEIGHT - 1 && worldMap[nx][ny] == 1) {
                neighbors.push_back({nx, ny});
            }
        }

        if (!neighbors.empty()) {
            std::uniform_int_distribution<> dist(0, static_cast<int>(neighbors.size() - 1));
            std::pair<int, int> next = neighbors[dist(gen)];

            int wallX = current.first + (next.first - current.first) / 2;
            int wallY = current.second + (next.second - current.second) / 2;

            worldMap[next.first][next.second] = 0;
            worldMap[wallX][wallY] = 0;

            stack.push(next);
        } else {
            stack.pop();
        }

        for(int x = 19; x <= 21; x++) {
            for(int y = 11; y <= 13; y++) {
                worldMap[x][y] = 0;
            }
        }//force clear spawn
        std::uniform_int_distribution<> colorDist(1, 3);
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                if (worldMap[x][y] > 0) worldMap[x][y] = colorDist(gen);
        }

    }
    }

}

int Map::getTile(int x, int y) const{
    if(x<0||x>=WIDTH||y<0||y>=HEIGHT) return 0;

        return worldMap[x][y];
}

void Map::update(int x,int y,int color){
    worldMap[x][y]=color;
}

Door* Map::getDoor(int X, int Y) {
    for(auto& door:activeDoors){
        if(door.locX==X && door.locY==Y){
            return &door;
        }
    }
    return nullptr;
}

const Door* Map::getDoor(int X,int Y) const{
    for(const auto& door:activeDoors){
        if(door.locX==X && door.locY==Y){
            return &door;
        }
    }
    return nullptr;
}

bool Map::isTileSolid(int X, int Y) const{
    int tile = getTile(X, Y);

    if(tile == 0) return false;

    if(tile == 99){
        const Door* door = getDoor(X, Y);
        if(door && door->state == DoorState::OPEN) return false;

        return true;
    }

    return true;
}
