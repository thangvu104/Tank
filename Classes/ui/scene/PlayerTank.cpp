#include "PlayerTank.h"
USING_NS_CC;

/// <summary>
/// create a player tank with some info
/// </summary>
/// <param name="entryPoint">Vec2: spawn point of tank</param>
/// <param name="tileMap">TMXTiledMap: map game</param>
/// <param name="listMap">vector<std::vector<int>>: 2d map game</param>
/// <param name="id">int: id tank</param>
PlayerTank::PlayerTank(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::vector<std::vector<int>> listMap, int id)
{
	updateTankInfo(PLAYER_TANK_ID, ALIVE, m_TankType::PLAYER, UP);
}
//end

/// <summary>
/// draw tank into map
/// </summary>
/// <param name="entryPoint">Vec2: spawn point</param>
/// <param name="tileMap">TMXTiledMap: mapgame</param>
/// <param name="pathImage">string: path of tank image</param>
void PlayerTank::m_draw(cocos2d::Vec2 entryPoint, cocos2d::TMXTiledMap* tileMap, std::string pathImage)
{
	playerTank = this->spawnTank(entryPoint, tileMap, pathImage);
	tileMap->addChild(playerTank);
}
//end

/// <summary>
/// move player tank 
/// </summary>
/// <param name="direction">(enum)m_Direction: direction of tank (UP, DOWN, RIGHT or LEFT) </param>
/// <param name="listMap">vector<std::vector<int>>: 2d map game</param>
/// <returns>MoveBy: action move player tank</returns>
cocos2d::MoveBy* PlayerTank::m_Move(m_Direction direction, std::vector<std::vector<int>> listMap)
{
	MoveBy* move = MoveBy::create(0, Vec2(0, 0));;
	RotateTo* rotate = RotateTo::create(0, playerTank->getRotation());
	auto location = getLocation(Vec2(playerTank->getPositionX(), playerTank->getPositionY()));

	//listMap;
	if (direction == UP) {
		rotate = RotateTo::create(0, 0);
		if (location.second >= 3) {
			if (checkMove(listMap, location, direction))
			{
				move = moveUp(playerTank);
			}
		}
	} else if (direction == RIGHT) {
		rotate = RotateTo::create(0, 90);
		if (location.first <= 49) {
			if (checkMove(listMap, location, direction))
			{
				move = moveRight(playerTank);
			}
		}
	} else if (direction == DOWN) {
		rotate = RotateTo::create(0, 180);
		if (location.second <= 49) {
			if (checkMove(listMap, location, direction))
			{
				move = moveDown(playerTank);
			}
		}
	} else if (direction == LEFT) {
		rotate = RotateTo::create(0, 270);
		if (location.first >= 3) {
			if (checkMove(listMap, location, direction))
			{
				move = moveLeft(playerTank);
			}
		}
	} 
	
	playerTank->runAction(rotate);
	return move;
}
//end

/// <summary>
/// shoot bullet
/// </summary>
/// <param name="tileMap">TMXTiledMap: map game</param>
/// <param name="m_sprite">Sprite: sprite of tank</param>
/// <param name="direction">(enum)m_Direction: direction of tank (UP, DOWN, RIGHT or LEFT) </param>
void PlayerTank::m_Shoot(cocos2d::TMXTiledMap* tileMap, cocos2d::Sprite* m_sprite, m_Direction direction)
{
	this->shoot(tileMap, m_sprite, direction, tankInfo.type);
}
//end