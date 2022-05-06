#include "cocos2d.h"
#include "Definations.h"

enum tankState {
    ALIVE,
    DIE
};

class Tanks : public cocos2d::Sprite
{
public:
    Tanks();
    m_Direction m_moveDirection;

    cocos2d::Sprite* spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap);

    cocos2d::MoveTo* moveUp(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveDown(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveLeft(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveRight(cocos2d::Sprite* m_sprite);


    void shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction);
};
