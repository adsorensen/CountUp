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
    QVector<QPair<int, int>> ballstoadd;

public:
    GameModel game;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void fillGrid();
    void start();
    int difficulty;
    int level;

private slots:
    bool eventFilter(QObject *obj, QEvent *event);
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_shuffleButton_pressed();
    void on_exitButton_clicked();
    void on_retryButton_clicked();
    void on_menuButton_clicked();
    void on_modBomb_pressed();
    void on_div2Bomb_pressed();
    void on_mul2Bomb_pressed();
    void on_mul4Bomb_pressed();

public slots:
    void dealWithInvalidFormula();
    void dealWithCompletedLevel();
    void gameOver();
    void nextMove(int movesRemaining, int currentNum);
    void removeBubbles(QVector<QPair<int, int>>,  QVector<QPair<int, int>> );
    void displayFormulaResult(int);
    void dealWithBombOp(int, int);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *);
    Object createWall(float32 x, float32 y, float32 w, float32 h, float32 angle);
    Object createBall(const b2Vec2& pos, float32 radius);
    Object createBall(const b2Vec2& pos, float32 radius, MathNode mn);
    void drawWall(QPainter *p, const Object& o);
    void drawEllipse(QPainter *p, const Object& o);
    void createBalls();
    void createWalls();
    QColor generateColor(MathNode);
    void removeBallAt(int index);
    int getIndex(int column, int row);
    float32 radius = 34.4f;
    void updateIndex(int index);
    void delay(int millisecondsToWait);
    void checkIntegrity();
    void clearLabels();
    void showCongratsScreen();

signals:
    void current_positions(QVector<QPair<int, int>>);
    void menu_pressed();
};

#endif // MAINWINDOW_H
