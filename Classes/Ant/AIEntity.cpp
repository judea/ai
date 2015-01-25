#include "AIEntity.h"


AIEntity::AIEntity()
{
    
}

AIEntity::AIEntity(int type, int state, int row, int col)
{
    this->type = type;
    this->row = row;
    this->col = col;
    this->state = state;
}



AIEntity::~AIEntity()
{
    
}

void AIEntity::Forage(int (*terrain)[MAX_COLS])
{
    int rowMove;
    int colMove;
    int newRow;
    int newCol;
    int foodRow;
    int foodCol;
    int poisonRow;
    int poisonCol;
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rnd(0, 2);
    std::uniform_int_distribution<int> rndRow(2, MAX_ROWS);
    std::uniform_int_distribution<int> rndCol(2, MAX_COLS);

    rowMove = rnd(mt) - 1;
    colMove = rnd(mt) - 1;
    
    newRow = row + rowMove;
    newCol = col + colMove;
    
    if (newRow < 1) return;
    if (newCol < 1) return;
    if (newRow >= MAX_ROWS - 1) return;
    if (newCol >= MAX_COLS - 1) return;
    
    if ((terrain[newRow][newCol] == TERRAIN::GROUND) ||
        (terrain[newRow][newCol] == TERRAIN::WATER)) {
        row = newRow;
        col = newCol;
    }
    
    if (terrain[newRow][newCol] == TERRAIN::FOOD) {
        row = newRow;
        col = newCol;
        terrain[row][col] = TERRAIN::GROUND;
        state = AIEntity::GO_HOME;
        do {
            foodRow = rndRow(mt) - 3;
            foodCol = rndCol(mt) - 3;
        } while (terrain[foodRow][foodCol] != TERRAIN::GROUND);
        terrain[foodRow][foodCol] = TERRAIN::FOOD;
    }
    
    if (terrain[newRow][newCol] == TERRAIN::POISON) {
        row = newRow;
        col = newCol;
        terrain[row][col] = TERRAIN::GROUND;
        state = STATE::DEAD;
        do {
            poisonRow = rndRow(mt) - 3;
            poisonCol = rndCol(mt) - 3;
        } while (terrain[poisonRow][poisonCol] != TERRAIN::GROUND);
        terrain[poisonRow][poisonCol] = TERRAIN::POISON;
    }
}

void AIEntity::GoHome(int (*terrain)[MAX_COLS], AIEntity *entityList)
{
    int rowMove;
    int colMove;
    int newRow;
    int newCol;
    int homeRow;
    int homeCol;
    int index;
    int poisonRow;
    int poisonCol;
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rnd(0, 2);
    std::uniform_int_distribution<int> rndRow(2, MAX_ROWS);
    std::uniform_int_distribution<int> rndCol(2, MAX_COLS);
    
    if (type == TYPE::RED_ANT) {
        homeRow = RED_HOME_ROW;
        homeCol = RED_HOME_COL;
    } else {
        homeRow = BLACK_HOME_ROW;
        homeCol = BLACK_HOME_COL;
    }
    
    if (row < homeRow)
        rowMove = 1;
    else if (row > homeRow)
        rowMove = -1;
    else
        rowMove = 0;
    
    if (col < homeCol)
        colMove = 1;
    else if (col > homeCol)
        colMove = -1;
    else
        colMove = 0;
    
    newRow = row + rowMove;
    newCol = col + colMove;
    
    if (newRow < 1) return;
    if (newCol < 1) return;
    if (newRow >= MAX_ROWS - 1) return;
    if (newCol >= MAX_COLS - 1) return;
    
    if (terrain[newRow][newCol] != TERRAIN::POISON) {
        row = newRow;
        col = newCol;
    } else {
        row = newRow;
        col = newCol;
        terrain[row][col] = TERRAIN::GROUND;
        state = STATE::DEAD;
        do {
            poisonRow = rndRow(mt) - 3;
            poisonCol = rndCol(mt) - 3;
        } while (terrain[poisonRow][poisonCol] != TERRAIN::GROUND);
        terrain[poisonRow][poisonCol] = TERRAIN::POISON;
    }
    
    if ((newRow == homeRow) && (newCol == homeCol)) {
        row = newRow;
        col = newCol;
        state = STATE::THIRSTY;
        for (index = 0; index < MAX_ENTITIES; index++) {
            if (entityList[index].getType() == 0) {
                entityList[index] = *new AIEntity(type, STATE::FORAGE, homeRow, homeCol);
                break;
            }
        }
    }
}

void AIEntity::Thirsty(int (*terrain)[MAX_COLS])
{
    int rowMove;
    int colMove;
    int newRow;
    int newCol;
    int foodRow;
    int foodCol;
    int poisonRow;
    int poisonCol;
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> rnd(0, 2);
    std::uniform_int_distribution<int> rndRow(2, MAX_ROWS);
    std::uniform_int_distribution<int> rndCol(2, MAX_COLS);
    
    rowMove = rnd(mt) - 1;
    colMove = rnd(mt) - 1;
    
    newRow = row + rowMove;
    newCol = col + colMove;
    
    if (newRow < 1) return;
    if (newCol < 1) return;
    if (newRow >= MAX_ROWS - 1) return;
    if (newCol >= MAX_COLS - 1) return;
    
    if ((terrain[newRow][newCol] == TERRAIN::GROUND) ||
        (terrain[newRow][newCol] == TERRAIN::FOOD)) {
        row = newRow;
        col = newCol;
    }
    
    if (terrain[newRow][newCol] == TERRAIN::WATER) {
        row = newRow;
        col = newCol;
        terrain[row][col] = TERRAIN::GROUND;
        state = AIEntity::FORAGE;
        do {
            foodRow = rndRow(mt) - 3;
            foodCol = rndCol(mt) - 3;
        } while (terrain[foodRow][foodCol] != TERRAIN::GROUND);
        terrain[foodRow][foodCol] = TERRAIN::WATER;
    }
    
    if (terrain[newRow][newCol] == TERRAIN::POISON) {
        row = newRow;
        col = newCol;
        terrain[row][col] = TERRAIN::GROUND;
        state = STATE::DEAD;
        do {
            poisonRow = rndRow(mt) - 3;
            poisonCol = rndCol(mt) - 3;
        } while (terrain[poisonRow][poisonCol] != TERRAIN::GROUND);
        terrain[poisonRow][poisonCol] = TERRAIN::POISON;
    }

}

void AIEntity::Dead(int (*terrain)[MAX_COLS])
{
    
}
