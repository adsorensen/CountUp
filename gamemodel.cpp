#include "gamemodel.h"

GameModel::GameModel()
{
    //Constructor
}

GameModel::~GameModel()
{
    //Destructor
}

bool GameModel::FormulaCheck(QString formula)
{
    //Checks if formula is valid
}

int GameModel::FormulaReader(QString formula)
{
    //Interprets formula
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

//    for (int x = 0; x < grid.size(); x++) {
//        int emptyCount = 0;
//        for (int y = 0; y < grid[x].size(); y++) {
//            if (grid[x][y].value == "0") {
//                emptyCount++;
//            }
//        }
//        if (emptyCount > 0) {
//            for (int y = grid[x].size; y > 0; y--) {
//                //Going through y column bottom up
//                if (grid[x][y].value == "0") {
//                    int curNode = y;
//                    while (curNode >= 0) {
//                        //Find non-empty MathNode to swap with
//                        if (grid[x][curNode].value == "0") {
//                            curNode--;
//                        } else {
//                            //Swap the MathNode with the empty one
//                            MathNode temp = grid[x][y];
//                            grid[x][y] = grid[x][curNode];
//                            grid[x][curNode] = temp;
//                        }
//                    }
//                }
//            }
//        }
//    }
}

void GameModel::PopulateGrid()
{
    //Goes through grid and smartly populates it with MathNodes
//    for (int x = 0; x < grid.size(); x++) {
//        for (int y = 0; y < grid[y].size(); y++) {
//            if (grid[x][y].value == "0") {
//                //Node is empty
//                QVector<bool> operatorSpread;
//                if (x != 0) {
//                    if (grid[x-1][y].value != "0") {
//                        operatorSpread.push_back(grid[x-1][y].isOperator);
//                    }
//                }
//                if (y != 0) {
//                    if (grid[x][y-1].value != "0") {
//                        operatorSpread.push_back(grid[x][y-1].isOperator);
//                    }
//                }
//                if (x != grid.size()-1) {
//                    if (grid[x+1][y].value != "0") {
//                        operatorSpread.push_back(grid[x+1][y].isOperator);
//                    }
//                }
//                if (y != grid[x].size()-1) {
//                    if (grid[x][y+1].value != "0") {
//                        operatorSpread.push_back(grid[x][y+1].isOperator);
//                    }
//                }
//                //operatorSpread is now full of the surrounding isOperator bools

//                double yCount = operatorSpread.count(true);
//                if (yCount != 0) {
//                    yCount = yCount/operatorSpread.length();
//                    if (yCount > 0.5) {
//                        //Create a operand MathNode
//                        grid[x][y].value = GenerateMathNode(false);
//                        grid[x][y].isOperator = false;
//                    } else if (yCount < 0.5) {
//                        //Create a operator MathNode
//                        grid[x][y].value = GenerateMathNode(true);
//                        grid[x][y].isOperator = true;
//                    } else {
//                        //Create a random MathNode
//                        if (qrand() % 2 == 0) {
//                            grid[x][y].value = GenerateMathNode(false);
//                            grid[x][y].isOperator = false;
//                        } else {
//                            grid[x][y].value = GenerateMathNode(true);
//                            grid[x][y].isOperator = true;
//                        }
//                    }
//                } else {
//                    //Create a random MathNode
//                    if (qrand() % 2 == 0) {
//                        grid[x][y].value = GenerateMathNode(false);
//                        grid[x][y].isOperator = false;
//                    } else {
//                        grid[x][y].value = GenerateMathNode(true);
//                        grid[x][y].isOperator = true;
//                    }
//                }

//            }
//        }
//    }
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
