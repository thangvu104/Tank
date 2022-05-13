#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Definations.h"
#include "MapGameView.h"
#include "PlayerTank.h"
#include "EnemyTanks.h"
#include "CommonFunc.h"

using namespace cocos2d;
struct m_Map {
    int level;
    int countEnemy;
    int enemyDead;
    void loadMap(int _level, int _countEnemy, int _enemyDead) {
        level = _level;
        countEnemy = _countEnemy;
        enemyDead = _enemyDead;
    }
};


class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(int lifeOfPlayer, int level);
    virtual bool init(int lifeOfPlayer, int level);
    static GameScene* create(int lifeOfPlayer, int level);
    bool isPause = false;
    bool isLose = false;
    bool isStartGame = true;
    std::string pathOfMapGame;


    cocos2d::PhysicsWorld* sceneWorld;
    void setPhysicsWorld(cocos2d::PhysicsWorld* world) {
        sceneWorld = world;
    }

    //collision handling
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    //end
    

    //move button
    Vec2 m_originPositon;
    m_Direction direction;
    m_Direction findDirection(Vec2 point1, Vec2 point2);
    //get touch event and handling it
    void onTouchMove(cocos2d::Ref* sender, ui::Widget::TouchEventType eventType);
    //end

    //get sin-cos of two point with Ox
    float cos(Vec2 point1, Vec2 point2);
    float sin(Vec2 point1, Vec2 point2);
    //end
    
    //end move button

    //map game
    MapGameView* mapGame;
    m_Map mapLevel;
    //end mapgame

   

    //player tank
    PlayerTank* player;
    cocos2d::TMXTiledMap* m_TMXMapGame;
    bool checkShoot = true;
    //end

        //enemy Tank
    std::vector<EnemyTank*> enemyList;

    //return number of enemy in map and dead
    int countEnenmy();
    //end

    // spawn a new enemy
    void spawnEnemy();
    //end
        //end enemy Tank

    void update(float dt);

};

#endif // __GAME_SCENE_H__
