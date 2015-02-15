#pragma once
#include "AntScene.h"

class AIEntity
{
public:
    enum TYPE
    {
        NONE = 0,
        RED_ANT = 1,
        BLACK_ANT = 2,
    };
    
    enum STATE
    {
        FORAGE = 1,
        GO_HOME = 2,
        THIRSTY = 3,
        DEAD = 4,
    };

    AIEntity();
    AIEntity(cocos2d::Node *parent, int type, int state, int row, int col, int tileSize);
    ~AIEntity();
    
    void forage(int (*terrain)[MAX_COLS]);
    void goHome(int (*terrain)[MAX_COLS], std::list<AIEntity*> entityList);
    void thirsty(int (*terrain)[MAX_COLS]);
    void dead(int (*terrain)[MAX_COLS]);
    
    bool isHome();
private:
    CC_SYNTHESIZE(int, type, Type);
    CC_SYNTHESIZE(int, state, State);
    CC_SYNTHESIZE(int, row, Row);
    CC_SYNTHESIZE(int, col, Col);
    CC_SYNTHESIZE(cocos2d::Sprite*, image, Image);
};
