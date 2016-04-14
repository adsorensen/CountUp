#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "mathnode.h"
#include <QString>
#include <QVector>
#include <QPair>


class GameModel
{
public:
    GameModel();
    ~GameModel();
    int targetNum;
    int currentNum;
    int mBombCounter;
    int movesRemaining;
    int difficulty;

    MathNode grid[10][10];

    int FormulaReader(QString);
    bool FormulaCheck(QString);
    void ShuffleGrid();
    void BombGrid(QString);
    void RemoveNode(QVector<QPair<int, int> >);
    void PopulateGrid();
    QString GenerateMathNode(bool);
};

#endif // GAMEMODEL_H
