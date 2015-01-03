#include "Chapter01Scene.h"
#include "Const.h"
#include "MainScene.h"

USING_NS_CC;

enum
{
    TAG_BACK,
    
};

Scene* Chapter01Scene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = Chapter01Scene::create();

    scene->addChild(layer);

    return scene;
}

bool Chapter01Scene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Menu
    {
        // Back
        auto backLabel = Label::createWithTTF("Back", SYSTEM_FONT, FONT_SIZE);
        auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(Chapter01Scene::menuCallback, this));
        backItem->setTag(TAG_BACK);
        
        // Menu
        auto menu = Menu::create(backItem, nullptr);
        menu->setPosition(visibleSize.width/2, visibleSize.height/2);
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    
    return true;
}


void Chapter01Scene::menuCallback(Ref* pSender)
{
    auto menu = (MenuItem*)pSender;
    int id = menu->getTag();
    log("Menu : id[%d]", id);
    
    switch (id) {
        case TAG_BACK:
            log("back");
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainScene::createScene(), Color3B::WHITE));            break;
        default:
            break;
    }
}
