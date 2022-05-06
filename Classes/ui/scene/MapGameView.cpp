#include "MapGameView.h"
#include "EnemyTanks.h"


MapGameView::MapGameView(cocos2d::TMXTiledMap* tileMap)
{
    convertTileMapToMap2D(tileMap);
    addPhysicsBoudingBox(tileMap);
}

void MapGameView::convertTileMapToMap2D(TMXTiledMap* tileMap)
{
    auto layerTerrain_1 = tileMap->getLayer("Terrain-1");
    auto layerTerrain_1_z = tileMap->getLayer("Terrain-1-z");
    auto objectEntryPoints = tileMap->getObjectGroup("EntryPoints");

    // enemy entry point
    auto entryPoint1 = objectEntryPoints->getObject("entry-1");
    auto xOfEntry1 = entryPoint1.at("x").asInt();
    auto yOfEntry1 = entryPoint1.at("y").asInt();

    auto entryPoint2 = objectEntryPoints->getObject("entry-2");
    auto xOfEntry2 = entryPoint2.at("x").asInt();
    auto yOfEntry2 = entryPoint2.at("y").asInt();

    auto entryPoint3 = objectEntryPoints->getObject("entry-3");
    auto xOfEntry3 = entryPoint3.at("x").asInt();
    auto yOfEntry3 = entryPoint3.at("y").asInt();
    //end 

    for (size_t x = 0; x < MAP_SIZE_WIDTH; x++)
    {
        std::vector<int> lineInMap;
        for (size_t y = 0; y < MAP_SIZE_HEIGHT; y++)
        {

            int nodeOfTerrain_1 = layerTerrain_1->getTileGIDAt(Vec2(x, y));
            int nodeOfTerrain_1_z = layerTerrain_1_z->getTileGIDAt(Vec2(x, y));


            if (nodeOfTerrain_1 != 0) {
                lineInMap.push_back(nodeOfTerrain_1);
            }
            else if (x == xOfEntry1 / 8 && y == tileMap->getMapSize().height - yOfEntry1 / 8 - 4) {
                lineInMap.push_back(1000);
            }
            else if (x == xOfEntry2 / 8 && y == tileMap->getMapSize().height - yOfEntry2 / 8 - 4) {
                lineInMap.push_back(1001);
            }
            else if (x == xOfEntry3 / 8 && y == tileMap->getMapSize().height - yOfEntry3 / 8 - 4) {
                lineInMap.push_back(1002);
            }
            else {
                lineInMap.push_back(nodeOfTerrain_1_z);
            }

        }
        listMap.push_back(lineInMap);
    }
    listMap;
    // end convert tile map


    //enemy
    for (size_t i = 0; i < 1; i++)
    {
        auto m_random = cocos2d::RandomHelper::random_int(1, 3);
        if (m_random == 1) {
            auto enemy1 = new EnemyTank(Vec2(xOfEntry1 + 2 * TILE_SIZE, yOfEntry1 + 2 * TILE_SIZE), tileMap, listMap, i);
        }
        else if (m_random == 2) {
            auto enemy1 = new EnemyTank(Vec2(xOfEntry2 + 2 * TILE_SIZE, yOfEntry2 + 2 * TILE_SIZE), tileMap, listMap, i);
        }
        else if (m_random == 3) {
            auto enemy1 = new EnemyTank(Vec2(xOfEntry3 + 2 * TILE_SIZE, yOfEntry3 + 2 * TILE_SIZE), tileMap, listMap, i);
        }
    }
    //end enemy



    
}

void MapGameView::addPhysicsBoudingBox(cocos2d::TMXTiledMap* tileMap)
{
    for (size_t x = 0; x < MAP_SIZE_WIDTH; x++)
    {

        for (size_t y = 0; y < MAP_SIZE_HEIGHT; y++)
        {
            if (listMap[x][y] > 0 && listMap[x][y] < 1000) {
                auto wallBody = PhysicsBody::createBox(Size(TILE_SIZE - 1, TILE_SIZE -1 ));
                wallBody->setContactTestBitmask(true);
                wallBody->setDynamic(false);

                if (listMap[x][y] == 1) {
                    wallBody->setCollisionBitmask(COLLISION_BITMASK_GOLD_POINT);
                }
                else if (listMap[x][y] >= 33 && listMap[x][y] <= 50) {
                    wallBody->setCollisionBitmask(COLLISION_BITMASK_EARTH_WALL);
                }
                else if (listMap[x][y] >= 129 && listMap[x][y] <= 177) {
                    wallBody->setCollisionBitmask(COLLISION_BITMASK_IRON_WALL);
                }


                auto wallNode = Sprite::create();
                wallNode->setPosition(Vec2(x * TILE_SIZE + TILE_SIZE/2,MAP_SIZE_HEIGHT * TILE_SIZE - y * TILE_SIZE - TILE_SIZE / 2));
                //edgeNode->setAnchorPoint(Vec2(0,0));
                wallNode->setPhysicsBody(wallBody);

                tileMap->addChild(wallNode);
            }
        }
    }
}


