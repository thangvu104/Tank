#ifndef __PLAYER_TANK_H__
#define __PLAYER_TANK_H__

#include "cocos2d.h"
#include "Tanks.h"
#include "CommonFunc.h"
#include "Definations.h"

class PlayerTank : public Tanks
{
public:
    //create a player tank in map
    PlayerTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id);
    //end

    cocos2d::Sprite* playerTank;
    bool isMove = false;
    int playerLife;

    // draw tank in map
    void m_draw(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::string pathImage);
    //end

    // move player tank 
    cocos2d::MoveBy* m_Move(m_Direction direction, std::vector<std::vector<int>> listMap);
    //end

    // shoot
    void m_Shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction);
    //end

};

#endif //__PLAYER_TANK_H__