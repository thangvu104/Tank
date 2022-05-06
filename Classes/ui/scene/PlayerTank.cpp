#include "PlayerTank.h"
#include "Definations.h"
USING_NS_CC;

PlayerTank::PlayerTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id)
{
	playerTank = spawnTank(entryPoint, tileMap);

	//physic world
	auto enemyBody = PhysicsBody::createBox(playerTank->getContentSize());
	enemyBody->setContactTestBitmask(true);
	enemyBody->setCollisionBitmask(COLLISION_BITMASK_ENEMY_TANK);
	enemyBody->setDynamic(false);

	playerTank->setPhysicsBody(enemyBody);
	//end physic world


	tileMap->addChild(playerTank);
	tileMap->schedule([=](float dt) {
		auto sequance = Sequence::create(m_Move(listMap), NULL);

		playerTank->runAction(sequance);

		m_Shoot(tileMap, playerTank, this->m_moveDirection);
		
		}, 0.15, "playerTank_" + std::to_string(id));

}

cocos2d::MoveTo* PlayerTank::m_Move(std::vector<std::vector<int>> listMap)
{
	auto move = MoveTo::create(0, Vec2(0, 0));
	return move;
}

void PlayerTank::m_Shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction)
{
	this->shoot(tileMap, m_sprite, direction);
}

std::pair<int, int> PlayerTank::getLocation(Vec2 point)
{
	auto locationX = point.x;
	auto locationY = point.y;

	std::pair<int, int> location = std::make_pair((locationX) / (TILE_SIZE), MAP_SIZE_HEIGHT - (locationY) / (TILE_SIZE));

	return location;
}
