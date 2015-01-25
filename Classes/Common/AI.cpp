#include "AI.h"

USING_NS_CC;

/*
 基本的な追跡アルゴリズム
 */
Vec2 AI::chase(Vec2 predator, Vec2 prey)
{
    if (predator.x > prey.x) {
        predator.x--;
    } else if (predator.x < prey.x) {
        predator.x++;
    }
    
    if (predator.y > prey.y) {
        predator.y--;
    } else if (predator.y < prey.y) {
        predator.y++;
    }
    
    return predator;
}

/*
 基本的な逃避アルゴリズム
*/
Vec2 AI::escape(Vec2 prey, Vec2 predator)
{
    if (prey.x > predator.x) {
        prey.x++;
    } else if (prey.x < predator.x) {
        prey.x--;
    }
    
    if (prey.y > predator.y) {
        prey.y++;
    } else if (prey.y < predator.y) {
        prey.y--;
    }
    
    return prey;
}

/*
 ブレゼンハムアルゴリズム(動作未検証)
 */
void AI::BuildPathToTarget()
{
    int kMaxPathLength = 25;
    int pathRow[kMaxPathLength];
    int pathCol[kMaxPathLength];

    // 始点
    int row = 0;
    int col = 0;
    // 終点
    int endRow = 20;
    int endCol = 20;

    int pathRowTarget = 0;
    int pathColTarget = 0;
    
    int nextRow = row;
    int nextCol = col;
    int deltaRow = endRow - row;
    int deltaCol = endCol - col;
    int stepCol, stepRow;
    int currentStep, fraction;
    
    // 初期化
    for (currentStep = 0; currentStep < kMaxPathLength; currentStep++) {
        pathRow[currentStep] = -1;
        pathCol[currentStep] = -1;
    }
    
    currentStep = 0;
    pathColTarget = endCol;
    pathRowTarget = endRow;
    
    if (deltaRow < 0) {
        stepRow = -1;
    } else {
        stepRow = 1;
    }
    
    if (deltaCol < 0) {
        stepCol = -1;
    } else {
        stepCol = 1;
    }
    
    deltaRow = abs(deltaRow * 2);
    deltaCol = abs(deltaCol * 2);
    
    pathRow[currentStep] = nextRow;
    pathCol[currentStep] = nextCol;
    currentStep++;
    
    if (deltaCol > deltaRow) {
        fraction = deltaRow * 2 - deltaCol;
        while (nextCol != endCol) {
            if (fraction >= 0) {
                nextRow = nextRow + stepRow;
                fraction = fraction - deltaCol;
            }
            nextCol = nextCol + stepCol;
            fraction = fraction + deltaRow;
            pathRow[currentStep] = nextRow;
            pathCol[currentStep] = nextCol;
            currentStep++;
        }
    } else {
        fraction = deltaCol * 2 - deltaRow;
        while (nextRow != endRow) {
            if (fraction >= 0) {
                nextCol = nextCol + stepCol;
                fraction = fraction - deltaRow;
            }
            nextRow = nextRow + stepRow;
            fraction = fraction + deltaCol;
            pathRow[currentStep] = nextRow;
            pathCol[currentStep] = nextCol;
            currentStep++;
        }
    }
}

