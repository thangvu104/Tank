#ifndef __MAP_GAME_VIEW_H__
#define __MAP_GAME_VIEW_H__
#include "cocos2d.h"
#include "Definations.h"
#include "tinyxml2.h"
#include "zlib.h"
#include "CommonFunc.h"

USING_NS_CC;

class MapGameView : public cocos2d::Layer
{
public:
    MapGameView(cocos2d::TMXTiledMap* tileMap);
    //map game 2d
    std::vector<std::vector<int>> listMap;
    //end

    //map tank
    std::vector<std::vector<int>> listMapTank;
    //end
    std::vector<std::pair<int, int>> listEntryPoint;

    //list layer is childen of tileMap
    std::vector<TMXLayer*> listLayer;
    //end


    /// <summary>
    /// convert tile map to map2d and add physics body
    /// </summary>
    /// <param name="tileMap">TMXTileMap: is a tile map</param>
    void convertTileMapToMap2D(cocos2d::TMXTiledMap* tileMap);
    ///end

};

#endif // __MAP_GAME_VIEW_H__
