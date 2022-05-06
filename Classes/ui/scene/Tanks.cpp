#include "Tanks.h"
#include "Definations.h"

USING_NS_CC;

Tanks::Tanks()
{
}

cocos2d::Sprite* Tanks::spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap)
{
    auto enemy = Sprite::create("../Resources/ui/images/tank_1.png");
    enemy->setPosition((entryPoint.x),(entryPoint.y));
    enemy->setContentSize(Size(TILE_SIZE * TANK_SIZE_WITH_TILE, TILE_SIZE * TANK_SIZE_WITH_TILE));
    return enemy;
}

cocos2d::MoveTo* Tanks::moveUp(cocos2d::Sprite* m_sprite)
{
    auto move = MoveTo::create(TIME_TO_MOVE, Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() + TILE_SIZE));
    return move;
}

cocos2d::MoveTo* Tanks::moveDown(cocos2d::Sprite* m_sprite)
{
    auto move = MoveTo::create(TIME_TO_MOVE, Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() - TILE_SIZE));
    return move;
}

cocos2d::MoveTo* Tanks::moveLeft(cocos2d::Sprite* m_sprite)
{
    auto move = MoveTo::create(TIME_TO_MOVE, Vec2(m_sprite->getPositionX() - TILE_SIZE, m_sprite->getPositionY()));
    return move;
}

cocos2d::MoveTo* Tanks::moveRight(cocos2d::Sprite* m_sprite)
{
    auto move = MoveTo::create(TIME_TO_MOVE, Vec2(m_sprite->getPositionX() + TILE_SIZE, m_sprite->getPositionY()));
    return move;
}
