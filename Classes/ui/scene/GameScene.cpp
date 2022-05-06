#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    auto layer = GameScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = CSLoader::getInstance()->createNode("csb/ui/Scene/GameScene.csb");
    this->addChild(background);

    //move button
    auto spaceMove = utils::findChild<ui::ImageView*>(this, "space-move");
    auto move = utils::findChild<ui::ImageView*>(this, "director-move");
    m_originPositon = move->getPosition();
    spaceMove->addTouchEventListener(CC_CALLBACK_2(GameScene::onTouchMove, this));
    //end
    
    //mapgame
    auto m_mapGame = utils::findChild<TMXTiledMap*>(this, "map-game");
    m_mapGame->setAnchorPoint(Vec2(0.5, 0.5));
    m_mapGame->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    mapGame = new MapGameView(m_mapGame);
    //end map game

    auto edgeBody = PhysicsBody::createEdgeBox(Size(m_mapGame->getContentSize().width * 2 + 3, m_mapGame->getContentSize().height * 2 + 3), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    edgeBody->setCollisionBitmask(1);
    edgeBody->setContactTestBitmask(true);

    auto edgeNode = Sprite::create();
    edgeNode->setPosition(Vec2(m_mapGame->getPositionX(), m_mapGame->getPositionY()));
    edgeNode->setPhysicsBody(edgeBody);

    this->addChild(edgeNode);

    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}



bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    auto abc = contact.getContactData();
    //auto abcj = contact.
    if ((a->getCollisionBitmask() == COLLISION_BITMASK_ENEMY_BULLET && b->getCollisionBitmask() != COLLISION_BITMASK_ENEMY_TANK && b->getCollisionBitmask() != COLLISION_BITMASK_ENEMY_BULLET)) {
        //a->onRemove();
        if (auto bullet = a->getNode())
        {
            bullet->removeFromParent();
        }
        if (b->getCollisionBitmask() == COLLISION_BITMASK_EARTH_WALL) {
            if (auto bullet = b->getNode())
            {
                bullet->removeFromParent();
            }
        }
    }
    else if (b->getCollisionBitmask() == COLLISION_BITMASK_ENEMY_BULLET && a->getCollisionBitmask() != COLLISION_BITMASK_ENEMY_TANK && a->getCollisionBitmask() != COLLISION_BITMASK_ENEMY_BULLET) {
        //b->onRemove();
        if (auto bullet = b->getNode())
        {
            bullet->removeFromParent();
        }
        if (a->getCollisionBitmask() == COLLISION_BITMASK_EARTH_WALL) {
            if (auto bullet = a->getNode())
            {
                bullet->removeFromParent();
            }
        }
    }

    return true;
}




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
        if (m_originPositon.getDistance(touchPlace) < 100) {
            item->setPosition(touchPlace);
        }
        else if (m_originPositon.getDistance(touchPlace) < 200) {
            if (findDirection(m_originPositon, touchPlace) == UP) {
                item->setPosition(Vec2(m_originPositon.x, m_originPositon.y + MOVE_SIZE / 4));
            }
            else if (findDirection(m_originPositon, touchPlace) == RIGHT) {
                item->setPosition(Vec2(m_originPositon.x + MOVE_SIZE / 4, m_originPositon.y));
            }
            else if (findDirection(m_originPositon, touchPlace) == DOWN) {
                item->setPosition(Vec2(m_originPositon.x, m_originPositon.y - MOVE_SIZE / 4));
            }
            else if (findDirection(m_originPositon, touchPlace) == LEFT) {
                item->setPosition(Vec2(m_originPositon.x - MOVE_SIZE / 4, m_originPositon.y));
            }
        }

        break;
    }
    case ui::Widget::TouchEventType::ENDED:
    case ui::Widget::TouchEventType::CANCELED:
    {
        item->runAction(MoveTo::create(0.25, m_originPositon));
        break;
    }
    default:
        break;
    }
}
m_Direction GameScene::findDirection(Vec2 point1, Vec2 point2)
{
    m_Direction direction = DEFAULSE;
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
float GameScene::cos(Vec2 point1, Vec2 point2)
{
    auto distance = point1.getDistance(point2);

    return (point2.x - point1.x) / distance;
}
float GameScene::sin(Vec2 point1, Vec2 point2)
{
    auto distance = point1.getDistance(point2);

    return (point2.y - point1.y) / distance;
}


