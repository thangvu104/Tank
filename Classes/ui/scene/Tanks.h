#ifndef __TANK_H__
#define __TANK_H__

#include "cocos2d.h"
#include "Definations.h"
#include "CommonFunc.h"

enum m_TankState {
    ALIVE,
    DIE
};
enum m_TankType {
    ENEMY,
    PLAYER
};
struct TankInfo {
    int id;
    m_TankState state;
    m_TankType type;
    m_Direction moveDirection;
    float speed;
};


class Tanks : public cocos2d::Sprite
{
public:
    Tanks();

    //load tank info
    void updateTankInfo(int _id, m_TankState _state, m_TankType _type, m_Direction _moveDiretion);
    //end


    TankInfo tankInfo;

    //move base
    cocos2d::MoveBy* moveUp(cocos2d::Sprite* m_sprite);
    cocos2d::MoveBy* moveDown(cocos2d::Sprite* m_sprite);
    cocos2d::MoveBy* moveLeft(cocos2d::Sprite* m_sprite);
    cocos2d::MoveBy* moveRight(cocos2d::Sprite* m_sprite);
    //end


    //simple shoot function
    void shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction, m_TankType typeTank);
    //end

    //spawn tank
    cocos2d::Sprite* spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::string pathImage);
    //end
};

#endif //__TANK_H__