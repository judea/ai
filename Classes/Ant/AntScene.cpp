#include "AntScene.h"
#include "Const.h"
#include "MainScene.h"
#include "AIEntity.h"
#include <random>

USING_NS_CC;

// ゲーム状態
enum COND
{
    INIT,
    MAIN,
    END,
};

// タグ
enum TAG
{
    TAG_BACK,
    
};

// Z座標
enum ZPOS
{
    TILE,
    ANT,
};

// 設定
static const int MAX_WATER = 15;
static const int MAX_POISON = 8;
static const int MAX_FOOD = 20;

// 蟻
AIEntity *entityList[MAX_ENTITIES];

int terrain[MAX_ROWS][MAX_COLS];
Sprite *tile[MAX_ROWS][MAX_COLS];
Sprite *ant[MAX_ENTITIES];

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
    
    
    // 蟻
    entityList[0] = new AIEntity(AIEntity::RED_ANT, AIEntity::FORAGE, 5, 5);
    entityList[1] = new AIEntity(AIEntity::RED_ANT, AIEntity::FORAGE, 8, 5);
    entityList[2] = new AIEntity(AIEntity::BLACK_ANT, AIEntity::FORAGE, 5, 36);
    entityList[3] = new AIEntity(AIEntity::BLACK_ANT, AIEntity::FORAGE, 8, 36);
}

AntScene::~AntScene()
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        delete(entityList[i]);
    }
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
    if ( !LayerColor::initWithColor(Color4B::GRAY) )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    cond = COND::INIT;
    fieldSize = 0;
    tileSize = 0;

    // Menu
    {
        // Back
        auto backLabel = Label::createWithTTF("Back", SYSTEM_FONT, FONT_SIZE);
        auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(AntScene::menuCallback, this));
        backItem->setTag(TAG_BACK);
        
        // Menu
        auto menu = Menu::create(backItem, nullptr);
        menu->setPosition(visibleSize.width*0.8, visibleSize.height*0.9);
        menu->alignItemsVertically();
        this->addChild(menu);
    }
    
    log("random[%f]", random(0.0f, visibleSize.width));
    
    
    // タイル作成
    {
        // フィールド
        if (visibleSize.width > visibleSize.height) {
            fieldSize = visibleSize.height;
        } else {
            fieldSize = visibleSize.width;
        }
        
        // タイル
        if (MAX_ROWS > MAX_COLS) {
            tileSize = fieldSize / MAX_ROWS;
        } else {
            tileSize = fieldSize / MAX_COLS;
        }
        
        for (int i = 0; i < MAX_ROWS; i++) {
            for (int j = 0; j < MAX_COLS; j++)
            {
                tile[i][j] = Sprite::create();
                tile[i][j]->setTextureRect(Rect(0, 0, tileSize, tileSize));
                tile[i][j]->setColor(Color3B::WHITE);
                tile[i][j]->setAnchorPoint(Vec2(0, 1));
                tile[i][j]->setPosition(Vec2(tileSize * (j + 1), visibleSize.height - tileSize * (i + 1)));
                this->addChild(tile[i][j], TILE);
            }
        }
    }

    for (int i = 0; i < MAX_ENTITIES; i++) {
        ant[i] = nullptr;
        switch(entityList[i]->getType()) {
            case AIEntity::TYPE::BLACK_ANT:
                ant[i] = Sprite::create("RedAnt.png");
                break;
            case AIEntity::TYPE::RED_ANT:
                ant[i] = Sprite::create("BlackAnt.png");
                break;
            default:
                break;
        }
        if(ant[i] == nullptr) continue;
        
        auto imgSize = ant[i]->getContentSize();
        auto imgScale = tileSize / imgSize.width;
        ant[i]->setScale(imgScale);
        this->addChild(ant[i], ANT);
    }
    
    
    this->scheduleUpdate();
    
    return true;
}

void AntScene::update(float dt)
{
    switch (cond) {
        case COND::INIT:
        {
            cond = COND::MAIN;
            break;
        }
        case COND::MAIN:
        {
            for (int i = 0; i < MAX_ENTITIES; i++) {
                switch (entityList[i]->getState())
                {
                    case AIEntity::FORAGE:
                        entityList[i]->Forage(terrain);
                        break;
                    case AIEntity::GO_HOME:
                        entityList[i]->GoHome(terrain, entityList);
                        break;
                    case AIEntity::THIRSTY:
                        entityList[i]->Thirsty(terrain);
                        break;
                    case AIEntity::DEAD:
                        entityList[i]->Dead(terrain);
                        break;
                    default:
                        break;
                }
            }
            
            for (int i = 0; i < MAX_ROWS; i++) {
                for (int j = 0; j < MAX_COLS; j++)
                {
                    switch (terrain[i][j]) {
                        case TERRAIN::GROUND:
                            tile[i][j]->setColor(Color3B::WHITE);
                            break;
                        case TERRAIN::WATER:
                            tile[i][j]->setColor(Color3B::BLUE);
                            break;
                        case TERRAIN::BLACK_HOME:
                            tile[i][j]->setColor(Color3B::BLACK);
                            break;
                        case TERRAIN::RED_HOME:
                            tile[i][j]->setColor(Color3B::RED);
                            break;
                        case TERRAIN::POISON:
                            tile[i][j]->setColor(Color3B::MAGENTA);
                            break;
                        case TERRAIN::FOOD:
                            tile[i][j]->setColor(Color3B::YELLOW);
                            break;
                        default:
                            break;
                    }
                }
            }
            
            for (int i = 0; i < MAX_ENTITIES; i++) {
                auto entity = entityList[i];

                log("ant[%d](%d) : row[%d], col[%d]", i, entity->getState(), entity->getRow(), entity->getCol());
                
                float x = tileSize * (entity->getCol() + 1);
                float y = visibleSize.height - tileSize * (entity->getRow() + 1);
                
                ant[i]->setPosition(x, y);
//                if (entity.getType() == AIEntity::BLACK_ANT) {
//                    tile[entity.getRow()][entity.getCol()]->setColor(Color3B::BLACK);
//                } else {
//                    tile[entity.getRow()][entity.getCol()]->setColor(Color3B::RED);
//                }
            }
            
            // 生存確認
            int deadNum = 0;
            for (int i = 0; i < MAX_ENTITIES; i++) {
                auto entity = entityList[i];
                if (entity->getState() == AIEntity::STATE::DEAD) {
                    deadNum++;
                }
            }
            
            if (deadNum >= MAX_ENTITIES) {
                cond = COND::END;
            }
            break;
        }
        case COND::END:
        {
            log("END");
            break;
        }
        default:
            break;
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

