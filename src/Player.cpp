#include "Player.h"


Player::Player(){
    posX=20,posY=12,dirX=1,dirY=0,planeX=0,planeY=0.66;
}

void Player::move(double dx,double dy, const Map &map){
    double newX=posX+dx;

    if(map.getTile(int(newX),int(posY))==0)
    {
        //this is a safe square
        posX=newX;
    }


    double newY=posY+dy;
    if(map.getTile(int(posX),int(newY))==0)
    {
        //this is a safe square
        posY=newY;
    }


}

void Player::turn(double angle){
    double newDirX=dirX*cos(angle)-dirY*sin(angle);
    double newDirY=dirX*sin(angle)+dirY*cos(angle);

    dirX=newDirX;
    dirY=newDirY;

    double newPlaneX=planeX*cos(angle)-planeY*sin(angle);
    double newPlaneY=planeX*sin(angle)+planeY*cos(angle);

    planeX=newPlaneX;
    planeY=newPlaneY;
}
