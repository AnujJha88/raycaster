#pragma once

#include "Map.h"
class Player{

public:
    double posX,posY;
    double dirX,dirY;
    double planeX,planeY;

    Player();
    void move(double dx,double dy,const Map &map);
    void turn(double angle);

    const double movement_speed=0.01;
    const double turn_speed=0.01;

    void moveForward( const Map &map);
    void moveBackward( const Map &map);
    void moveLeft( const Map &map);
    void moveRight( const Map &map);

    void Interact( Map &map );

};
