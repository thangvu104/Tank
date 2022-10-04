#include "EnemyTanks.h"

USING_NS_CC;

/// <summary>
/// spawn enemy tank and load it info
/// </summary>
/// <param name="entryPoint">Vec2: spawn point of enemy</param>
/// <param name="tileMap">TMXTiledMap: map game</param>
/// <param name="listMap">vector<std::vector<int>>: mapgame 2d </param>
/// <param name="id">int: id tank</param>
EnemyTank::EnemyTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> *listMap, int id)
{
	updateTankInfo(id, ALIVE, ENEMY, DOWN);

	_tileMap = tileMap;
	_listMap = listMap;

	enemy = this->spawnTank(entryPoint, tileMap, "ui/images/t01.png");

	tileMap->addChild(enemy);

	AI();
}
//end

/// <summary>
/// auto move function of enemy
/// </summary>
/// <param name="listMap">vector<std::vector<int>>: mapgame 2d convert from tileMap</param>
/// <returns>MoveBy: action move of enemy tank</returns>
cocos2d::MoveBy* EnemyTank::autoMove(std::vector<std::vector<int>> listMap)
{
	auto i = cocos2d::RandomHelper::random_int(0, 3);
	move = cocos2d::MoveBy::create(0, cocos2d::Vec2(0, 0));
	auto location = getLocation(Vec2(enemy->getPositionX(), enemy->getPositionY()));

	if (i == 0) {
		auto rotate = RotateTo::create(0, 180);
		enemy->runAction(rotate);
		tankInfo.moveDirection = UP;
		if (checkMove(listMap, location, tankInfo.moveDirection))
		{
			move = moveUp(enemy);
		}
		
	}
	else if (i == 1) {
		auto rotate = RotateTo::create(0, 270);
		enemy->runAction(rotate);
		tankInfo.moveDirection = RIGHT;
		if (checkMove(listMap, location, tankInfo.moveDirection))
		{
			move = moveRight(enemy);
		}
		
	}
	else if (i == 2) {
		auto rotate = RotateTo::create(0, 0);
		enemy->runAction(rotate);
		tankInfo.moveDirection = DOWN;

		if (checkMove(listMap, location, tankInfo.moveDirection))
		{
			move = moveDown(enemy);
		}
		
	}
	else if (i == 3) {
		auto rotate = RotateTo::create(0, 90);
		enemy->runAction(rotate);
		tankInfo.moveDirection = LEFT;
		if (checkMove(listMap, location, tankInfo.moveDirection))
		{
			move = moveLeft(enemy);
		}
		
	}
	
	return move;
}
//end

/// <summary>
/// auto shoot function of tank
/// </summary>
/// <param name="tileMap">TMXTiledMap: map game</param>
/// <param name="m_sprite">Sprite: sprite of enemy</param>
/// <param name="direction">(enum)m_Direction: direction of enemy</param>
void EnemyTank::autoShoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction)
{
	this->shoot(tileMap, m_sprite, direction, tankInfo.type);
}
//end


/// <summary>
/// move enemy with action of function autoMove()
/// </summary>
void EnemyTank::AI()
{
		int m_random = cocos2d::RandomHelper::random_int(1, 30);
		move = autoMove(*_listMap);
		cocos2d::Repeat* m_repeatAction = cocos2d::Repeat::create(move, checkMoveARandomNode(*_listMap, getLocation(Vec2(enemy->getPositionX(), enemy->getPositionY())), tankInfo.moveDirection, m_random));
		enemy->runAction(Sequence::create(
			m_repeatAction,
			DelayTime::create(0.5f),
			CallFunc::create([=] {
				AI();
				}),
			NULL)
			);
		
		if (m_random % 2 == 0) {
			autoShoot(_tileMap, enemy, tankInfo.moveDirection);
		}
}
//end

//vector<pair<int, int>> EnemyTank::findPath(int _x, int _y, int x, int y)
//{
//	std::pair<int, int> s = { _x + 1, _y + 1 };
//	std::pair<int, int> t = { x + 1, y + 1 };
//
//	//BFS
//	const int dx[4] = { -1, 0, 1, 0 };
//	const int dy[4] = { 0, 1, 0, -1 };
//	std::deque<std::pair<int, int>> q;
//	std::vector<std::vector<std::pair<int, int>>> trace(e.size(), std::vector<std::pair<int, int>>(e[0].size(), std::make_pair(-1, -1)));
//	q.push_back(t);
//	trace[t.first][t.second] = std::make_pair(-2, -2);
//	e[s.first][s.second] = 0;
//	e[t.first][t.second] = 0;
//	while (!q.empty()) {
//		auto u = q.front();
//		q.pop_front();
//		if (u == s) break;
//		for (int i = 0; i < 4; ++i) {
//			int x = u.first + dx[i];
//			int y = u.second + dy[i];
//			while (x >= 0 && x < e.size() && y >= 0 && y < e[0].size() && e[x][y] == 0) {
//				if (trace[x][y].first == -1) {
//					trace[x][y] = u;
//					q.push_back({ x, y });
//				}
//				x += dx[i];
//				y += dy[i];
//			}
//		}
//	}
//
//	//trace back
//	std::vector<std::pair<int, int>> res;
//	if (trace[s.first][s.second].first != -1) {
//		while (s.first != -2) {
//			res.push_back({ s.first - 1, s.second - 1 });
//			s = trace[s.first][s.second];
//		}
//	}
//	return res;
//}