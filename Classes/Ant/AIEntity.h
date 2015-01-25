#pragma once
#include "AntScene.h"

class AIEntity
{
public:
    enum TYPE
    {
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
    AIEntity(int type, int state, int row, int col);
    ~AIEntity();
    
    void Forage(int (*terrain)[MAX_COLS]);
    void GoHome(int (*terrain)[MAX_COLS], AIEntity *entityList);
    void Thirsty(int (*terrain)[MAX_COLS]);
    void Dead(int (*terrain)[MAX_COLS]);
private:
    CC_SYNTHESIZE(int, type, Type);
    CC_SYNTHESIZE(int, state, State);
    CC_SYNTHESIZE(int, row, Row);
    CC_SYNTHESIZE(int, col, Col);
};
