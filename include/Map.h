#pragma once
class Map{

public:
    static const int WIDTH=24;
    static const int HEIGHT=24;
    Map(bool random=false);

    void generateRandom();
    int getTile(int x,int y)const;
    void update(int x,int y, int color);
private:
    int worldMap[WIDTH][HEIGHT];
};
