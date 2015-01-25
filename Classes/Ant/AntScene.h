#pragma once

#include "cocos2d.h"

// 地形配列
static const int MAX_ROWS = 32;
static const int MAX_COLS = 42;

// ゲーム環境初期化
static const int RED_HOME_ROW = 5;
static const int RED_HOME_COL = 5;

static const int BLACK_HOME_ROW = 5;
static const int BLACK_HOME_COL = 36;

static const int MAX_ENTITIES = 4;

// 地形値
enum TERRAIN
{
    GROUND = 1,
    WATER = 2,
    BLACK_HOME = 3,
    RED_HOME = 4,
    POISON = 5,
    FOOD = 6,
};

class AntScene : public cocos2d::Layer
{
public:
    AntScene();
    ~AntScene();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void update(float dt);
    
    void menuCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(AntScene);
private:
};
