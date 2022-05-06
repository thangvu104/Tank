#include "EnemyTanks.h"
#include "Definations.h"
USING_NS_CC;

EnemyTank::EnemyTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id)
{
	enemy = spawnTank(entryPoint, tileMap);
	tileMap->addChild(enemy);
	tileMap->schedule([=](float dt) {
		auto sequance = Sequence::create(autoMove(listMap), NULL);

		enemy->runAction(sequance);
		
		}, 0.15, "EnemyTank_" + std::to_string(id));
}

cocos2d::MoveTo* EnemyTank::autoMove(std::vector<std::vector<int>> listMap)
{
	listMap;
	auto i = cocos2d::RandomHelper::random_int(0, 3);
	cocos2d::MoveTo* move = MoveTo::create(0,Vec2(enemy->getPositionX(), enemy->getPositionY()));
	auto location = getLocation(Vec2(enemy->getPositionX(), enemy->getPositionY()));

	if (i == 0) {
		auto rotate = RotateTo::create(0, 0);
		enemy->runAction(rotate);
		if (location.second >= 3) {
			if (listMap[location.first - 2][location.second - 3] == 0 && listMap[location.first - 1][location.second - 3] == 0 && listMap[location.first][location.second - 3] == 0 && listMap[location.first + 1][location.second - 3] == 0)
			{
				move = moveUp(enemy);
			}
		}
		
	}
	else if (i == 1) {
		auto rotate = RotateTo::create(0, 90);
		enemy->runAction(rotate);
		if (location.first <= 49) {
			if (listMap[location.first + 2][location.second - 2] == 0 && listMap[location.first + 2][location.second - 1] == 0 && listMap[location.first + 2][location.second] == 0 && listMap[location.first + 2][location.second + 1] == 0)
			{
				move = moveRight(enemy);
			}
		}
		
	}
	else if (i == 2) {
		auto rotate = RotateTo::create(0, 180);
		enemy->runAction(rotate);
		if (location.second <= 49) {
			if (listMap[location.first - 2][location.second + 2] == 0 && listMap[location.first - 1][location.second + 2] == 0 && listMap[location.first][location.second + 2] == 0 && listMap[location.first + 1][location.second + 2] == 0)
			{
				move = moveDown(enemy);
			}
		}
		
	}
	else if (i == 3) {
		auto rotate = RotateTo::create(0, 270);
		enemy->runAction(rotate);
		if (location.first >= 3) {
			if (listMap[location.first - 3][location.second - 2] == 0 && listMap[location.first - 3][location.second - 1] == 0 && listMap[location.first - 3][location.second] == 0 && listMap[location.first - 3][location.second + 1] == 0)
			{
				move = moveLeft(enemy);
			}
		}
		
	}
	return move;
}

void EnemyTank::autoShoot()
{
}

std::pair<int, int> EnemyTank::getLocation(Vec2 point)
{
	auto locationX = point.x;
	auto locationY = point.y;

	std::pair<int, int> location = std::make_pair((locationX) / (TILE_SIZE), MAP_SIZE_HEIGHT - (locationY) / (TILE_SIZE));

	return location;
}
