#include "AntScene.h"
#include "Const.h"
#include "MainScene.h"
#include "AIEntity.h"
#include <random>

USING_NS_CC;


// タグ
enum TAG
{
    TAG_BACK,
    
};

// 設定
static const int MAX_WATER = 15;
static const int MAX_POISON = 8;
static const int MAX_FOOD = 20;

// 蟻
AIEntity entityList[MAX_ENTITIES];

int terrain[MAX_ROWS][MAX_COLS];

AntScene::AntScene()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++)
        {
            terrain[i][j] = TERRAIN::GROUND;
        }
    }
    
    terrain[RED_HOME_ROW][RED_HOME_COL] = TERRAIN::RED_HOME;
    terrain[BLACK_HOME_ROW][BLACK_HOME_COL] = TERRAIN::BLACK_HOME;
    
    std::uniform_int_distribution<int> rndRow(2, MAX_ROWS);
    std::uniform_int_distribution<int> rndCol(1, MAX_COLS);
    
    for (int i = 0; i < MAX_WATER; i++) {
        terrain[rndRow(mt) - 3][rndCol(mt) - 1] = TERRAIN::WATER;
    }
    for (int i = 0; i < MAX_POISON; i++) {
        terrain[rndRow(mt) - 3][rndCol(mt) - 1] = TERRAIN::POISON;
    }
    for (int i = 0; i < MAX_FOOD; i++) {
        terrain[rndRow(mt) - 3][rndCol(mt) - 1] = TERRAIN::FOOD;
    }
    
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++)
        {
            log("terrain[%d][%d] : %d", i, j, terrain[i][j]);
        }
    }
}

AntScene::~AntScene()
{
}

Scene* AntScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = AntScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool AntScene::init()
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
        auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(AntScene::menuCallback, this));
        backItem->setTag(TAG_BACK);
        
        // Menu
        auto menu = Menu::create(backItem, nullptr);
        menu->setPosition(visibleSize.width*0.1, visibleSize.height*0.9);
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    
    log("random[%f]", random(0.0f, visibleSize.width));
    
    
    // タイル作成
    {
        int TILE_WIDTH = 15;
        int TILE_HEIGH = 15;
        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLS; j++)
            {
                auto tile = Sprite::create();
                tile->setTextureRect(Rect(0, 0, TILE_WIDTH, TILE_WIDTH));
                switch (terrain[i][j]) {
                    case TERRAIN::GROUND:
                        tile->setColor(Color3B::WHITE);
                        break;
                    case TERRAIN::WATER:
                        tile->setColor(Color3B::BLUE);
                        break;
                    case TERRAIN::BLACK_HOME:
                        tile->setColor(Color3B::BLACK);
                        break;
                    case TERRAIN::RED_HOME:
                        tile->setColor(Color3B::RED);
                        break;
                    case TERRAIN::POISON:
                        tile->setColor(Color3B::MAGENTA);
                        break;
                    case TERRAIN::FOOD:
                        tile->setColor(Color3B::YELLOW);
                        break;
                    default:
                        break;
                }
                tile->setPosition(Vec2(TILE_WIDTH * (j + 1), visibleSize.height - TILE_HEIGH * (i + 1)));
                this->addChild(tile);
            }
        }
    }

    // 蟻
    entityList[0] = *new AIEntity(AIEntity::RED_ANT, AIEntity::FORAGE, 5, 5);
    entityList[1] = *new AIEntity(AIEntity::RED_ANT, AIEntity::FORAGE, 8, 5);
    entityList[2] = *new AIEntity(AIEntity::BLACK_ANT, AIEntity::FORAGE, 5, 36);
    entityList[3] = *new AIEntity(AIEntity::BLACK_ANT, AIEntity::FORAGE, 8, 36);
    
    this->scheduleUpdate();
    
    return true;
}

void AntScene::update(float dt)
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        switch (entityList[i].getState())
        {
            case AIEntity::FORAGE:
                entityList[i].Forage(terrain);
                break;
            case AIEntity::GO_HOME:
                entityList[i].GoHome(terrain, entityList);
                break;
            case AIEntity::THIRSTY:
                entityList[i].Thirsty(terrain);
                break;
            case AIEntity::DEAD:
                entityList[i].Dead(terrain);
                break;
            default:
                break;
        }
    }
}


void AntScene::menuCallback(Ref* pSender)
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

