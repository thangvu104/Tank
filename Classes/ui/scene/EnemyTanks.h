#ifndef __ENEMY_TANK_H__
#define __ENEMY_TANK_H__
#include "cocos2d.h"
#include "Tanks.h"
#include "Definations.h"
#include "CommonFunc.h"


class EnemyTank : public Tanks
{
public:

    //create enemy tank 
    EnemyTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> *listMap, int id);
    //end


    cocos2d::Sprite* enemy;
    cocos2d::MoveBy* move;

    //auto move enemy tank
    cocos2d::MoveBy* autoMove(std::vector<std::vector<int>> listMap);
    //end

    //shoot
    void autoShoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction);
    //end

    // call pack function automove
    void AI();
    //end

    std::vector<pair<int, int>> findPath(int _x, int _y, int x, int y);
private:
    cocos2d::TMXTiledMap* _tileMap;
    std::vector<std::vector<int>>* _listMap;
};
#endif //__ENEMY_TANK_H__