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
    READY,
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

// 説明用ラベル
static const float POS_DESC_LABEL_X = 800;
static const float POS_DESC_LABEL_Y = 600;
static const float POS_DESC_LABEL_Y_SPACE = -20;

//　蟻用ラベル
static const float POS_ANT_LABEL_X = 50;
static const float POS_ANT_LABEL_Y = 150;
static const float POS_ANT_LABEL_Y_SPACE = -30;


// 蟻
//AIEntity *entityList[MAX_ENTITIES];
std::list<AIEntity*> entityList;

int terrain[MAX_ROWS][MAX_COLS];
Sprite *tile[MAX_ROWS][MAX_COLS];

AntScene::AntScene()
{
    // 地形設定
    
    // 地面
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++)
        {
            terrain[i][j] = TERRAIN::GROUND;
        }
    }
    
    //　蟻拠点設定
    terrain[RED_HOME_ROW][RED_HOME_COL] = TERRAIN::RED_HOME;
    terrain[BLACK_HOME_ROW][BLACK_HOME_COL] = TERRAIN::BLACK_HOME;
    
    // 水
    setTerrain(TERRAIN::WATER, MAX_WATER);

    // 毒
    setTerrain(TERRAIN::POISON, MAX_POISON);
    
    // 食料
    setTerrain(TERRAIN::FOOD, MAX_FOOD);

    // 地形データ確認(デバッグ用)
//    for (int i = 0; i < MAX_ROWS; i++) {
//        for (int j = 0; j < MAX_COLS; j++)
//        {
//            log("terrain[%d][%d] : %d", i, j, terrain[i][j]);
//        }
//    }
}

AntScene::~AntScene()
{
    for (std::list<AIEntity*>::iterator it = entityList.begin(); it != entityList.end(); it++) {
        CC_SAFE_DELETE(*it);
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
    
    // ラベル
    {
        auto groundLabel = Label::createWithTTF("GROUND", SYSTEM_FONT, FONT_SIZE);
        groundLabel->setTextColor(Color4B::WHITE);
        groundLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 0);
        this->addChild(groundLabel);
        
        auto redHomeLabel = Label::createWithTTF("RED_HOME", SYSTEM_FONT, FONT_SIZE);
        redHomeLabel->setTextColor(Color4B::RED);
        redHomeLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 1);
        this->addChild(redHomeLabel);
        
        auto blackHomeLabel = Label::createWithTTF("BLACK_HOME", SYSTEM_FONT, FONT_SIZE);
        blackHomeLabel->setTextColor(Color4B::BLACK);
        blackHomeLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 2);
        this->addChild(blackHomeLabel);

        auto waterLabel = Label::createWithTTF("WATER", SYSTEM_FONT, FONT_SIZE);
        waterLabel->setTextColor(Color4B::BLUE);
        waterLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 3);
        this->addChild(waterLabel);

        auto poisonLabel = Label::createWithTTF("POISON", SYSTEM_FONT, FONT_SIZE);
        poisonLabel->setTextColor(Color4B::MAGENTA);
        poisonLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 4);
        this->addChild(poisonLabel);

        auto foodLabel = Label::createWithTTF("FOOD", SYSTEM_FONT, FONT_SIZE);
        foodLabel->setTextColor(Color4B::YELLOW);
        foodLabel->setPosition(POS_DESC_LABEL_X, POS_DESC_LABEL_Y + POS_DESC_LABEL_Y_SPACE * 5);
        this->addChild(foodLabel);
    }
    
//    log("random[%f]", random(0.0f, visibleSize.width));
    
    // 蟻表示用ラベル
    redAntLabel = Label::createWithTTF("0", SYSTEM_FONT, FONT_SIZE);
    redAntLabel->setTextColor(Color4B::RED);
    redAntLabel->setPosition(POS_ANT_LABEL_X, POS_ANT_LABEL_Y + POS_ANT_LABEL_Y_SPACE * 0);
    this->addChild(redAntLabel);
    
    blackAntLabel = Label::createWithTTF("0", SYSTEM_FONT, FONT_SIZE);
    blackAntLabel->setTextColor(Color4B::BLACK);
    blackAntLabel->setPosition(POS_ANT_LABEL_X, POS_ANT_LABEL_Y + POS_ANT_LABEL_Y_SPACE * 1);
    this->addChild(blackAntLabel);
    
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
    
    // 蟻
    entityList.clear();
    entityList.push_back(new AIEntity(this, AIEntity::RED_ANT, AIEntity::FORAGE, 5, 5, tileSize));
    entityList.push_back(new AIEntity(this, AIEntity::RED_ANT, AIEntity::FORAGE, 8, 5, tileSize));
    entityList.push_back(new AIEntity(this, AIEntity::BLACK_ANT, AIEntity::FORAGE, 5, 36, tileSize));
    entityList.push_back(new AIEntity(this, AIEntity::BLACK_ANT, AIEntity::FORAGE, 8, 36, tileSize));
    
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
            for (std::list<AIEntity*>::iterator it = entityList.begin(); it != entityList.end();) {
                auto entity = *it;
                switch (entity->getState())
                {
                    case AIEntity::FORAGE:
                        entity->forage(terrain);
                        break;
                    case AIEntity::GO_HOME:
                        entity->goHome(terrain, entityList);
                        if (entity->isHome()) {
                            entityList.push_back(new AIEntity(this, entity->getType(), AIEntity::STATE::FORAGE, entity->getRow(), entity->getCol(), tileSize));
                        }
                        break;
                    case AIEntity::THIRSTY:
                        entity->thirsty(terrain);
                        break;
                    case AIEntity::DEAD:
                        entity->dead(terrain);
                        it = entityList.erase(it);
                        CC_SAFE_DELETE(entity);
                        continue;
                        break;
                    default:
                        break;
                }
                it++;
            }
            
            for (int i = 0; i < MAX_ROWS; i++) {
                for (int j = 0; j < MAX_COLS; j++)
                {
                    switch (terrain[i][j]) {
                        case TERRAIN::GROUND:
                            tile[i][j]->setColor(Color3B::WHITE);
                            break;
                        case TERRAIN::RED_HOME:
                            tile[i][j]->setColor(Color3B::RED);
                            break;
                        case TERRAIN::BLACK_HOME:
                            tile[i][j]->setColor(Color3B::BLACK);
                            break;
                        case TERRAIN::WATER:
                            tile[i][j]->setColor(Color3B::BLUE);
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
            
            // 蟻移動
            for (std::list<AIEntity*>::iterator it = entityList.begin(); it != entityList.end(); it++) {
                auto entity = *it;

//                log("ant(%d) : row[%d], col[%d]", entity->getState(), entity->getRow(), entity->getCol());
                
                float x = tileSize * (entity->getCol() + 1);
                float y = visibleSize.height - tileSize * (entity->getRow() + 1);
                
                entity->getImage()->setPosition(x, y);
            }
            
            // 生存確認
            if (countAnt() == 0) {
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
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainScene::createScene(), Color3B::WHITE));            break;
        default:
            break;
    }
}

void AntScene::setTerrain(TERRAIN kind, int size)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rndRow(2, MAX_ROWS);
    std::uniform_int_distribution<int> rndCol(1, MAX_COLS);

    for (int i = 0; i < size; i++) {
        int row = rndRow(mt) - 3;
        int col = rndCol(mt) - 1;
        if (terrain[row][col] == TERRAIN::GROUND) {
            terrain[row][col] = kind;
        } else {
            i--;
        }
    }
}

int AntScene::countAnt()
{
    int redAntNum = 0;
    int blackAntNum = 0;
    
    for (std::list<AIEntity*>::iterator it = entityList.begin(); it != entityList.end(); it++) {
        auto entity = *it;
        if (entity->getType() == AIEntity::RED_ANT) {
            redAntNum++;
        } else {
            blackAntNum++;
        }
    }
    
    redAntLabel->setString(StringUtils::format("%d", redAntNum));
    blackAntLabel->setString(StringUtils::format("%d", blackAntNum));
    
    return redAntNum + blackAntNum;
}