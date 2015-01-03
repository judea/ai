#pragma

#include "cocos2d.h"

class Chapter01Scene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void menuCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(Chapter01Scene);
};
