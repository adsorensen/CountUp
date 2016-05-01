/**
* Logical backend of the Number Crunch game
*
* CS3505, April 2016.
* Team: It's Lit (fire, fire, fire)
*
* @author Adam Sorensen
*         Connor Douglas
*         Johnny Le
*         Michelle Nguyen
*         Steven Sun
*         Trung Le
*         Yu Song
*         Zhi Wong
*/

#include "gamemodel.h"
#include <QDebug>
#include <ctype.h>

/*
 * Constructor
 */
GameModel::GameModel()
{
    // Initializes the backend "grid" to hold MathNodes
    for (int u = 0; u < 8; u++)
    {
        grid.push_back( QVector<MathNode>());
    }

    // Adds a new MathNode to each index of the grid array
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            MathNode mn;
            grid[i].push_back(mn);
        }
    }

    // Create the level mappings
    mapLevels();
}

/*
 * Destructor
 */
GameModel::~GameModel()
{
}

/*
 * Returns true if the formula is valid, false if otherwise
 */
bool GameModel::FormulaCheck(QVector<bool> boolList)
{
    // Check if passed in QVector of booleans is false at index 0
    if (!boolList[0])
    {
        // Sets lastBool to the boolean at index 0 of boolList
        bool lastBool = boolList[0];

        // Check is passed in boolList has a size greater than or equal to 3
        if (boolList.length() >= 3)
        {
            // Check each boolean in boolList again lastBool
            for (int i = 1; i < boolList.length(); i++)
            {
                if ((lastBool && boolList[i]) || (!lastBool && !boolList[i]))
                {
                    return false;
                }

                lastBool = boolList[i];
            }
            // Return false if last item of boolList is true
            if (boolList[boolList.length()-1])
            {
                return false;
            }
            // Otherwise, return true
            else
            {
                return true;
            }
        }
        // Return false if the passed in boolList has a size less than 3
        else
        {
            return false;
        }
    }

    // Else return true if boolList[0] is true
    return false;
}

/*
 * Reads in a String, determines whether each character is an operator or operand,
 * returns the calculated result of the passed in formula
 */
int GameModel::FormulaReader(QVector<QString> formula)
{
    // Defines variables
    QStack<QString> operators;
    QStack<QString> operands;
    int firstOperand;
    int secondOperand;

    // Deals with order of operations
    for (int i = 0; i < formula.length(); i++)
    {
        if (formula[i].toInt() != 0)
        {
            if (!operators.isEmpty())
            {
                if (operators.top() == "*")
                {
                    operands.push(QString::number(operands.pop().toInt() * formula[i].toInt()));

                    operators.pop();
                } else if (operators.top() == "/")
                {
                    operands.push(QString::number(operands.pop().toInt() / formula[i].toInt()));
                    operators.pop();
                } else
                {
                    operands.push(formula[i]);
                }
            } else
            {
                operands.push(formula[i]);
            }
        }
        else if (formula[i] == "+" or formula[i] == "-")
        {
            if (!operators.isEmpty()) {
                if (operators.top() == "+") {
                    secondOperand = operands.pop().toInt();
                    firstOperand = operands.pop().toInt();
                    operands.push(QString::number(firstOperand + secondOperand));
                    operators.pop();

                }
                else if (operators.top()  == "-")
                {
                    secondOperand = operands.pop().toInt();
                    firstOperand = operands.pop().toInt();

                    operands.push(QString::number(firstOperand- secondOperand));
                    operators.pop();
                } else
                {
                    operators.push(formula[i]);
                }
            } else
            {
                operators.push(formula[i]);
            }
        }
        else
            operators.push(formula[i]);
    }
    if (!operators.isEmpty())
    {
        if (operators.top() == "+")
        {
            secondOperand = operands.pop().toInt();
            firstOperand = operands.pop().toInt();
            operands.push(QString::number(firstOperand + secondOperand));
            operators.pop();
        }
        else if (operators.top()  == "-")
        {
            secondOperand = operands.pop().toInt();
            firstOperand = operands.pop().toInt();
            operands.push(QString::number(firstOperand- secondOperand));
            operators.pop();
        }
    }

    // Returns the calculated value
    return operands.pop().toInt();
}

/*
 * Handles backend when user shuffles grid to prevent stale games
 */
void GameModel::ShuffleGrid()
{
    //Sets the value of all grid values to 0
    ClearGrid();

    //Repopulates the grid
    PopulateGrid();

    //Decrements the moves number by one
    movesRemaining--;

    // If there are no more moves remaining, emit a GaveOverSig() signal
    if (movesRemaining <= 0)
    {
        emit GameOverSig();
    }
}

/*
 * Handles backend when user presses a bomb button, the passed in int refers to
 * a specific bomb where 1 is the mul2Bomb, 2 represents mul4Bomb, 3 represents
 * div2Bomb, and 4 represents the mBomb (modulus bomb)
 */
void GameModel::BombGrid(int bombOp)
{
    switch (bombOp)
    {
    // Handle mul2Bomb logic
    case 1:
        if  (mul2BombCounter > 0)
        {
            qDebug() << "trying to multiply by two";
            for (int i = 0; i < grid.length(); i++)
            {
                for (int j = 0; j < grid[i].length(); j++)
                {
                    if (!grid[i][j].isOperator)
                    {
                        grid[i][j].value = QString::number(grid[i][j].value.toInt() * 2);
                        if (grid[i][j].value == "0")
                        {
                            grid[i][j].value = "1";
                        }
                    }
                }
            }
            mul2BombCounter--;
        }
        emit BombUsed(bombOp, mul2BombCounter);
        break;

    // Handle mul4Bomb login
    case 2:
        if (mul4BombCounter > 0)
        {
            for (int i = 0; i < grid.length(); i++)
            {
                for (int j = 0; j < grid[i].length(); j++)
                {
                    if (!grid[i][j].isOperator) {
                        grid[i][j].value = QString::number(grid[i][j].value.toInt() * 4);
                        if (grid[i][j].value == "0")
                        {
                            grid[i][j].value = "1";
                        }
                    }
                }
            }
            mul4BombCounter--;
        }
        emit BombUsed(bombOp, mul4BombCounter);
        break;

    // Handle div2Bomb logic
    case 3:
        if (div2BombCounter > 0) {
            for (int i = 0; i < grid.length(); i++)
            {
                for (int j = 0; j < grid[i].length(); j++)
                {
                    if (!grid[i][j].isOperator)
                    {
                        grid[i][j].value = QString::number((int)(grid[i][j].value.toInt() / 2));
                        if (grid[i][j].value == "0")
                        {
                            grid[i][j].value = "1";
                        }
                    }
                }
            }
            div2BombCounter--;
        }
        emit BombUsed(bombOp, div2BombCounter);
        break;

    // Handle mBomb logic
    case 4:
        if (mBombCounter > 0)
        {
            for (int i = 0; i < grid.length(); i++)
            {
                for (int j = 0; j < grid[i].length(); j++)
                {
                    if (!grid[i][j].isOperator)
                    {
                        grid[i][j].value = QString::number(grid[i][j].value.toInt() % 10);
                        if (grid[i][j].value == "0")
                        {
                            grid[i][j].value = "1";
                        }
                    }
                }
            }
            mBombCounter--;
        }
        emit BombUsed(bombOp, mBombCounter);
        break;
    }
}

/*
 * Removes nodes from the backend that were passed in as a parameter
 */
void GameModel::RemoveNode(QVector<QPair<int, int> > removedNodes)
{
     // Set the grid at the removedNodes to represent an empty slot
    for (int i = 0; i < removedNodes.length(); i++)
    {
        grid[removedNodes[i].first][removedNodes[i].second].isSelected = false;
        grid[removedNodes[i].first][removedNodes[i].second].isOperator = false;
        grid[removedNodes[i].first][removedNodes[i].second].value = "0";
    }

    // Go through the grid and check for empty slots
    for (int x = 0; x < grid.size(); x++)
    {
        int emptyCount = 0;
        for (int y = 0; y < grid[x].size(); y++)
        {
            if (grid[x][y].value == "0")
            {
                emptyCount++;
            }
        }

        // If there are empty slots, rearrange nodes in the grid to fill them
        if (emptyCount > 0) {
            for (int y = grid[x].size()-1; y >= 0; y--)
            {
                // Go through y column from the bottom up
                if (grid[x][y].value == "0")
                {
                    int curNode = y;

                    // Swap the MathNode with the empty one
                    for (int i = y; i >= 0; i--)
                    {
                        if (grid[x][i].value != "0")
                        {
                            MathNode temp = grid[x][i];
                            grid[x][i] = grid[x][curNode];
                            grid[x][curNode] = temp;
                            curNode = i;
                        }
                    }
                }
            }
        }
    }
}

/*
 * Populates the grid QVector and returns a QVector of coordinates
 */
QVector<QPair<int, int>> GameModel::PopulateGrid()
{
    // Declare variables
    QVector<QPair<int, int>> totalCoords;
    QPair<int, int> coords;

    // Goes through grid and algorithmically populates it with MathNodes
    for (int x = 0; x < grid.size(); x++)
    {
        for (int y = 0; y < grid[x].size(); y++)
        {
            if (grid[x][y].value == "0")
            {
                // Node is empty
                QVector<bool> operatorSpread;
                if (x != 0) {
                    if (grid[x-1][y].isOperator)
                    {
                        operatorSpread.push_back(grid[x-1][y].isOperator);
                    }
                }
                if (y != 0) {
                    if (grid[x][y-1].isOperator)
                    {
                        operatorSpread.push_back(grid[x][y-1].isOperator);
                    }
                }
                if (x != grid.size()-1)
                {
                    if (grid[x+1][y].isOperator)
                    {
                        operatorSpread.push_back(grid[x+1][y].isOperator);
                    }
                }
                if (y != grid[x].size()-1)
                {
                    if (grid[x][y+1].isOperator)
                    {
                        operatorSpread.push_back(grid[x][y+1].isOperator);
                    }
                }

                // operatorSpread is now full of the surrounding isOperator bools
                double yCount = operatorSpread.count(true);
                if (yCount != 0)
                {
                    yCount = yCount/operatorSpread.length();
                    if (yCount > 0.5)
                    {
                        // Create an operand MathNode
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                    }
                    else if (yCount < 0.5)
                    {
                        // Create an operator MathNode
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                    }
                    else
                    {
                        // Create a random MathNode
                        if (qrand() % 2 == 0)
                        {
                            grid[x][y].value = GenerateMathNode(false);
                            grid[x][y].isOperator = false;
                        }
                        else
                        {
                            grid[x][y].value = GenerateMathNode(true);
                            grid[x][y].isOperator = true;
                        }
                    }
                }
                else
                {
                    // Create a random MathNode
                    if (qrand() % 2 == 0)
                    {
                        grid[x][y].value = GenerateMathNode(false);
                        grid[x][y].isOperator = false;
                    } else
                    {
                        grid[x][y].value = GenerateMathNode(true);
                        grid[x][y].isOperator = true;
                    }
                }
                // Set values to the coords QPair
                coords.first = x;
                coords.second = y;

                // Add these coordinates to totalCoords QVector
                totalCoords.append(coords);
            }
        }
    }
    // Return totalCoords QVector
    return totalCoords;
}

/*
 * Sets all values in the grid to 0
 */
void GameModel::ClearGrid()
{
    for (int x = 0; x < grid.size(); x++)
    {
        for (int y = 0; y < grid[x].size(); y++)
        {
            grid[x][y].value = "0";
        }
    }
}

/*
 * Get called every time player finishes a move
 */
void GameModel::CheckWin()
{
    // Enter if condition if the user's current total equals the level target number
    if (currentNum == targetNum)
    {
        // Update the user's high score in the database
        network.updateHighscore(name, QString::number(levelNum), QString::number(difficulty), QString::number(movesRemaining));

        // Emit a LevelCompletedSig signal
        emit LevelCompletedSig();
    }
    // Otherwise emit ContinueLevelSig signal
    else
    {
        // Pass movesRemaining and currentNum to the signal
        emit ContinueLevelSig(movesRemaining, currentNum);
    }
}

/*
 * Returns a generate value for a MathNode based on difficulty
 */
QString GameModel::GenerateMathNode(bool isOperator)
{
    // Declare variables
    QString value = "";
    int num;

    // Enter if condition if passed in isOperator is false
    if (!isOperator)
    {
        switch (difficulty)
        {
        // MathNode generation for easy level
        case 1:
            num = qrand() % 5 + 1;
            if ((qrand() % 2) == 0)
            {
                value = "-" + QString::number(num);
            } else
            {
                value = QString::number(num);
            }
            break;

        // MathNode generation for medium level
        case 2:
            num = qrand() % 10 + 1;
            if ((qrand() % 2) == 0)
            {
                value = "-" + QString::number(num);
            } else
            {
                value = QString::number(num);
            }
            break;

        // MathNode generation for hard level
        case 3:
            num = qrand() % 20 + 1;
            if ((qrand() % 2) == 0)
            {
                value = "-" + QString::number(num);
            } else
            {
                value = QString::number(num);
            }
            break;

        // MathNode generation for very hard level
        case 4:
            num = qrand() % 50 + 1;
            if ((qrand() % 2) == 0)
            {
                value = "-" + QString::number(num);
            } else
            {
                value = QString::number(num);
            }
            break;

        // MathNode generation for extreme level
        case 5:
            num = qrand() % 100 + 1;
            if ((qrand() % 2) == 0)
            {
                value = "-" + QString::number(num);
            } else
            {
                value = QString::number(num);
            }
            break;
        }
    }

    // Deal with generating operator MathNodes
    else
    {
        int num;
        switch (difficulty)
        {
        // Easy level
        case 1:
            num = qrand() % 2;
            break;

        // Medium level
        case 2:
            num = qrand() % 3;
            break;

        // Hard level
        case 3:
            num = qrand() % 4;
            break;

        // Very hard level
        case 4:
            num = qrand() % 3 + 1;
            break;

        // Extreme level
        case 5:
            num = qrand() % 2 + 2;
            break;
        }
        switch (num)
        {
        // Set addition as operator MathNode value
        case 0:
            value = "+";
            break;

        // Set subtraction as operator MathNode value
        case 1:
            value = "-";
            break;

        // Set multiplication as operator MathNode value
        case 2:
            value = "*";
            break;

        // Set division as operator MathNode value
        case 3:
            value = "/";
            break;
        }
    }

    // Return the value of the generated MathNode
    return value;
}

/*
 * Initializes properties at the beginning of each level based on passed in
 * level number and difficulty number
 */
void GameModel::LevelStart(int lNum, int diffNum)
{
    // Initialize properties of the game
    levelNum = lNum;
    difficulty = diffNum;
    int currLevel = ((difficulty - 1) * 5) + levelNum;
    targetNum = levelMap[currLevel]->targetNum;
    currentNum = 0;
    mBombCounter = levelMap[currLevel]->moduloBomb;
    mul2BombCounter = levelMap[currLevel]->multiplyTwoBomb;
    mul4BombCounter = levelMap[currLevel]->multiplyFourBomb;
    div2BombCounter = levelMap[currLevel]->divideTwoBomb;
    movesRemaining = levelMap[currLevel]->movesNum;

    // Emit signals
    emit BombUsed(1, mul2BombCounter);
    emit BombUsed(2, mul4BombCounter);
    emit BombUsed(3, div2BombCounter);
    emit BombUsed(4, mBombCounter);

    // Populate the grid
    PopulateGrid();
}

/*
 * Handles game logic after user makes a move
 */
void GameModel::OnMove(QVector<QPair<int, int> > cellList)
{
    // Declare variables
    QVector<QString> formula;
    QVector<bool> boolList;

    // Sets boolList based on whether passed in cellList contains operators
    for (auto i = 0; i < cellList.length(); i++)
    {
        if (grid[cellList[i].first][cellList[i].second].isOperator)
        {
            boolList.push_back(true);
        }
        else
        {
            boolList.push_back(false);
        }
        // Update the formula QVector
        formula.push_back(grid[cellList[i].first][cellList[i].second].value);
    }

    // Enter if condition if user's formula is valid
    if (FormulaCheck(boolList))
    {
        // Save formula result into result
        int result = FormulaReader(formula);

        // Emit the result
        emit sendResult(result);

        // Update the current total
        currentNum += result;

        qDebug() << "result = " << result;
        qDebug() << "current number = " << currentNum;

        // Decrement number of moves remaining
        movesRemaining--;

        // Remove the cells from the game model
        RemoveNode(cellList);

        // Create new nodes
        QVector<QPair<int, int>> newNodes = PopulateGrid();

        // Pass the user's cellList and newNodes through the RemoveBubblesAtSig
        // signal
        emit RemoveBubblesAtSig(cellList, newNodes);

        // Check if the user has won the level
        CheckWin();

        // Emit GameOverSig signal if user is out of remaining moves
        if (movesRemaining == 0)
        {
            emit GameOverSig();
        }
    }
    // Otherwise, enter the else condition
    else
    {
        // Emit InvalidFormulaSig signal
        emit InvalidFormulaSig();
    }
}

/*
 * Sets the username of the game model to the passed in QString
 */
void GameModel::getUsername(QString username)
{
    name = username;
}

/*
 * Sets up properties for each level (1-25) including level number, target goal,
 * total moves per level, amounts of shuffles allowed per level along with times
 * 2 bombs, times 4 boms, and div 2 bombs available
 */
void GameModel::mapLevels() {
    levelMap[1] = new Level(1, 12, 4, 0, 1, 1, 0, 0);
    levelMap[2] = new Level(2, 25, 6, 0, 1, 1, 0, 0);
    levelMap[3] = new Level(3, 32, 7, 0, 1, 1, 0, 0);
    levelMap[4] = new Level(4, 46, 8, 0, 1, 1, 0, 0);
    levelMap[5] = new Level(5, 94, 12, 0, 0, 0, 0, 1);
    levelMap[6] = new Level(6, 124, 10, 0, 1, 1, 1, 0);
    levelMap[7] = new Level(7, 257, 20, 0, 2, 2, 1, 0);
    levelMap[8] = new Level(8, 432, 20, 0, 0, 2, 1, 0);
    levelMap[9] = new Level(9, 166, 10, 0, 2, 0, 1, 0);
    levelMap[10] = new Level(10, 512, 25, 1, 1, 1, 1, 0);
    levelMap[11] = new Level(11, 111, 4, 1, 1, 1, 1, 1);
    levelMap[12] = new Level(12, 529, 12, 1, 2, 2, 1, 1);
    levelMap[13] = new Level(13, 1023, 25, 3, 3, 1, 3, 1);
    levelMap[14] = new Level(14, 105, 3, 1, 0, 0, 1, 1);
    levelMap[15] = new Level(15, 2019, 25, 2, 0, 0, 0, 2);
    levelMap[16] = new Level(16, 1504, 15, 1, 1, 1, 1, 1);
    levelMap[17] = new Level(17, 57, 3, 0, 0, 0, 0, 0);
    levelMap[18] = new Level(18, 782, 10, 1, 0, 0, 0, 1);
    levelMap[19] = new Level(19, 7820, 25, 2, 2, 2, 2, 0);
    levelMap[20] = new Level(20, 322, 10, 0, 0, 2, 2, 0);
    levelMap[21] = new Level(21, 627, 10, 1, 0, 0, 0, 0);
    levelMap[22] = new Level(22, 102, 5, 0, 0, 1, 1, 0);
    levelMap[23] = new Level(23, 8891, 20, 2, 2, 2, 2, 2);
    levelMap[24] = new Level(24, 15029, 30, 2, 0, 0, 0, 5);
    levelMap[25] = new Level(25, 40000, 40, 1, 1, 1, 1, 1);
}
