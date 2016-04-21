#include "gamemodel.h"
#include <QDebug>
#include <ctype.h>

GameModel::GameModel()
{
    //Constructor
    for (int u = 0; u < 8; u++) {
        grid.push_back( QVector<MathNode>());
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            MathNode mn;
            grid[i].push_back(mn);
        }
    }

    for (int i = 0; i < 25; i++) {
        levelMap[i] = new Level(i,i,i,i,i,i,i,i);
    }

    PopulateGrid();
}

//GameModel::~GameModel()
//{
//    //Destructor
//}

bool GameModel::FormulaCheck(QVector<bool> opList)
{
    //Checks if formula is valid
    if (!opList[0]) {
        bool lastBool = opList[0];
        if (opList.length() >= 3) {
            for (int i = 1; i < opList.length(); i++) {
                if ((lastBool && opList[i]) || (!lastBool && !opList[i])) {
                    return false;
                }
                lastBool = opList[i];
            }
            if (opList[opList.length()-1]) {
                return false;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }
    return false;
}

int GameModel::FormulaReader(QVector<QString> formula)
{
    //Interprets formula
    /*QStack<QString> operators;
    QStack<QString> operands;

    int totalVal = 0;

    for (int i = 0; i < formula.length(); i ++) {
        if (isdigit(formula[i][0])) {

            if (operators.length() > 0) {
                if (operators.top() == "*") {
                    operands.push(operands.pop() * formula[i].toInt());
                    operators.pop();
                } else if (operators.top() == "/") {
                    operands.push(operands.pop() / formula[i].toInt());
                    operators.pop();
                }
            } else {
            operands.push(formula[i]);
            }
        }
        else if (formula[i] == "+") {

        }
        else if (formula[i] == "-") {

        }
    }*/
}

void GameModel::ShuffleGrid()
{
    //Shuffles grid to prevent
}

void GameModel::BombGrid(QString bombOp)
{
    //Applies operation bombOp on all operands in grid
}

void GameModel::RemoveNode(QVector<QPair<int, int> > removedNodes)
{
    //Removes Nodes from the grid
    for (int i = 0; i < removedNodes.length(); i++) {
        //Set removedNodes to empty
        grid[removedNodes[i].first][removedNodes[i].second].isSelected = false;
        grid[removedNodes[i].first][removedNodes[i].second].isOperator = false;
        grid[removedNodes[i].first][removedNodes[i].second].value = "0";
    }

    for (int x = 0; x < grid.size(); x++) {
        int emptyCount = 0;
        for (int y = 0; y < grid[x].size(); y++) {
            if (grid[x][y].value == "0") {
                emptyCount++;
            }
        }
        if (emptyCount > 0) {
            for (int y = grid[x].size()-1; y >= 0; y--) {
                //Going through y column bottom up
                if (grid[x][y].value == "0") {
                    int curNode = y;
                    while (curNode >= 0) {
                        //Find non-empty MathNode to swap with
                        if (grid[x][curNode].value == "0") {
                            curNode--;
                        } else {
                            //Swap the MathNode with the empty one
                            MathNode temp = grid[x][y];
                            grid[x][y] = grid[x][curNode];
                            grid[x][curNode] = temp;
                        }
                    }
                }
            }
        }
    }
}

void GameModel::PopulateGrid()
{
    //Goes through grid and smartly populates it with MathNodes
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid[x].size(); y++) {
            if (grid[x][y].value == "0") {
                //Node is empty
                QVector<bool> operatorSpread;
                if (x != 0) {
                    if (grid[x-1][y].value != "0") {
                        operatorSpread.push_back(grid[x-1][y].isOperator);
                    }
                }
                if (y != 0) {
                    if (grid[x][y-1].value != "0") {
                        operatorSpread.push_back(grid[x][y-1].isOperator);
                    }
                }
                if (x != grid.size()-1) {
                    if (grid[x+1][y].value != "0") {
                        operatorSpread.push_back(grid[x+1][y].isOperator);
                    }
                }
                if (y != grid[x].size()-1) {
                    if (grid[x][y+1].value != "0") {
                        operatorSpread.push_back(grid[x][y+1].isOperator);
                    }
                }
                //operatorSpread is now full of the surrounding isOperator bools

                double yCount = operatorSpread.count(true);
                if (yCount != 0) {
                    yCount = yCount/operatorSpread.length();
                    if (yCount > 0.5) {
                        //Create a operand MathNode
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                    } else if (yCount < 0.5) {
                        //Create a operator MathNode
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                    } else {
                        //Create a random MathNode
                        if (qrand() % 2 == 0) {
                            grid[x][y].value = GenerateMathNode(false);
                            grid[x][y].isOperator = false;
                        } else {
                            grid[x][y].value = GenerateMathNode(true);
                            grid[x][y].isOperator = true;
                        }
                    }
                } else {
                    //Create a random MathNode
                    if (qrand() % 2 == 0) {
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                    } else {
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                    }
                }

            }
        }
    }
}

QString GameModel::GenerateMathNode(bool isOperator)
{
    //Generates MathNode based on difficulty
    QString ret = "";
    int num;
    if (!isOperator) {
        switch (difficulty) {
        case 1: //Easy
            num = qrand() % 5 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + num;
            } else {
                ret = num;
            }
            return ret;
            break;
        case 2: //Medium
            num = qrand() % 10 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + num;
            } else {
                ret = num;
            }
            return ret;
            break;
        case 3: //Hard
            num = qrand() % 20 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + num;
            } else {
                ret = num;
            }
            return ret;
            break;
        case 4: //Very Hard
            num = qrand() % 50 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + num;
            } else {
                ret = num;
            }
            return ret;
            break;
        case 5: //Extreme
            num = qrand() % 100 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + num;
            } else {
                ret = num;
            }
            return ret;
            break;
        }
    } else {
        int num;
        switch (difficulty) {
        case 1: //Easy
            num = qrand() % 2;
            break;
        case 2:
            num = qrand() % 3;
            break;
        case 3:
            num = qrand() % 4;
            break;
        case 4:
            num = qrand() % 3 + 1;
            break;
        case 5:
            num = qrand() % 2 + 2;
            break;
        }
        switch (num) {
        case 0: //Plus
            ret = "+";
            break;
        case 1: //Minus
            ret = "-";
            break;
        case 2: //Multiply
            ret = "*";
            break;
        case 3: //Divide
            ret = "/";
            break;
        }
    }
}

void GameModel::LevelStart(int lNum)
{
    levelNum = lNum;
    difficulty = lNum % 5 + 1;
    targetNum = levelMap[levelNum]->targetNum;
    currentNum = 0;
    mBombCounter = levelMap[levelNum]->moduloBomb;
    mul2BombCounter = levelMap[levelNum]->multiplyTwoBomb;
    mul4BombCounter = levelMap[levelNum]->multiplyFourBomb;
    div2BombCounter = levelMap[levelNum]->divideTwoBomb;
    movesRemaining = levelMap[levelNum]->movesNum;

    PopulateGrid();
}

void GameModel::OnMove(QVector<QPair<int, int> > cellList)
{
    QVector<QString> formula;
    QVector<bool> boolList;
    for (auto i = 0; i < cellList.length(); i++) {
        if (grid[cellList[i].first][cellList[i].second].isOperator) {
            boolList.push_back(true);
        } else {
            boolList.push_back(false);
        }
        formula.push_back(grid[cellList[i].first][cellList[i].second].value);
    }

    if (FormulaCheck(boolList)) {
        //valid formula
        int result = FormulaReader(formula);
        currentNum += result;
        //TODO: Check win-state and proceed appropriately
    } else {
        //invalid formula
        emit InvalidFormulaSig();
    }
}
