#include "cocos2d.h"
#include "Tanks.h"
class PlayerTank : public Tanks
{
public:
    PlayerTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id);
    cocos2d::Sprite* playerTank;

    cocos2d::MoveTo* m_Move(std::vector<std::vector<int>> listMap);
    void m_Shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction);

    std::pair<int, int> getLocation(cocos2d::Vec2 point);
};
