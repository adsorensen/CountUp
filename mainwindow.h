/**
* Main view controller for the CountUp game
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gamemodel.h>
#include <mathnode.h>
#include <QMouseEvent>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>
#include <QTimerEvent>


namespace Ui {
class MainWindow;
}

enum {
    BallObject,
    WallObject,
};

struct Object
{
    int type;
    int index;
    b2Body *body;
    b2Fixture *fixture;
    QString numberValue;
    QString operatorValue;
    float32 column;
    float32 row;
    QColor color;
};

struct ObjectInfo
{
    Object thisObject;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    b2World* World;
    b2Body* groundBody;
    int _timerId = 0;
    QTransform _transform;
    QVector<Object> _objects;
    QVector<Object> walls;
    QVector<QPair<int, int>> coordinates;
    bool begin;
    bool gameStarted;

public:
    GameModel game;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fillGrid();
    void start();
    int difficulty;
    int level;

private slots:
    void on_tableWidget_cellClicked(int row, int column);
    void on_tableWidget_cellEntered(int row, int column);
    bool eventFilter(QObject *obj, QEvent *event);
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_bombButton_pressed();
    void on_shuffleButton_pressed();

public slots:
    void dealWithInvalidFormula();
    void dealWithCompletedLevel();
    void gameOver();
    void nextMove(int movesRemaining, int currentNum);
    void dealWithNewBubble(int, int);
    void removeBubbles(QVector<QPair<int, int>>);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *);
    Object createWall(float32 x, float32 y, float32 w, float32 h, float32 angle);
    Object createBall(const b2Vec2& pos, float32 radius);
    Object createBall(const b2Vec2& pos, float32 radius, int index);
    Object createBall(const b2Vec2& pos, float32 radius, int index, MathNode mn);
    void drawWall(QPainter *p, const Object& o);
    void drawEllipse(QPainter *p, const Object& o);
    void createBalls();
    void createWalls();
    QColor generateColor(MathNode);
    void removeBallAt(float32 column, float32 row);
    void removeBallAt(float32 column, float32 row, int delayVal);
    void spawnBallAt(float32 column, int index);
    void spawnBallAt(float32 column, int index, MathNode mn);
    int getIndex(int column, int row);
    float32 radius = 34.4f;
    void updateIndex(int index);
    void delay(int millisecondsToWait);
    void checkIntegrity();

signals:
    void current_positions(QVector<QPair<int, int>>);
};

#endif // MAINWINDOW_H
