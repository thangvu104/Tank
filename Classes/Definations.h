#pragma once
enum m_Direction {
    DEFAULSE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

#define MOVE_SIZE 200

#define SIN_COS_45 0.71

#define MAP_SIZE_WIDTH 52
#define MAP_SIZE_HEIGHT 52
#define TILE_SIZE 8
#define TANK_SIZE_WITH_TILE 4


#define TIME_TO_TRANSITION 0.5
#define TIME_TO_MOVE 0.1
#define TIME_DELAY 0.05

#define COLLISION_BITMASK_GOLD_POINT 0x000000

#define COLLISION_BITMASK_PLAYER_TANK 0x000001
#define COLLISION_BITMASK_ENEMY_TANK 0x000002

#define COLLISION_BITMASK_PLAYER_BULLET 0x0000011
#define COLLISION_BITMASK_ENEMY_BULLET 0x0000012

#define COLLISION_BITMASK_EARTH_WALL 0x0000021
#define COLLISION_BITMASK_IRON_WALL 0x0000022