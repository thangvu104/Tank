#ifndef __MAP_GAME_VIEW_H__
#define __MAP_GAME_VIEW_H__

#include "cocos2d.h"
#include "Definations.h"
#include "tinyxml2.h"
#include "zlib.h"

USING_NS_CC;

class MapGameView : public cocos2d::Layer
{
public:
    MapGameView(cocos2d::TMXTiledMap* tileMap);

    std::vector<std::vector<int>> listMap;
    void convertTileMapToMap2D(cocos2d::TMXTiledMap* tileMap);
};

#endif // __MAP_GAME_VIEW_H__
