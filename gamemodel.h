#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "mathnode.h"
#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>
#include <QStack>
#include "level.h"
#include <QObject>


class GameModel : public QObject
{
    Q_OBJECT

public:
    GameModel();
    ~GameModel();
    int levelNum;
    int targetNum;
    int currentNum;
    int mBombCounter;
    int mul2BombCounter;
    int mul4BombCounter;
    int div2BombCounter;
    int movesRemaining;
    int difficulty;

    QVector<QVector<MathNode> > grid;
    QMap<int, Level*> levelMap;

    int FormulaReader(QVector<QString>);
    bool FormulaCheck(QVector<bool>);
    void ShuffleGrid();
    void BombGrid(QString);
    void RemoveNode(QVector<QPair<int, int> >);
    void PopulateGrid();
    void CheckWin();
    QString GenerateMathNode(bool);
public slots:
    void LevelStart(int);
    void OnMove(QVector<QPair<int,int> >);
signals:
    void InvalidFormulaSig();
    void LevelCompletedSig();\
    void GameOverSig();
    void ContinueLevelSig(int, int);
};

#endif // GAMEMODEL_H
