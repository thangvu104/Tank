#include "MapGameView.h"
#include "EnemyTanks.h"

MapGameView::MapGameView(cocos2d::TMXTiledMap* tileMap)
{
    for (size_t i = 0; i < MAP_SIZE_WIDTH; i++)
    {
        std::vector<int> lineMapTank;
        for (size_t j = 0; j < MAP_SIZE_HEIGHT; j++)
        {
            lineMapTank.push_back(0);
        }
        listMapTank.push_back(lineMapTank);
    }
    convertTileMapToMap2D(tileMap);
}

/// <summary>
/// transform TMXTiledMap to 2d map
/// </summary>
/// <param name="tileMap">TMXTiledMap: tile map</param>
void MapGameView::convertTileMapToMap2D(TMXTiledMap* tileMap)
{

    auto objectEntryPoints = tileMap->getObjectGroup("EntryPoints");
    auto listChildren = tileMap->getChildren();
    

    // enemy entry point
    int m_count = 1;
    while (objectEntryPoints->getObject("entry-" + std::to_string(m_count)).size() > 0) {
        auto x = objectEntryPoints->getObject("entry-" + std::to_string(m_count)).at("x").asInt();
        auto y = objectEntryPoints->getObject("entry-" + std::to_string(m_count)).at("y").asInt();
        listEntryPoint.push_back(std::make_pair(x, y));
        m_count++;
    }
    //end 


    //get list layer in map
    for (size_t i = 0; i < listChildren.size(); i++)
    {
        listLayer.push_back(tileMap->getLayer((*((cocos2d::TMXLayer*)listChildren.at(i))).getLayerName()));
        
    }
    //end

    //goal point
    auto goalPoint = Sprite::create("ui/images/goal.png");
    goalPoint->setPosition(Vec2(MAP_SIZE_WIDTH * TILE_SIZE / 2, 2 * TILE_SIZE));
    goalPoint->setContentSize(Size(TILE_SIZE * 4, TILE_SIZE * 4));

    auto goalPointBody = PhysicsBody::createBox(Size(TILE_SIZE * 4 - 1, TILE_SIZE * 4 - 1));
    goalPointBody->setContactTestBitmask(true);
    goalPointBody->setDynamic(false);
    goalPointBody->setCollisionBitmask(COLLISION_BITMASK_GOAL_POINT);

    goalPoint->setPhysicsBody(goalPointBody);
    tileMap->addChild(goalPoint);
    //end

    //convert tile map to map2d and add physic body 
    for (size_t x = 0; x < MAP_SIZE_WIDTH; x++)
    {
        std::vector<int> lineInMap;
        for (size_t y = 0; y < MAP_SIZE_HEIGHT; y++)
        {
            int tileValue = 0;
            int currentI = 4;

            //convert tile map to map2d
            for (size_t i = 0; i < listLayer.size(); i++)
            {
                if (listLayer[i]->getTileGIDAt(Vec2(x, y)) > 0 && (listLayer[i]->getLayerName() != "Terrain-2")) {
                    tileValue = listLayer[i]->getTileGIDAt(Vec2(x, y));
                    currentI = i;
                }
            }
            lineInMap.push_back(tileValue);
            //end
           
            if (lineInMap[y] == 1) {
                listLayer[currentI]->getTileAt(Vec2(x, y))->removeFromParent();
            }

            //tile physics body
            if (lineInMap[y] > 1 && ((listLayer[currentI]->getLayerName() != "Terrain-0" && listLayer[currentI]->getLayerName() != "Terrain-0-a")))
            {

                cocos2d::Sprite* tileSprite = listLayer[currentI]->getTileAt(Vec2(x, y));
                auto wallBody = PhysicsBody::createBox(Size(TILE_SIZE - 1, TILE_SIZE - 1));
                wallBody->setContactTestBitmask(true);
                wallBody->setDynamic(false);

                
                if ((lineInMap[y] >= 33 && lineInMap[y] <= 50) || listLayer[currentI]->getLayerName() == "Terrain-1-a") {
                    wallBody->setCollisionBitmask(COLLISION_BITMASK_EARTH_WALL);
                }
                else if (lineInMap[y] >= 129 && lineInMap[y] <= 178) {
                    wallBody->setCollisionBitmask(COLLISION_BITMASK_IRON_WALL);
                }

                tileSprite->setPhysicsBody(wallBody);

            }
            //end

        }
        listMap.push_back(lineInMap);
    }
    listMap;
    //end
}
//end