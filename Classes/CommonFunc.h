#pragma once
#include "cocos2d.h"
#include "Definations.h"


using namespace std;

enum m_Direction {
    DEFAULSE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

/// <summary>
/// Check if a tile is null for tank move in it
/// </summary>
/// <param name="m_tile">int: vlue of tile</param>
/// <returns></returns>
static bool m_checkTileCanMove(int m_tile) {
    if (m_tile == 0 || m_tile >= 1000) {
        return true;
    }
    return false;
}
//end

/// <summary>
/// check if the tank can move in the direction with the distance is a tile node
/// </summary>
/// <param name="listMap">vector<std::vector<int>>: 2d map game transformed by tileMap</param>
/// <param name="location">pair<int, int>: index of tank in 2d map</param>
/// <param name="m_direction">(enum) m_Direction:the direction of tank to move</param>
/// <returns>true if tank can move in the m_direction</returns>
static bool checkMove(std::vector<std::vector<int>> listMap, std::pair<int, int> location, m_Direction m_direction) {
    if (m_direction == UP)
    {
        if (location.second >= 3) {
            if (m_checkTileCanMove(listMap[location.first - 2][location.second - 3]) && m_checkTileCanMove(listMap[location.first - 1][location.second - 3]) && m_checkTileCanMove(listMap[location.first][location.second - 3]) && m_checkTileCanMove(listMap[location.first + 1][location.second - 3])) {
                return true;
            }
        }
    }
    else if (m_direction == RIGHT) {
        if (location.first <= MAP_SIZE_HEIGHT - 3) {
            if (m_checkTileCanMove(listMap[location.first + 2][location.second - 2]) && m_checkTileCanMove(listMap[location.first + 2][location.second - 1]) && m_checkTileCanMove(listMap[location.first + 2][location.second]) && m_checkTileCanMove(listMap[location.first + 2][location.second + 1])) {
                return true;
            }
        }
    }
    else if (m_direction == DOWN) {
        if (location.second <= MAP_SIZE_HEIGHT - 3) {
            if (m_checkTileCanMove(listMap[location.first - 2][location.second + 2]) && m_checkTileCanMove(listMap[location.first - 1][location.second + 2]) && m_checkTileCanMove(listMap[location.first][location.second + 2]) && m_checkTileCanMove(listMap[location.first + 1][location.second + 2]))
            {
                return true;
            }
        }
    }
    else if (m_direction == LEFT) {
        if (location.first >= 3) {
            if (m_checkTileCanMove(listMap[location.first - 3][location.second - 2]) && m_checkTileCanMove(listMap[location.first - 3][location.second - 1]) && m_checkTileCanMove(listMap[location.first - 3][location.second]) && m_checkTileCanMove(listMap[location.first - 3][location.second + 1]))
            {
                return true;
            }
        }
    }
    return false;
}
//end


/// <summary>
/// check the distance tank can move in the direction
/// </summary>
/// <param name="listMap">vector<std::vector<int>>: 2d map game transformed by tileMap</param>
/// <param name="location">pair<int, int>: index of tank in 2d map</param>
/// <param name="m_direction">m_Direction(enum): the direction of tank to move</param>
/// <param name="_NodeCount">int: max distance tank move with unit is tile node</param>
/// <returns>number of node tank can move in the m_direction</returns>
static int checkMoveARandomNode(std::vector<std::vector<int>> listMap, std::pair<int, int> location, m_Direction m_direction, int _NodeCount) {
    
    for (size_t i = 0; i < _NodeCount; i++)
    {
        if (!checkMove(listMap, location, m_direction)) {
            return i;
        }


        if (m_direction == UP)
        {
            location.second -= 1;
        }
        else if (m_direction == RIGHT) {
            location.first += 1;
        }
        else if (m_direction == DOWN) {
            location.second += 1;
        }
        else if (m_direction == LEFT) {
            location.first -= 1;
        }
         
    }
    return _NodeCount;
}
//end


/// <summary>
/// get index of a object in 2d map of map game
/// </summary>
/// <param name="point">Vec2: position of object</param>
/// <returns>pair<int, int>: the index of object in 2d map</returns>
static std::pair<int, int> getLocation(cocos2d::Vec2 point)
{
    auto locationX = point.x;
    auto locationY = point.y;

    std::pair<int, int> location = std::make_pair((locationX) / (TILE_SIZE), MAP_SIZE_HEIGHT - (locationY) / (TILE_SIZE));

    return location;
}