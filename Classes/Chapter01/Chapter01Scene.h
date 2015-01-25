#pragma

#include "cocos2d.h"

class Chapter01Scene : public cocos2d::Layer
{
public:
    Chapter01Scene();
    ~Chapter01Scene();
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    void update(float dt);
    
    void menuCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(Chapter01Scene);
private:
    cocos2d::Sprite* m_pPrey;
    cocos2d::Sprite* m_pPredator;
};
