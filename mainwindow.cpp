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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Box2D/Box2D.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QDebug>
#include <QRadialGradient>
#include <QMouseEvent>
#include <QtGui>
#include <QPixmap>
#include <QAbstractItemView>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>
#include <QtOpenGL>
#include <QTimerEvent>

/*
 * Constructor
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // Centers application on start up
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    ui->setupUi(this);
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->show();

    // UI setup
    ui->gameOverBox->hide();
    ui->congratsBox->hide();

    // Sets the background image
    this->setStyleSheet("MainWindow {border-image: url(:/background/Resources/bg.png); };");

    // Initializes the game "grid"
    ui->tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setMouseTracking(true);
    ui->tableWidget->viewport()->setMouseTracking(true);
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->viewport()->installEventFilter(this);

    // Connect slots and signals
    QObject::connect(&game, SIGNAL(InvalidFormulaSig()), this, SLOT(dealWithInvalidFormula()));
    QObject::connect(&game, SIGNAL(LevelCompletedSig()), this, SLOT(dealWithCompletedLevel()));
    QObject::connect(&game, SIGNAL(GameOverSig()), this, SLOT(gameOver()));
    QObject::connect(&game, SIGNAL(ContinueLevelSig(int, int)), this, SLOT(nextMove(int, int)));
    QObject::connect(&game, SIGNAL(RemoveBubblesAtSig(QVector<QPair<int,int> >, QVector<QPair<int, int>>)), this, SLOT(removeBubbles(QVector<QPair<int,int>>, QVector<QPair<int, int>>)));
    QObject::connect(&game, SIGNAL(sendResult(int)), this, SLOT(displayFormulaResult(int)));
    QObject::connect(&game, SIGNAL(BombUsed(int,int)), this, SLOT(dealWithBombOp(int,int)));

    // Setup of Box2d with world and walls
    b2Vec2 gravity(0.0f, 1000.0f);
    World = new b2World(gravity);
    createWalls();
}

/*
 * Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete World;
}

/*
 * Sets value of the cell according to model
 */
void MainWindow::fillGrid()
{
    // Declare and initialize variables
    int offsetX = 95.0f;
    int offsetY = 90.0f;

    // Repetitively fill grid based on game model and append to _objects QVector
    for(int i = 0; i < game.grid.size(); i++)
    {
        for(int j = 0; j < game.grid[i].size(); j++)
        {
            int dy = offsetY + i * 70;
            int dx = offsetX + j * 70;
            MathNode mn = game.grid[j][i];
            _objects.append(createBall(b2Vec2(dx, dy), radius, mn));
            qDebug()<<"test";
        }
    }
    qDebug()<<"test2";

    /*for(int i = 0; i < game.grid.size(); i++)
    {
        qDebug() << "here5";
        for(int j = 0; j < game.grid.at(i).size(); j++)
        {
            MathNode mn = game.grid[i][j];
            qDebug()  << "column: " << i << " row: " << j << " value: " << mn.value;
        }
    }*/
}

/*
 * Creates a singular object of type ball for the Box2D world that takes in a position and radius
 */
Object MainWindow::createBall(const b2Vec2& pos, float32 radius)
{
    Object o;

    // Define the body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);

    // Define the shape
    b2CircleShape shape;
    shape.m_radius = radius;

    // Define the mass
    for(int i = 0; i < 2; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);
    }

    // Set properties of the ball
    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;

    // Return the ball
    return o;
}

/*
 * Creates a singular object of type ball for the Box2D world that takes in a position, radius, and MathNode
 */
Object MainWindow::createBall(const b2Vec2& pos, float32 radius, MathNode mn)
{
    Object o;

    // Define the body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    qDebug()<< "Crashes here!!! On 53rd drawn ball";
    qDebug() << "Body count:" << World->GetBodyCount();
    o.body = World->CreateBody(&bd);

    // Define the shape
    qDebug()<< "2";
    b2CircleShape shape;
    qDebug()<< "3";
    shape.m_radius = radius;
    qDebug()<< "4";
    // Define the mass
    for(int i = 0; i < 2; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);
    }

    // Set properties of the ball
    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;
    o.color = generateColor(mn);
    o.numberValue = mn.value;

    // Return the ball
    return o;
}

/*
 * Creates multiple balls and adds them to the _objects QVector
 */
void MainWindow::createBalls()
{
    // Define and initialize variables
    int offsetX = 95.0f;
    int offsetY = 90.0f;

    // Repetively creates and adds balls to the QVector
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int dx = offsetX + j*70;
            int dy = offsetY + i*70;
            _objects.append(createBall(b2Vec2(dx, dy), radius));
        }
    }
}

/*
 * Creates Box2D walls and appends them to the walls QVector
 */
void MainWindow::createWalls()
{
    // Outer edges
    walls.append(createWall(60.0f, 615.0f, 560.0f, 1.0f, 0.0)); //ground
    walls.append(createWall(60.0f, 55.0f, 1.0f, 560.0f, 0.0));  //left border
    walls.append(createWall(620.0f, 55.0f, 1.0f, 560.0f, 0.0));  //right border

    // Funnel into the grid
    walls.append(createWall(-30.0f, 28.0f, 100.0f, 3.0f, -0.25f*b2_pi));
    walls.append(createWall(613.0f, 28.0f, 100.0f, 3.0f, 0.25f*b2_pi));

    // Inner borders
    walls.append(createWall(130.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(200.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(270.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(340.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(410.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(480.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(550.0f, 55.0f, 0.5f, 560.0f, 0.0));
}

/*
 * Paints walls and balls
 */
void MainWindow::paintEvent(QPaintEvent *)
{
    // Declare and initialize a QPainter
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);

    // Draw each ball in the _objects QVector
    foreach(const Object& o, _objects)
    {
        drawEllipse(&p, o);
    }

    // Draw each wall in the walls QVector
    foreach(const Object& o, walls)
    {
        drawWall(&p, o);
    }
}

/*
 * Detects mouse click events
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget->viewport())
    {
        // Detect mouse release even
        if (event->type() == QEvent::MouseButtonRelease)
        {
            // Sends formulas to the game model for evaluation
            for(int i = 0; i < coordinates.size(); i++)
            {
                MathNode mn = game.grid.at(coordinates.at(i).first).at(coordinates.at(i).second);
                qDebug() << "x:" <<coordinates.at(i).first << " y" << coordinates.at(i).second << "value: " << mn.value;
            }

            // If coordinates.size() > 0 call the game's OnMove method and clear
            // coordinates
            if(coordinates.size() > 0)
            {
                game.OnMove(coordinates);
                coordinates.clear();
            }
        }

        // Clear dynFormulaLabel on mouse press event
        if (event->type() == QEvent::MouseButtonPress)
        {
            ui->dynFormulaLabel->setText("");
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

/*
 * Draws the Box2D walls with a QPainter
 */
void MainWindow::drawWall(QPainter *p, const Object& o)
{
    // Declare and initialize variables
    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 angle = o.body->GetAngle();
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;
    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    // Call painter functions
    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->fillRect(r, Qt::white);
    p->restore();
}

/*
 * Creates Box2D walls
 */
Object MainWindow::createWall(float32 x, float32 y, float32 w, float32 h, float32 angle)
{
    Object o;

    // Define the body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = angle * b2_pi;
    o.body = World->CreateBody(&bd);

    // Define the shape
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);

    // Define the fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    o.fixture = o.body->CreateFixture(&fd);

    // Define the type
    o.type = WallObject;

    // Return the wall
    return o;
}

/*
 * Draws the Box2D balls
 */
void MainWindow::drawEllipse(QPainter *p, const Object& o)
{

    // Define the pen and brush
    QPen pen(Qt::black, 2);
    p->setPen(pen);
    QBrush brush(o.color);

    // Define positions
    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 r = o.fixture->GetShape()->m_radius;

    // Paint ellipses
    p->setBrush(brush);
    p->drawEllipse(QPointF(x, y), r, r);

    // Define the font
    QFont font;
    font.setPixelSize(20);

    // Define brush
    brush.setColor(Qt::black);

    // Paint text
    p->setBrush(brush);
    p->setFont(font);
    p->drawText(QPointF(x - 5, y + 5), o.numberValue);
}

/*
 * Gets called at the beginning of each level
 */
void MainWindow::start()
{
    // Define the timer
    _timerId = startTimer(1000/60);

    // Initialize labels
    clearLabels();

    qDebug() << "_objects currently has:" << _objects.count();
    qDebug() << "world currently has:" << World->GetBodyCount();

    // Destroy bodies in the world
    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    // Remove objects from the _objects QVector
    if (!_objects.isEmpty())
    {
        _objects.clear();
    }

     // Create a b2World
    b2Vec2 gravity(0.0f, 1000.0f); //normal earth gravity, 9.8 m/s/s straight down!
    World = new b2World(gravity);
    createWalls();

    // Initialize the game's level and difficulty
    game.LevelStart(level, difficulty);

    // Update the UI
    ui->dynTargetLabel->setText(QString::number(game.targetNum));
    ui->dynDifficultyLabel->setText(game.difficultyString);
    ui->dynLevelLabel->setText(QString::number(game.levelNum));
    ui->dynRemMovesLabel->setText(QString::number(game.movesRemaining));

    // Fill the grid
    fillGrid();
    qDebug()<<"initial";
    qDebug() << "_objects currently has (should be 64):" << _objects.count();
    qDebug() << "world currently has (76):" << World->GetBodyCount();

    // Enable widgets in the UI
    ui->toolBox->setEnabled(true);
    ui->tableWidget->setEnabled(true);
}

/*
 * Enforces 60 FPS
 */
void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == _timerId)
    {
        World->Step(1.0f/60.0f, 8, 3);
        update();
    }
}

/*
 * Takes in a MathNode and returns a color based on the node's ending value
 */
QColor MainWindow::generateColor(MathNode currentNode)
{
    if (currentNode.value.endsWith("1"))
    {
        return Qt::green;
    }
    if (currentNode.value.endsWith("2"))
    {
        return Qt::blue;
    }
    if (currentNode.value.endsWith("3"))
    {
        return Qt::yellow;
    }
    if (currentNode.value.endsWith("4"))
    {
        return Qt::red;
    }
    if (currentNode.value.endsWith("5"))
    {
        return Qt::cyan;
    }
    if (currentNode.value.endsWith("6"))
    {
        return Qt::magenta;
    }
    if (currentNode.value.endsWith("7"))
    {
        return QColor(237, 145, 33);
    }
    if (currentNode.value.endsWith("8"))
    {
        return QColor(159, 0, 197);
    }
    if (currentNode.value.endsWith("9"))
    {
        return QColor(222, 82, 133);
    }
    if (currentNode.value.endsWith("0"))
    {
        return QColor(51, 102, 0);
    }
    else
    {
        return Qt::white;
    }
}

/*
 * Handles when the current cell in the QTableWidget changes
 */
void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    // Declare a QPair<int, int>
    QPair<int, int> rowAndCol;

    // Don't register the default cell of (0, 0)
    if ((currentRow == 0 && currentColumn == 0) && ((previousRow == -1 &&  previousColumn == -1)))
    {
        ;
    }
    else
    {
        // Save the current column and row to the coordinates QVector
        rowAndCol.first = currentColumn;
        rowAndCol.second = currentRow;

        // Add these coordinates to the coordinates QVector
        coordinates.append(rowAndCol);

        // Save MathNode at current coordinates
        MathNode mn = game.grid[currentColumn][currentRow];

        // Update dynFormulaLabel's text
        QString text = ui->dynFormulaLabel->text();
        text += mn.value;
        ui->dynFormulaLabel->setText(text);
    }
}

/*
 * Removes the ball at the given index
 */
void MainWindow::removeBallAt(int index)
{
     // Create a b2Body object of the ball at the specified index
    b2Body *body = _objects.at(index).body;

    // Update the indices of balls in the grid
    updateIndex(index);

    // Destroy the body from our world
    World->DestroyBody(body);
}

/*
 * Returns the index of the ball
 */
int MainWindow::getIndex(int column, int row)
{
    int index = column * 8 + row;
    return index;
}

/*
 * Updates the index of the ball
 */
void MainWindow::updateIndex(int index)
{
    // Save index of the current ball
    int current = index;

    // Save index of the ball above the current ball
    int previous = current - 8;

    // Deals with non-first row balls
    while(current >= 8)
    {
        Object temp = _objects.at(previous);

        // Removes the current ball from _objects
        _objects.removeAt(current);

        // Inserts a temp ball into _objects
        _objects.insert(current, temp);

        // Updates values
        current = current - 8;
        previous = previous - 8;
    }
}

/*
 * Deals with when the shuffle button is pressed
 */
void MainWindow::on_shuffleButton_pressed()
{
    // Calls game's ShuffleGrid method
    game.ShuffleGrid();

    // Update remaining moves label
    ui->dynRemMovesLabel->setText(QString::number(game.movesRemaining));

    // Destroy all bodies in the world
    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    // Clears _objects
    if (!_objects.isEmpty())
    {
        _objects.clear();
    }

    // Calls fillGrid()
    fillGrid();
}

/*
 * Enforces a delay
 */
void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

/*
 * Deals with when user attempts to play an invalid formula
 */
void MainWindow::dealWithInvalidFormula()
{
    qDebug() << "invalid";
}

/*
 * Handles when the user has completed a level
 */
void MainWindow::dealWithCompletedLevel()
{
    // Display different images on different levels
    if (level == 5)
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/easyCongrats.png);");
    }
    if (level == 10)
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/mediumCongrats.png);");
    }
    if (level == 15)
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/hardCongrats.png);");
    }
    if (level == 20)
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/veryHardCongrats.png);");
    }
    if (level == 25)
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/extremeCongrats.png);");
    }
    else
    {
        ui->congratsWidget->setStyleSheet("background-image: url(:/background/Resources/levelComplete.png);");
    }

    // Calls showCongratsScreen
    showCongratsScreen();

    // Increment level
    level++;

    // Call start
    start();
}

/*
 * Handles when the user loses
 */
void MainWindow::gameOver()
{
    ui->gameOverBox->show();
    ui->tableWidget->setEnabled(false);
    ui->toolBox->setEnabled(false);
}

/*
 * Shows the congratsBox for 2000 msec amongst other UI things
 */
void MainWindow::showCongratsScreen()
{
    ui->congratsBox->show();
    ui->tableWidget->setEnabled(false);
    ui->toolBox->setEnabled(false);
    delay(2000);
    ui->congratsBox->hide();
    ui->tableWidget->setEnabled(true);
    ui->toolBox->setEnabled(true);
}

/*
 * Handles when user completes a successful move
 */
void MainWindow::nextMove(int movesRemaining, int currentNum)
{
    ui->dynTotalLabel->setText(QString::number(currentNum));
    ui->dynRemMovesLabel->setText(QString::number(movesRemaining));
}

/*
 * Removes balls a certain coordinate and adds new balls at a certain coordinate
 */
void MainWindow::removeBubbles(QVector<QPair<int, int>> ballsToRemove, QVector<QPair<int, int>> ballsToAdd)
{
    QVector<int> ballsToRemove2;
        QVector<int> ballsToAdd2;

        //qDebug() << "remvoing bubbles";
        for(int i = 0; i <ballsToRemove.size(); i++)
        {
            QPair<int, int> coord = ballsToRemove.at(i);

            int index = getIndex((int)coord.second, (int)coord.first);

            ballsToRemove2.append(index);
        }

        for(int i = 0; i <ballsToAdd.size(); i++)
        {
            QPair<int, int> coord = ballsToAdd.at(i);

            int index = getIndex((int)coord.second, (int)coord.first);

            ballsToAdd2.append(index);
        }

        qSort(ballsToAdd2);
        qSort(ballsToRemove2);

        for(int i = 0; i < ballsToRemove2.size(); i++)
        {
            removeBallAt(ballsToRemove2.at(i));
        }


        qDebug() << " NEW NODES ";
        int dropheight = 10.0f;
        for(int i = ballsToAdd.size()-1; i >= 0; i--)
        {
            int offsetX = 95.0f;
            int offsetY = 90.0f;
            int dy = offsetY + ballsToAdd.at(i).second * 70;
            int dx = offsetX + ballsToAdd.at(i).first * 70;

            int index = getIndex((int)ballsToAdd.at(i).second, (int)ballsToAdd.at(i).first);
            QPair<int, int> coord = ballsToAdd.at(i);

            MathNode mn = game.grid[ballsToAdd.at(i).first][ballsToAdd.at(i).second];

            qDebug() << "COLUMN: " << coord.first << " ROW: " << coord.second << " VALUE: " << mn.value;

            _objects.append(createBall(b2Vec2(dx, dropheight), radius, mn));
            dropheight = dropheight - 70.0f;
        }

        qDebug() << "\n CURRENT GRID";

        for(int i = 0; i < game.grid.size(); i++)
        {
            for(int j = 0; j < game.grid.at(i).size(); j++)
            {
                MathNode mn = game.grid[i][j];
                qDebug()  << "column: " << i << " row: " << j << " value: " << mn.value;
            }
        }
    qDebug() << "_objects currently has (should be 64):" << _objects.count() << "ISSUE(?)! NEVER GETS REMOVED FROM _OBJECT";
    qDebug() << "world currently has (76):" << World->GetBodyCount();
}

/*
 * Handles when the backend formula changes
 */
void MainWindow::displayFormulaResult(int toDisplay)
{
    ui->dynAnswerLabel->setText(QString::number(toDisplay));
}

/*
 * Resets dynamic labels in the UI to default values
 */
void MainWindow::clearLabels()
{
    ui->dynAnswerLabel->setText("");
    ui->dynFormulaLabel->setText("");
    ui->dynTotalLabel->setText("0");
}

/*
 * Handles when exit button is clicked
 */
void MainWindow::on_exitButton_clicked()
{
    this->close();
}

/*
 * Handles when the retry button is clicked
 */
void MainWindow::on_retryButton_clicked()
{
    ui->gameOverBox->hide();
    ui->tableWidget->setEnabled(true);
    ui->toolBox->setEnabled(true);
    start();
}

/*
 * Handles when the menu button is clicked
 */
void MainWindow::on_menuButton_clicked()
{
    ui->gameOverBox->hide();
    this->hide();
    emit menu_pressed();
}

/*
 * Handles when the modBomb is pressed
 */
void MainWindow::on_modBomb_pressed()
{
    game.BombGrid(4);

    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    qDebug() << "bodycount should be 12:" << World->GetBodyCount();

    if(!_objects.isEmpty())
    {
        _objects.clear();
    }

    fillGrid();
}

/*
 * Handles when the div2Bomb is pressed
 */
void MainWindow::on_div2Bomb_pressed()
{
    game.BombGrid(3);

    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    qDebug() << "bodycount should be 12:" << World->GetBodyCount();

    if(!_objects.isEmpty())
    {
        _objects.clear();
    }

    fillGrid();
}

/*
 * Handles when the mul2Bomb is pressed
 */
void MainWindow::on_mul2Bomb_pressed()
{
    game.BombGrid(1);
    qDebug() << "times two pressed\n";


    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    qDebug() << "bodycount should be 12:" << World->GetBodyCount();

    if(!_objects.isEmpty())
    {
        _objects.clear();
    }

    fillGrid();
}

/*
 * Handles when the mul4Bomb is pressed
 */
void MainWindow::on_mul4Bomb_pressed()
{
    game.BombGrid(2);
    qDebug() << "multiply four pressed\n";

    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    qDebug() << "bodycount should be 12:" << World->GetBodyCount();

    if (!_objects.isEmpty())
    {
        _objects.clear();
    }

    fillGrid();
}

/*
 * Deals with labels when user presses a bomb button
 */
void MainWindow::dealWithBombOp(int bombOp, int counter)
{
    qDebug() << "bomb op";
    switch(bombOp)
    {
    // mul2Bomb
    case 1:
        if(counter!= 0)
        {
            ui->timesTwoCounter->setText(QString::number(counter));
        }
        else
        {
            ui->mul2Bomb->setEnabled(false);
            //disable button
        }
        break;

    // mul4Bomb
    case 2:
        if(counter!= 0)
        {
            ui->timesFourCounter->setText(QString::number(counter));
        }
        else
        {
            ui->mul4Bomb->setEnabled(false);
            //disable button
        }
        break;

    // div2Bomb
    case 3:
        if(counter!= 0)
        {
            ui->divideByTwoCounter->setText(QString::number(counter));
        }
        else
        {
            //disable button
            ui->div2Bomb->setEnabled(false);
        }
        break;

    // modBomb
    case 4:
        if(counter!= 0)
        {
            ui->modCounter->setText(QString::number(counter));
        }
        else
        {
            //disable button
            ui->modBomb->setEnabled(false);
        }
        break;
    }
}

