#pragma once

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    CREATE_FUNC(MainScene);
private:
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuCallback(cocos2d::Ref* pSender);

};
