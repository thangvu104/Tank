#include "cocos2d.h"

enum tankState {
    ALIVE,
    DIE
};

class Tanks : public cocos2d::Sprite
{
public:
    Tanks();

    cocos2d::Sprite* spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap);

    cocos2d::MoveTo* moveUp(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveDown(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveLeft(cocos2d::Sprite* m_sprite);
    cocos2d::MoveTo* moveRight(cocos2d::Sprite* m_sprite);


    void shoot();
};
