#include "cocos2d.h"
#include "Tanks.h"
class EnemyTank : public Tanks
{
public:
    EnemyTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id);
    cocos2d::Sprite* enemy;

    cocos2d::MoveTo* autoMove(std::vector<std::vector<int>> listMap);
    void autoShoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction);

    std::pair<int, int> getLocation(cocos2d::Vec2 point);
};
