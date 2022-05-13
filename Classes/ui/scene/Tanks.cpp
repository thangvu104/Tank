#include "Tanks.h"

USING_NS_CC;

Tanks::Tanks()
{
    
}

/// <summary>
/// load tank info
/// </summary>
/// <param name="_id">int: id of tank</param>
/// <param name="_state">(enum)m_TankState: sate of tank (LIVE or DIE) </param>
/// <param name="_type">(enum)m_TankType: type of tank (ENEMY or PLAYER) </param>
/// <param name="_moveDiretion">(enum)m_Direction: direction of tank(UP, DOWN, LEFT, RIGHT)</param>
void Tanks::updateTankInfo(int _id, m_TankState _state, m_TankType _type, m_Direction _moveDiretion)
{
    tankInfo.id = _id;
    tankInfo.state = _state;
    tankInfo.type = _type;
    tankInfo.moveDirection = _moveDiretion;
    tankInfo.speed = 0;
}
//end

/// <summary>
/// spawn enemy
/// </summary>
/// <param name="entryPoint">Vec2: entry point of tank</param>
/// <param name="tileMap">TMXTiledMap: mapgame</param>
/// <param name="pathImage">string: path of tank image</param>
/// <returns>Sprite: tank </returns>
cocos2d::Sprite* Tanks::spawnTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::string pathImage)
{
    auto tank = Sprite::create();
    tank->setTexture(pathImage);
    tank->setPosition((entryPoint.x),(entryPoint.y));
    tank->setContentSize(Size(TILE_SIZE * TANK_SIZE_WITH_TILE, TILE_SIZE * TANK_SIZE_WITH_TILE));
    
    //physic world
    auto tankBody = PhysicsBody::createBox(tank->getContentSize());
    tankBody->setContactTestBitmask(true);

    if (tankInfo.type == m_TankType::ENEMY)
    {
        tankBody->setCollisionBitmask(COLLISION_BITMASK_ENEMY_TANK);
    }
    else if (tankInfo.type == m_TankType::PLAYER) {
        tankBody->setCollisionBitmask(COLLISION_BITMASK_PLAYER_TANK);
    }

    tankBody->setDynamic(false);
    tank->setPhysicsBody(tankBody);
    //end physic world

    return tank;
}
//end

/// <summary>
/// move up a distance is a tile node
/// </summary>
/// <param name="m_sprite">Sprite: tank sprite</param>
/// <returns>MoveBy: action of tank</returns>
cocos2d::MoveBy* Tanks::moveUp(cocos2d::Sprite* m_sprite)
{
    auto move = MoveBy::create(TIME_TO_MOVE, Vec2(0, TILE_SIZE));
    return move;
}
//end

/// <summary>
/// move down a distance is a tile node
/// </summary>
/// <param name="m_sprite">Sprite: tank sprite</param>
/// <returns>MoveBy: action of tank</returns>
cocos2d::MoveBy* Tanks::moveDown(cocos2d::Sprite* m_sprite)
{
    auto move = MoveBy::create(TIME_TO_MOVE, Vec2(0, -TILE_SIZE));
    return move;
}
//end

/// <summary>
/// move left a distance is a tile node
/// </summary>
/// <param name="m_sprite">Sprite: tank sprite</param>
/// <returns>MoveBy: action of tank</returns>
cocos2d::MoveBy* Tanks::moveLeft(cocos2d::Sprite* m_sprite)
{
    auto move = MoveBy::create(TIME_TO_MOVE, Vec2(- TILE_SIZE, 0));
    return move;
}
//end

/// <summary>
/// move right a distance is a tile node
/// </summary>
/// <param name="m_sprite">Sprite: tank sprite</param>
/// <returns>MoveBy: action of tank</returns>
cocos2d::MoveBy* Tanks::moveRight(cocos2d::Sprite* m_sprite)
{
    auto move = MoveBy::create(TIME_TO_MOVE, Vec2(TILE_SIZE, 0));
    return move;
}
//end

/// <summary>
/// create a bullet and shoot it
/// </summary>
/// <param name="tileMap">TMXTiledMap: map game</param>
/// <param name="m_sprite">Sprite: tank</param>
/// <param name="direction">(enum)m_Direction: the direction of tank to shoot in it</param>
/// <param name="typeTank">int: tank type to make physics body of bullet</param>
void Tanks::shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction, m_TankType typeTank)
{
    auto bullet = Sprite::create("ui/images/bullet.png");
    bullet->setContentSize(Size(TILE_SIZE*1.3, TILE_SIZE * 1.3));

    auto bulletBody = PhysicsBody::createBox(Size(bullet->getContentSize().width - 1, bullet->getContentSize().height));

    bulletBody->setContactTestBitmask(true);
    if (typeTank == ENEMY) {
        bulletBody->setCollisionBitmask(COLLISION_BITMASK_ENEMY_BULLET);
    }
    else {
        bulletBody->setCollisionBitmask(COLLISION_BITMASK_PLAYER_BULLET);
    }
    

    bulletBody->setDynamic(false);

    bullet->setPhysicsBody(bulletBody);

    cocos2d::RotateTo* rotate;
    cocos2d::MoveTo* move;
    if (direction == UP || direction == DEFAULSE) {
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

}
//end