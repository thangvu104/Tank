#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Definations.h"
#include "MapGameView.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();


    cocos2d::PhysicsWorld* sceneWorld;
    void setPhysicsWorld(cocos2d::PhysicsWorld* world) {
        sceneWorld = world;
    }

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    //move button
    Vec2 m_originPositon;
    m_Direction direction;
    m_Direction findDirection(Vec2 point1, Vec2 point2);
    void onTouchMove(cocos2d::Ref* sender, ui::Widget::TouchEventType eventType);
    float cos(Vec2 point1, Vec2 point2);
    float sin(Vec2 point1, Vec2 point2);
    //end move button

    //map game
    MapGameView* mapGame;
    //end mapgame


    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
