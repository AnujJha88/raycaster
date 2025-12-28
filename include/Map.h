#pragma once
#include<vector>
#include "Door.h"

class Map{
public:
    static const int WIDTH=25;
    static const int HEIGHT=25;
    Map(bool random=false,bool isMaze=false);

    void generateRandom();
    void generateMaze();
    int getTile(int x,int y) const;

    void update(int x,int y, int color);

    Door* getDoor(int X, int Y);
    const Door* getDoor(int X, int Y) const;

    bool isTileSolid(int X, int Y) const;

    std::vector<Door>& getDoors(){return activeDoors;}
private:
    int worldMap[WIDTH][HEIGHT];
    std::vector<Door> activeDoors;
};
