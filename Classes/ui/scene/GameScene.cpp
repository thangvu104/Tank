#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
/// <summary>
/// create a new GameScene scene and add physics world for it
/// </summary>
/// <returns></returns>
Scene* GameScene::createScene(int lifeOfPlayer, int level)
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));

    auto layer = GameScene::create(lifeOfPlayer, level);
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}
GameScene* GameScene::create(int lifeOfPlayer, int level) {
    GameScene* pRet = new(std::nothrow) GameScene();
    if (pRet && pRet->init(lifeOfPlayer, level))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
bool GameScene::init(int lifeOfPlayer, int level)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = CSLoader::getInstance()->createNode("csb/ui/scene/GameScene.csb");
    this->addChild(background);

    mapLevel.loadMap(1, 5, 0);
   

    //move button
    auto spaceMove = utils::findChild<ui::ImageView*>(this, "space-move");
    auto move = utils::findChild<ui::ImageView*>(this, "director-move");
    m_originPositon = move->getPosition();
    spaceMove->addTouchEventListener(CC_CALLBACK_2(GameScene::onTouchMove, this));
    //end
    
    //get Tile Map and convert it to map2d by constructor of MapGameView
    m_TMXMapGame = utils::findChild<TMXTiledMap*>(this, "map-game");
    m_TMXMapGame->setAnchorPoint(Vec2(0.5, 0.5));
    m_TMXMapGame->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    m_TMXMapGame->removeAllChildrenWithCleanup(true);
    m_TMXMapGame->initWithTMXFile("ui/assets/map/"+ std::to_string(level) +".tmx");
    mapGame = new MapGameView(m_TMXMapGame);
    //end get Tile Map
    
    //physic body around map game
    auto edgeBody = PhysicsBody::createEdgeBox(Size(m_TMXMapGame->getContentSize().width * 1.55 + 3, m_TMXMapGame->getContentSize().height * 1.55 + 3), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    edgeBody->setCollisionBitmask(COLLISION_BITMASK_MAP_BOX);
    edgeBody->setContactTestBitmask(true);

    auto edgeNode = Sprite::create();
    edgeNode->setPosition(Vec2(m_TMXMapGame->getPositionX(), m_TMXMapGame->getPositionY()));
    edgeNode->setPhysicsBody(edgeBody);

    this->addChild(edgeNode);
    //end


    //player tank
    player = new PlayerTank(Vec2(18 * TILE_SIZE, 2 * TILE_SIZE), m_TMXMapGame, mapGame->listMap, 0);
    player->m_draw(Vec2(18 * TILE_SIZE, 2 * TILE_SIZE), m_TMXMapGame, "ui/images/tank_2.png");
    player->playerLife = lifeOfPlayer;
    
    m_TMXMapGame->schedule([=](float dt) {
        if (player->isMove) {
            player->playerTank->runAction(Sequence::create(player->m_Move(player->tankInfo.moveDirection, mapGame->listMap), NULL));
        }
        }, 0.15, "PlayerTank");
    //end
    
    //shooting button  
    auto shootingButton = utils::findChild<ui::Button*>(this, "shoot-button");
    bool checkschedule = true;
    int dm=0;
    shootingButton->addClickEventListener([=](Ref*) {
        if (checkShoot)
        {
            player->m_Shoot(m_TMXMapGame, player->playerTank, player->tankInfo.moveDirection);
            checkShoot = false;
            this->scheduleOnce([=](float) {
                checkShoot = true;

                }, RATE_OF_FIRE, "shoot");
        }
        });
    //end

    

     //pause button
    auto pauseButton = utils::findChild<ui::Button*>(this, "pause-button");
    pauseButton->addClickEventListener([=](Ref*) {
        auto elementInMap = m_TMXMapGame->getChildren();
        if (!isPause) {
            pauseButton->loadTextureNormal("ui/images/play.png");
            isPause = true;
            for (size_t i = 0; i < elementInMap.size(); i++)
            {
                elementInMap.at(i)->pause();
            }
            shootingButton->setTouchEnabled(false);
            spaceMove->setTouchEnabled(false);
            player->pause();
            this->pause();
        }
        else {
            pauseButton->loadTextureNormal("ui/images/pause-button.png");
            isPause = false;
            for (size_t i = 0; i < elementInMap.size(); i++)
            {
                elementInMap.at(i)->resume();
            }
            shootingButton->setTouchEnabled(true);
            spaceMove->setTouchEnabled(true);
            player->resume();
            this->resume();
        }
        });
    //end


    //contact event
    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    //end

    this->scheduleUpdate();
    return true;
}


/// <summary>
/// collision handling
/// </summary>
/// <param name="contact">PhysicsContact: contact</param>
/// <returns>bool: true</returns>
bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* m_shapeA = contact.getShapeA()->getBody();
    PhysicsBody* m_shapeB = contact.getShapeB()->getBody();

    auto collisionBitmaskA = m_shapeA->getCollisionBitmask();
    auto collisionBitmaskB = m_shapeB->getCollisionBitmask();

    auto contactData = contact.getContactData();
    //auto abcj = contact.
    
    if (collisionBitmaskB == COLLISION_BITMASK_EARTH_WALL && collisionBitmaskA != COLLISION_BITMASK_MAP_BOX) {
        auto bulletNode = m_shapeA->getNode()->getPosition();
        auto wallNode = m_shapeB->getNode()->getPosition();
        std::pair<int, int> clearNode;
        
        if (bulletNode.y == wallNode.y) {
            clearNode = getLocation(Vec2(wallNode.x, wallNode.y + TILE_SIZE));
        }
        else if (bulletNode.y == wallNode.y + TILE_SIZE) {
            clearNode = getLocation(Vec2(wallNode.x, wallNode.y - TILE_SIZE));
        }
        else if (bulletNode.x == wallNode.x) {
            clearNode = getLocation(Vec2((wallNode.x + TILE_SIZE), (wallNode.y)));
        }
        else if (bulletNode.x == wallNode.x + TILE_SIZE) {
            clearNode = getLocation(Vec2((wallNode.x - TILE_SIZE), (wallNode.y)));
        }
        
        for (size_t i = 0; i < mapGame->listLayer.size(); i++)
        {
            if (mapGame->listLayer[i]->getTileGIDAt(Vec2(clearNode.first, clearNode.second)) > 0) {
                mapGame->listLayer[i]->getTileAt(Vec2(clearNode.first, clearNode.second))->removeFromParent();
            }
               
        }
        mapGame->listMap[clearNode.first][clearNode.second] = 0;
        mapGame->listMap;
    }
        

    if (collisionBitmaskA != collisionBitmaskB) {

        //contact of enemy bullet
        if (collisionBitmaskA == COLLISION_BITMASK_ENEMY_BULLET) {
            if (collisionBitmaskB != COLLISION_BITMASK_ENEMY_BULLET && collisionBitmaskB != COLLISION_BITMASK_ENEMY_TANK) {
                if (auto bullet = m_shapeA->getNode())
                {
                        bullet->setVisible(false);
                        bullet->stopAllActions();
                        this->scheduleOnce([=](float) {
                            bullet->removeFromParent();
                            }, 0.1, "1");
                }

                if (collisionBitmaskB == COLLISION_BITMASK_GOAL_POINT) {
                    isLose = true;
                }
                else if (collisionBitmaskB == COLLISION_BITMASK_PLAYER_TANK) {
                    auto tank = m_shapeB->getNode();
                    player->playerLife--;
                }
                else if (collisionBitmaskB != COLLISION_BITMASK_IRON_WALL && collisionBitmaskB != COLLISION_BITMASK_MAP_BOX)
                {
                    if (auto tileB = m_shapeB->getNode()) {
                        tileB->removeFromParent();
                    }
                    

                    for (size_t i = 0; i < contactData->count; i++)
                    {
                        auto clearNode = getLocation(Vec2((contactData->points[i].x - POSITION_LEFT_RIGHT_OF_MAP) / SCALE_TILE_NODE_IN_MAP, (contactData->points[i].y - POSITION_TOP_BUTTON_OF_MAP) / SCALE_TILE_NODE_IN_MAP));
                        mapGame->listMap[clearNode.first][clearNode.second] = 0;
                    }
                }
            }

        }
        else if (collisionBitmaskB == COLLISION_BITMASK_ENEMY_BULLET) {
            if (collisionBitmaskA != COLLISION_BITMASK_ENEMY_BULLET && collisionBitmaskA != COLLISION_BITMASK_ENEMY_TANK) {
                if (auto bullet = m_shapeB->getNode())
                {
                    bullet->setVisible(false);
                    bullet->stopAllActions();
                    this->scheduleOnce([=](float) {
                        bullet->removeFromParent();
                        }, 0.1, "2");
                }


                if (collisionBitmaskA == COLLISION_BITMASK_PLAYER_TANK) {
                    auto tank = m_shapeA->getNode();
                    player->playerLife--;
                }
                else if (collisionBitmaskA != COLLISION_BITMASK_IRON_WALL && collisionBitmaskA != COLLISION_BITMASK_MAP_BOX)
                {
                    if (auto tileA = m_shapeA->getNode()) {
                        tileA->removeFromParent();
                    }
                    for (size_t i = 0; i < contactData->count; i++)
                    {
                        auto clearNode = getLocation(Vec2((contactData->points[i].x - POSITION_LEFT_RIGHT_OF_MAP) / SCALE_TILE_NODE_IN_MAP, (contactData->points[i].y - POSITION_TOP_BUTTON_OF_MAP) / SCALE_TILE_NODE_IN_MAP));
                        mapGame->listMap[clearNode.first][clearNode.second] = 0;
                    }
                }
            }
        }
        //end

        //contact of player bullet
        if (collisionBitmaskA == COLLISION_BITMASK_PLAYER_BULLET && collisionBitmaskB != COLLISION_BITMASK_PLAYER_TANK) {
            
            if (auto bullet = m_shapeA->getNode())
            {
                bullet->setVisible(false);
                bullet->stopAllActions();
                this->scheduleOnce([=](float) {
                    bullet->removeFromParent();
                    }, 0.1,"3");
            }

            //delete earth wall or enemy or enemy bullet
            if (collisionBitmaskB != COLLISION_BITMASK_IRON_WALL && collisionBitmaskB != COLLISION_BITMASK_MAP_BOX)
            {
                if (auto tileB = m_shapeB->getNode()) {
                    if (collisionBitmaskB == COLLISION_BITMASK_ENEMY_TANK)
                    {
                        auto node = std::find(enemyList.begin(), enemyList.end(), tileB);
                        auto index = node - enemyList.begin() - 1;
                        enemyList.erase(enemyList.begin() + index);
                        mapLevel.enemyDead++;
                    }
                    tileB->removeFromParent();
                }

                for (size_t i = 0; i < contactData->count; i++)
                {
                    auto clearNode = getLocation(Vec2((contactData->points[i].x - POSITION_LEFT_RIGHT_OF_MAP) / SCALE_TILE_NODE_IN_MAP, (contactData->points[i].y - POSITION_TOP_BUTTON_OF_MAP) / SCALE_TILE_NODE_IN_MAP));
                    mapGame->listMap[clearNode.first][clearNode.second] = 0;
                }
            }
            //end
        }
        else if (collisionBitmaskB == COLLISION_BITMASK_PLAYER_BULLET && collisionBitmaskA != COLLISION_BITMASK_PLAYER_TANK) {
           
            if (auto bullet = m_shapeB->getNode())
            {
                bullet->setVisible(false);
                bullet->stopAllActions();
                this->scheduleOnce([=](float) {
                    bullet->removeFromParent();
                    }, 0.1, "4");
            }

            if (collisionBitmaskA != COLLISION_BITMASK_IRON_WALL && collisionBitmaskA != COLLISION_BITMASK_MAP_BOX)
            {
                
                if (auto tileA = m_shapeA->getNode()) {
                    if (collisionBitmaskA == COLLISION_BITMASK_ENEMY_TANK)
                    {
                        auto node = std::find(enemyList.begin(), enemyList.end(), tileA);
                        auto index = node - enemyList.begin() - 1;
                        enemyList.erase(enemyList.begin() + index);
                        mapLevel.enemyDead++;
                    }
                    tileA->removeFromParent();
                }
                for (size_t i = 0; i < contactData->count; i++)
                {
                    auto clearNode = getLocation(Vec2((contactData->points[i].x - POSITION_LEFT_RIGHT_OF_MAP) / SCALE_TILE_NODE_IN_MAP, (contactData->points[i].y - POSITION_TOP_BUTTON_OF_MAP) / SCALE_TILE_NODE_IN_MAP));
                    mapGame->listMap[clearNode.first][clearNode.second] = 0;
                }
            }
        }
        //end
    }
    //this->scheduleUpdate();
    return true;
}
//end


/// <summary>
/// handling the touch event
/// </summary>
/// <param name="sender">Ref: sender</param>
/// <param name="eventType">ui::Widget::TouchEventType: eventType</param>
void GameScene::onTouchMove(cocos2d::Ref* sender, ui::Widget::TouchEventType eventType)
{
    auto spaceMove = utils::findChild<ui::ImageView*>(this, "space-move");
    auto touchPlace = spaceMove->getTouchMovePosition();
    auto item = utils::findChild<ui::ImageView*>(this, "director-move");

    switch (eventType)
    {
    case ui::Widget::TouchEventType::BEGAN:

        break;
    case ui::Widget::TouchEventType::MOVED:
    {
        player->isMove = true;
        player->tankInfo.moveDirection = findDirection(m_originPositon, touchPlace);
        //shootDirection = playerDirection;
        
        float moveLocation = 0;
        if (m_originPositon.getDistance(touchPlace) < 50) {
            moveLocation = m_originPositon.getDistance(touchPlace);
        }
        else if (m_originPositon.getDistance(touchPlace) <= 200) {
            moveLocation = MOVE_SIZE / 4;
        }

        if (m_originPositon.getDistance(touchPlace) <= 300) {

            if (player->tankInfo.moveDirection == UP) {
                item->setPosition(Vec2(m_originPositon.x, m_originPositon.y + moveLocation));
            }
            else if (player->tankInfo.moveDirection == RIGHT) {
                item->setPosition(Vec2(m_originPositon.x + moveLocation, m_originPositon.y));
            }
            else if (player->tankInfo.moveDirection == DOWN) {
                item->setPosition(Vec2(m_originPositon.x, m_originPositon.y - moveLocation));
            }
            else if (player->tankInfo.moveDirection == LEFT) {
                item->setPosition(Vec2(m_originPositon.x - moveLocation, m_originPositon.y));
            }
        }
        else {
            //player->isMove = false;
            item->setPosition(Vec2(m_originPositon.x, m_originPositon.y));
        }
        break;
    }
    case ui::Widget::TouchEventType::ENDED:
    case ui::Widget::TouchEventType::CANCELED:
    {
        player->isMove = false;
        item->runAction(MoveTo::create(0.25, m_originPositon));
        break;
    }
    default:
        break;
    }
    
}
//end

/// <summary>
/// find the dirction of point 2 with point 1
/// </summary>
/// <param name="point1">Vec2: point 1</param>
/// <param name="point2">Vec2: point2</param>
/// <returns>(enum)m_Direction: direction of point2 with point1</returns>
m_Direction GameScene::findDirection(Vec2 point1, Vec2 point2)
{
    if (cos(point1, point2) >= SIN_COS_45) {
        direction = RIGHT;
    }
    else if (cos(point1, point2) <= -SIN_COS_45) {
        direction = LEFT;
    }
    else if (sin(point1, point2) >= SIN_COS_45) {
        direction = UP;
    }
    else {
        direction = DOWN;
    }
    return direction;
}
//end

/// <summary>
/// find cos of a line, which has point1 and point2, with Ox
/// </summary>
/// <param name="point1">Vec2: point1</param>
/// <param name="point2">Vec2: point2</param>
/// <returns>float: cos of line</returns>
float GameScene::cos(Vec2 point1, Vec2 point2)
{
    auto distance = point1.getDistance(point2);

    return (point2.x - point1.x) / distance;
}
//end

/// <summary>
/// find sin of a line, which has point1 and point2, with Ox
/// </summary>
/// <param name="point1">Vec2: point1</param>
/// <param name="point2">Vec2: point2</param>
/// <returns>float: sin of line</returns>
float GameScene::sin(Vec2 point1, Vec2 point2)
{
    auto distance = point1.getDistance(point2);

    return (point2.y - point1.y) / distance;
}
//end


/// <summary>
/// count number of enemy in map and dead
/// </summary>
/// <returns>int: number of enemy in map and dead</returns>
int GameScene::countEnenmy()
{
    return enemyList.size() + mapLevel.enemyDead;
}
//end

/// <summary>
/// spawn a new enemy in map
/// </summary>
void GameScene::spawnEnemy()
{
    int listEntryPointSize = mapGame->listEntryPoint.size();
    auto m_random = cocos2d::RandomHelper::random_int(0, listEntryPointSize - 1);
    auto coordinates = Vec2(mapGame->listEntryPoint[m_random].first + 2 * TILE_SIZE, mapGame->listEntryPoint[m_random].second + 2 * TILE_SIZE);
    auto enemy = new EnemyTank(coordinates, m_TMXMapGame, &mapGame->listMap, 0);
    enemyList.push_back(enemy);
}
//end

/// <summary>
/// schedule update call it
/// </summary>
/// <param name="dt">float dt</param>
void GameScene::update(float dt)
{
    
    //life of player's tank 
    auto lifePlayerTankText = utils::findChild<ui::Text*>(this, "life-text");
    lifePlayerTankText->setString(std::to_string(player->playerLife));
    //end

    //spawn enemy tank
    if (countEnenmy() < mapLevel.countEnemy) {
        auto randomSpawn = cocos2d::RandomHelper::random_int(0, 5000);
        if (randomSpawn == 0) {
            spawnEnemy();
        }
        if (enemyList.size() == 0) {
            spawnEnemy();
        }
    }
    //end
}
//end


