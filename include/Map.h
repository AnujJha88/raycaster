#pragma once
#include<vector>
class Map{

public:
    static const int WIDTH=24;
    static const int HEIGHT=24;
    Map();
    int getTile(int x,int y)const;
private:
    int worldMap[WIDTH][HEIGHT];
}
