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

//    //f/*or(int i = 0; i < grid.size(); i++)
//    {
//        QVector<MathNode> temp = grid.at(i);

//        for(int j = 0; j < temp.size(); j++)
//        {
//            qDebug() << temp.at(j).value;
//            //qDebug() << index;
//        }
//    }*/

    for (int i = 0; i < 25; i++) {
        levelMap[i] = new Level(i,i,i,i,i,i,i,i);
    }
}



GameModel::~GameModel()
{
    //Destructor
}

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
    QStack<QString> operators;
    QStack<QString> operands;

    int totalVal = 0;
    int firstOperand;
    int secondOperand;

    for (int i = 0; i < formula.length(); i++) {
        if (formula[i].toInt() != 0) {
            qDebug() << formula[i].toInt();
            if (!operators.isEmpty()) {
                if (operators.top() == "*") {
                    operands.push(QString::number(operands.pop().toInt() * formula[i].toInt()));
                    operators.pop();
                } else if (operators.top() == "/") {
                    operands.push(QString::number(operands.pop().toInt() / formula[i].toInt()));
                    operators.pop();
                } else {
                    operands.push(formula[i]);
                }
            } else {
                operands.push(formula[i]);
            }
        }
        else if (formula[i] == "+" or formula[i] == "-") {
            if (!operators.isEmpty()) {
                if (operators.top() == "+") {
                     secondOperand = operands.pop().toInt();
                     firstOperand = operands.pop().toInt();
                     operands.push(QString::number(firstOperand + secondOperand));
                     operators.pop();

                }
                else if (operators.top()  == "-") {
                    secondOperand = operands.pop().toInt();
                    firstOperand = operands.pop().toInt();

                    operands.push(QString::number(firstOperand- secondOperand));
                    operators.pop();
                } else {
                    operators.push(formula[i]);
                }
            } else {
                operators.push(formula[i]);
            }
        }
        else
            operators.push(formula[i]);
    }


    if (!operators.isEmpty()) {
        qDebug() << "operators was not empty";
        if (operators.top() == "+") {
            qDebug() << operators.count();
            qDebug() << operands.count();


             secondOperand = operands.pop().toInt();
             firstOperand = operands.pop().toInt();

             operands.push(QString::number(firstOperand + secondOperand));
             qDebug() << QString::number(firstOperand + secondOperand);
             operators.pop();

        }
        else if (operators.top()  == "-") {
            secondOperand = operands.pop().toInt();
            firstOperand = operands.pop().toInt();

            operands.push(QString::number(firstOperand- secondOperand));
            operators.pop();
        }
    }

    return operands.pop().toInt();
}

void GameModel::ShuffleGrid()
{
    //Shuffles grid to prevent stale games

    //Sets the value of all grid values to 0
    ClearGrid();
    //Repopulates the grid
    PopulateGrid();

    //Decriments the turn number by one
    movesRemaining--;

    if (movesRemaining <= 0) {
        emit GameOverSig();
    }
}

void GameModel::BombGrid(int bombOp)
{
    //Applies operation bombOp on all operands in grid
    switch (bombOp) {
    case 1: // Multiply by 2
        for (int i = 0; i < grid.length(); i++) {
            for (int j = 0; j < grid[i].length(); j++) {
                if (!grid[i][j].isOperator) {
                    grid[i][j].value = grid[i][j].value.toInt() * 2;
                }
            }
        }
        break;
    case 2: //Multiply by 4
        for (int i = 0; i < grid.length(); i++) {
            for (int j = 0; j < grid[i].length(); j++) {
                if (!grid[i][j].isOperator) {
                    grid[i][j].value = grid[i][j].value.toInt() * 4;
                }
            }
        }
    case 3: //Divide by 2
        for (int i = 0; i < grid.length(); i++) {
            for (int j = 0; j < grid[i].length(); j++) {
                if (!grid[i][j].isOperator) {
                    grid[i][j].value = (int)(grid[i][j].value.toInt() / 2 + 0.5);
                }
            }
        }
        break;
    case 4: //Modulo by 10
        for (int i = 0; i < grid.length(); i++) {
            for (int j = 0; j < grid[i].length(); j++) {
                if (!grid[i][j].isOperator) {
                    grid[i][j].value = grid[i][j].value.toInt() % 10;
                }
            }
        }
    }
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
                            curNode--;
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
                    if (grid[x-1][y].isOperator) {
                        operatorSpread.push_back(grid[x-1][y].isOperator);
                    }
                }
                if (y != 0) {
                    if (grid[x][y-1].isOperator) {
                        operatorSpread.push_back(grid[x][y-1].isOperator);
                    }
                }
                if (x != grid.size()-1) {
                    if (grid[x+1][y].isOperator) {
                        operatorSpread.push_back(grid[x+1][y].isOperator);
                    }
                }
                if (y != grid[x].size()-1) {
                    if (grid[x][y+1].isOperator) {
                        operatorSpread.push_back(grid[x][y+1].isOperator);
                    }
                }
                //operatorSpread is now full of the surrounding isOperator bools

                double yCount = operatorSpread.count(true);
                if (yCount != 0) {
                    yCount = yCount/operatorSpread.length();
                    if (yCount > 0.5) {
                        //Create a operand MathNode and emit signal for the views
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                        //emit CreateBubbleAtSig(x,y);
                    } else if (yCount < 0.5) {
                        //Create a operator MathNode and emit signal for the view
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                        //emit CreateBubbleAtSig(x,y);
                    } else {
                        //Create a random MathNode and emit signal for the view
                        if (qrand() % 2 == 0) {
                            grid[x][y].value = GenerateMathNode(false);
                            grid[x][y].isOperator = false;
                            //emit CreateBubbleAtSig(x,y);
                        } else {
                            grid[x][y].value = GenerateMathNode(true);
                            grid[x][y].isOperator = true;
                            //emit CreateBubbleAtSig(x,y);
                        }
                    }
                } else {
                    //Create a random MathNode and emit signal for the view
                    if (qrand() % 2 == 0) {
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                        //emit CreateBubbleAtSig(x,y);
                    } else {
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                        //emit CreateBubbleAtSig(x,y);
                    }
                }

            }
        }
    }
}

void GameModel::ClearGrid() {
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid[x].size(); y++) {
            grid[x][y].value = "0";
        }
    }
}

void GameModel::CheckWin() {
    if (currentNum == targetNum) {
        network.updateHighscore(username, QString::number(levelNum), QString::number(difficulty), QString(movesRemaining) );
        emit LevelCompletedSig();
    }
    else {
        emit ContinueLevelSig(movesRemaining, currentNum);
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
                ret = "-" + QString::number(num);
            } else {
                ret = QString::number(num);
            }
            break;
        case 2: //Medium
            num = qrand() % 10 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + QString::number(num);
            } else {
                ret = QString::number(num);
            }
            break;
        case 3: //Hard
            num = qrand() % 20 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + QString::number(num);
            } else {
                ret = QString::number(num);
            }
            break;
        case 4: //Very Hard
            num = qrand() % 50 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + QString::number(num);
            } else {
                ret = QString::number(num);
            }
            break;
        case 5: //Extreme
            num = qrand() % 100 + 1;
            if ((qrand() % 2) == 0) {
                ret = "-" + QString::number(num);
            } else {
                ret = QString::number(num);
            }
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
    return ret;
}

void GameModel::LevelStart(int lNum, int diffNum)
{
    levelNum = lNum;
    difficulty = diffNum;
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
        movesRemaining--;
        RemoveNode(cellList);

        emit RemoveBubblesAtSig(cellList);
        if (movesRemaining == 0) {
            emit GameOverSig();
        }
        //TODO: Check win-state and proceed appropriately
        CheckWin();
    } else {
        //invalid formula
        emit InvalidFormulaSig();
    }
}
