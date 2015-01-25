#include "MainScene.h"
#include "Const.h"
#include "Chapter01/Chapter01Scene.h"
#include "Ant/AntScene.h"

USING_NS_CC;

enum {
    TAG_CHAPTER01,
    TAG_CHAPTER02,
};

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Chapter
    {
        // Chapter01
        auto chapter01Label = Label::createWithTTF("Chapter01", SYSTEM_FONT, FONT_SIZE);
        auto chapter01Item = MenuItemLabel::create(chapter01Label, CC_CALLBACK_1(MainScene::menuCallback, this));
        chapter01Item->setTag(TAG_CHAPTER01);
        
        // Chapter02
        auto chapter02Label = Label::createWithTTF("Ant", SYSTEM_FONT, FONT_SIZE);
        auto chapter02Item = MenuItemLabel::create(chapter02Label, CC_CALLBACK_1(MainScene::menuCallback, this));
        chapter02Item->setTag(TAG_CHAPTER02);
        
        // Menu
        auto menu = Menu::create(chapter01Item, chapter02Item, nullptr);
        menu->setPosition(visibleSize.width/2, visibleSize.height/2);
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    
    // Close
    {
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(MainScene::menuCloseCallback, this));
        
        closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                    origin.y + closeItem->getContentSize().height/2));
        
        auto menu = Menu::create(closeItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
    }
    
    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::menuCallback(Ref* pSender)
{
    auto menu = (MenuItem*)pSender;
    int id = menu->getTag();
    log("Menu : id[%d]", id);
    
    switch (id) {
        case TAG_CHAPTER01:
            log("chapter01");
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, Chapter01Scene::createScene(), Color3B::WHITE));            break;
        case TAG_CHAPTER02:
            log("chapter02");
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, AntScene::createScene(), Color3B::WHITE));            break;
        default:
            break;
    }
}
