#pragma once
#include<vector>
#include "Door.h"

class Map{
public:
    static const int WIDTH=100;
    static const int HEIGHT=100;
    Map(bool random=false);

    void generateRandom();

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
