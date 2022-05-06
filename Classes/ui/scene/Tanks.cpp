#include "Tanks.h"

USING_NS_CC;

Tanks::Tanks()
{
    m_moveDirection = DOWN;
}

cocos2d::Sprite* Tanks::spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap)
{
    auto enemy = Sprite::create("../Resources/ui/images/t01.png");
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



void Tanks::shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction)
{
    auto bullet = Sprite::create("../Resources/ui/images/bullet.png");
    bullet->setContentSize(Size(TILE_SIZE, TILE_SIZE));

    auto bulletBody = PhysicsBody::createBox(Size(bullet->getContentSize().width - 1, bullet->getContentSize().height));

    bulletBody->setContactTestBitmask(true);
    bulletBody->setCollisionBitmask(COLLISION_BITMASK_ENEMY_BULLET);

    bulletBody->setDynamic(false);

    bullet->setPhysicsBody(bulletBody);

    cocos2d::RotateTo* rotate;
    cocos2d::MoveTo* move;
    if (direction == UP) {
        rotate = RotateTo::create(0, 0);
        bullet->setPosition(Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() + TILE_SIZE * 2));
        move = MoveTo::create(3, Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() + 416));
    }
    else if (direction == RIGHT) {
        rotate = RotateTo::create(0, 90);
        bullet->setPosition(Vec2(m_sprite->getPositionX() + TILE_SIZE * 2, m_sprite->getPositionY()));
        move = MoveTo::create(3, Vec2(m_sprite->getPositionX() + 416, m_sprite->getPositionY()));
    }
    else if (direction == DOWN) {
        rotate = RotateTo::create(0, 180);
        bullet->setPosition(Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() - TILE_SIZE * 2));
        move = MoveTo::create(3, Vec2(m_sprite->getPositionX(), m_sprite->getPositionY() - 416));
    }
    else if (direction == LEFT) {
        rotate = RotateTo::create(0, 270);
        bullet->setPosition(Vec2(m_sprite->getPositionX() - TILE_SIZE * 2, m_sprite->getPositionY()));
        move = MoveTo::create(3, Vec2(m_sprite->getPositionX() - 416, m_sprite->getPositionY() ));
    }
    bullet->runAction(Sequence::create(rotate, move, NULL));
    

    tileMap->addChild(bullet);

    /*if (bullet->getPositionX() >= 416 || bullet->getPositionX() < 10 ) {
        tileMap->removeChild(bullet);
    }*/
}
