#pragma once

#include "cocos2d.h"

class AI
{
public:
    static cocos2d::Vec2 chase(cocos2d::Vec2 predator, cocos2d::Vec2 prey);
    static cocos2d::Vec2 escape(cocos2d::Vec2 prey, cocos2d::Vec2 predator);
    static void BuildPathToTarget();

private:
};
