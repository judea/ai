#include "Chapter01Scene.h"
#include "Const.h"
#include "MainScene.h"
#include "AI.h"

USING_NS_CC;

static const float SIZE = 25;

enum
{
    TAG_BACK,
    
};

Chapter01Scene::Chapter01Scene()
{
    m_pPrey = nullptr;
    m_pPredator = nullptr;
}

Chapter01Scene::~Chapter01Scene()
{
}

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
        menu->setPosition(visibleSize.width*0.1, visibleSize.height*0.9);
        menu->alignItemsVertically();
        this->addChild(menu);
    }

    log("random[%f]", random(0.0f, visibleSize.width));
    

    // 追跡者
    {
        m_pPrey = Sprite::create();
        m_pPrey->setTextureRect(Rect(0, 0, SIZE, SIZE));
        m_pPrey->setColor(Color3B::BLUE);
        m_pPrey->setPosition(Vec2(random(0.0f, visibleSize.width), random(0.0f, visibleSize.height)));
        this->addChild(m_pPrey);
        
    }
    
    // 逃亡者
    {
        m_pPredator = Sprite::create();
        m_pPredator->setTextureRect(Rect(0, 0, SIZE, SIZE));
        m_pPredator->setColor(Color3B::RED);
        m_pPredator->setPosition(Vec2(Vec2(random(0.0f, visibleSize.width), random(0.0f, visibleSize.height))));
        this->addChild(m_pPredator);
    }
    
    this->scheduleUpdate();
    
    return true;
}

void Chapter01Scene::update(float dt)
{
    // 追跡
    m_pPredator->setPosition(AI::chase(m_pPredator->getPosition(), m_pPrey->getPosition()));
    // 逃避
    m_pPrey->setPosition(AI::escape(m_pPrey->getPosition(), m_pPredator->getPosition()));
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
