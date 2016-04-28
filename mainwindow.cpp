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

// Setup
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
    QObject::connect(&game, SIGNAL(CreateBubbleAtSig(int, int)), this, SLOT(dealWithNewBubble(int, int)));
    QObject::connect(&game, SIGNAL(RemoveBubblesAtSig(QVector<QPair<int,int> >, QVector<QPair<int, int>>)), this, SLOT(removeBubbles(QVector<QPair<int,int>>, QVector<QPair<int, int>>)));
    QObject::connect(&game, SIGNAL(sendResult(int)), this, SLOT(displayFormulaResult(int)));

    //Create world
    b2Vec2 gravity(0.0f, 1000.0f); //normal earth gravity, 9.8 m/s/s straight down!
    World = new b2World(gravity);
    gameStarted = false;

    createWalls();

    begin = true;
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete World;
}

//COPY THIS OVER
//Sets value of the cell according to model
void MainWindow::fillGrid()
{
    int index = 0;
    int offsetX = 95.0f;
    int offsetY = 90.0f;
    //qDebug() << game.grid.size();
    for(int i = 0; i < game.grid.size(); i++)
    {
        //QVector<MathNode> column = game.grid.at(i);


        for(int j = 0; j < game.grid[i].size(); j++)
        {
            int dy = offsetY + i * 70;
            int dx = offsetX + j * 70;

            MathNode mn = game.grid[j][i];
            _objects.append(createBall(b2Vec2(dx, dy), radius, index, mn));
            index++;
        }

//        for(int i = 0; i < _objects.size(); i++)
//        {
//            qDebug() << "index: " << _objects.at(i).index << " value : " << _objects.at(i).numberValue;
//        }


    }
    for(int i = 0; i < game.grid.size(); i++)
    {
        for(int j = 0; j < game.grid.at(i).size(); j++)
        {
            MathNode mn = game.grid[i][j];
            qDebug()  << "column: " << i << " row: " << j << " value: " << mn.value;
        }
    }
}

Object MainWindow::createBall(const b2Vec2& pos, float32 radius, int index, MathNode mn)
{
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    o.index = index;

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 2; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;


    o.color = generateColor(mn);
    o.numberValue = mn.value;
    return o;
}

void MainWindow::createBalls()
{
    int offsetX = 95.0f;
    int offsetY = 90.0f;
    int index = 0;
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            int dx = offsetX + j*70;
            int dy = offsetY + i*70;
            _objects.append(createBall(b2Vec2(dx, dy), radius, index));
            index++;
        }
    }
}

void MainWindow::createWalls()
{
    //_objects.append(createWall(60.0f, 305.0f, 560.0f, 1.0f, 0.0)); //test wall

    //Create walls for grid
    walls.append(createWall(60.0f, 615.0f, 560.0f, 1.0f, 0.0)); //ground
    walls.append(createWall(60.0f, 55.0f, 1.0f, 560.0f, 0.0));  //left border
    walls.append(createWall(620.0f, 55.0f, 1.0f, 560.0f, 0.0));  //right border

    //balls do not populate grid, they fall outside and into the abyss of nothingness

    //funnel 1
    walls.append(createWall(-30.0f, 28.0f, 100.0f, 3.0f, -0.25f*b2_pi));
    walls.append(createWall(613.0f, 28.0f, 100.0f, 3.0f, 0.25f*b2_pi));

    //funnel2
    //   _objects.append(createWall(-210.0f, -45.0f, 300.0f, 1.0f, -0.25f*b2_pi));
    //   _objects.append(createWall(590.0f, -45.0f, 300.0f, 1.0f, 0.25f*b2_pi));

    //inner borders
    walls.append(createWall(130.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(200.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(270.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(340.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(410.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(480.0f, 55.0f, 0.5f, 560.0f, 0.0));
    walls.append(createWall(550.0f, 55.0f, 0.5f, 560.0f, 0.0));
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setTransform(_transform);
    foreach(const Object& o, _objects)
    {
        drawEllipse(&p, o);
    }
    foreach(const Object& o, walls)
    {
        drawWall(&p, o);
    }
}



//Add math node to current formula
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    //retrieve value from math node
    //append to formula string

    //qDebug() << "Start expression" << column << row;
    //ui->tableWidget->setItem(row,column,item);

    //validate formula before removing math ball

}

//Same thing here but for drag
//Do not register diagonal nodes?
void MainWindow::on_tableWidget_cellEntered(int row, int column)
{
    //validate formula before removing math ball
    //removeBallAt(column, row);

}

//End string on mouse release
//Send current formula to game model to evaluate
//If valid expression, replace math nodes with new nodes from game model
//else return nodes to grid
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tableWidget->viewport())
    {
        QPair<int, int> coord;
        if (event->type() == QEvent::MouseButtonRelease)
        {

            for(int i = 0; i < coordinates.size(); i++)
            {
                MathNode mn = game.grid.at(coordinates.at(i).first).at(coordinates.at(i).second);
                qDebug() << "x:" <<coordinates.at(i).first << " y" << coordinates.at(i).second << "value: " << mn.value;
            }
            //qDebug() << "here1";
            if(coordinates.size() > 0)
            {
                qDebug() << "here";
                game.OnMove(coordinates);
                coordinates.clear();
            }
            qDebug() << "here2";

            //qDebug() << "current num" << game.currentNum;
        }

        if (event->type() == QEvent::MouseButtonPress)
        {
            ui->dynFormulaLabel->setText("");

        }

        // qDebug() << "End expression";
    }
    return QMainWindow::eventFilter(obj, event);
}

//Draw walls for grid
void MainWindow::drawWall(QPainter *p, const Object& o){
    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 angle = o.body->GetAngle();
    const b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(o.fixture->GetShape());
    float32 hx = shape->GetVertex(1).x;
    float32 hy = shape->GetVertex(2).y;

    QRectF r(x-hx, y-hy, 2*hx, 2*hy);

    p->save();
    p->translate(r.center());
    p->rotate(angle*180/b2_pi);
    p->translate(-r.center());
    p->drawRect(r);
    p->fillRect(r, Qt::white);

    p->restore();
}

//Create walls for grid
Object MainWindow::createWall(float32 x, float32 y, float32 w, float32 h, float32 angle) {
    Object o;
    // body
    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position = b2Vec2(x+w/2.0f, y+h/2.0f);
    bd.angle = angle * b2_pi;
    o.body = World->CreateBody(&bd);
    // shape
    b2PolygonShape shape;
    shape.SetAsBox(w/2.0f, h/2.0f);
    // fixture
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.3f;
    o.fixture = o.body->CreateFixture(&fd);
    o.type = WallObject;
    return o;
}

//Create ball model
Object MainWindow::createBall(const b2Vec2& pos, float32 radius) {
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 2; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;

    return o;
}

Object MainWindow::createBall(const b2Vec2& pos, float32 radius, int index)
{
    Object o;    //_objects.append(createBall(b2Vec2(dx, dy), 34.4f));

    // body
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = pos;
    o.body = World->CreateBody(&bd);
    o.index = index;

    // shape
    b2CircleShape shape;
    shape.m_radius = radius; //ADJUST BALL RADIUS HERE
    // fixture
    // add mass
    for(int i = 0; i < 5; i++)
    {
        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 1.0f;
        fd.friction = 0.6f;
        fd.restitution = 0.0f;
        o.fixture = o.body->CreateFixture(&fd);

    }

    o.type = BallObject;
    o.column = pos.x;
    o.row = pos.y;

    return o;
}


//Draw ball model
void MainWindow::drawEllipse(QPainter *p, const Object& o)
{
    QFont font;
    font.setPixelSize(20);

    // Fills nodes
    QPen pen(Qt::black, 2);
    p->setPen(pen);
    QBrush brush(o.color);

    float32 x = o.body->GetPosition().x;
    float32 y = o.body->GetPosition().y;
    float32 r = o.fixture->GetShape()->m_radius;

    p->setBrush(brush);

    p->drawEllipse(QPointF(x, y), r, r);

    brush.setColor(Qt::white);

    p->setBrush(brush);

    p->setFont(font);
    p->drawText(QPointF(x - 5, y + 5), o.numberValue);
}

//Start simulator
void MainWindow::start() {
    //if(!_timerId) {
    _timerId = startTimer(1000/60); // 60fps
    //}
    gameStarted = true;


    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    while(!_objects.isEmpty())
    {
        _objects.removeFirst();
    }

    //Create world
    b2Vec2 gravity(0.0f, 1000.0f); //normal earth gravity, 9.8 m/s/s straight down!
    World = new b2World(gravity);

    createWalls();


    game.LevelStart(level, difficulty);

    //:LevelStart(int lNum, int diffNum)

    ui->dynTargetLabel->setText(QString::number(game.targetNum));
    ui->dynDifficultyLabel->setText(game.difficultyString);
    ui->dynLevelLabel_2->setText(QString::number(game.levelNum));

    fillGrid();


    begin = true;

}

void MainWindow::timerEvent(QTimerEvent *event) {
    if(event->timerId() == _timerId) {
        World->Step(1.0f/60.0f, 8, 3);
        update();
    }
}

// Returns a QColor based on the input MathNode's ending value
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

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    QPair<int, int> rowAndCol;
    if ((currentRow == 0 && currentColumn == 0) && ((previousRow == -1 &&  previousColumn == -1)))
    {
        ;
    }
    else
    {
        rowAndCol.first = currentColumn;
        rowAndCol.second = currentRow;
        qDebug() << "add expression" << currentColumn << currentRow;
        coordinates.append(rowAndCol);
        MathNode mn = game.grid[currentColumn][currentRow];
        QString text = ui->dynFormulaLabel->text();
        text += mn.value;
        ui->dynFormulaLabel->setText(text);
    }
}

void MainWindow::on_bombButton_pressed()
{

}

void MainWindow::removeBallAt(float32 column, float32 row)
{
    int index = getIndex((int)column, (int)row);

    b2Body *body = _objects.at(index).body;
    //qDebug() << "removing ball at address " << body;

    updateIndex(index);

    World->DestroyBody(body);

    spawnBallAt(row, index);

}

void MainWindow::removeBallAt(float32 column, float32 row, int delayVal)
{
    int index = getIndex((int)column, (int)row);
    //qDebug() << "removing ball at column: " <<column << " row: " << row << " index: " << index <<"\n";

    b2Body *body = _objects.at(index).body;
    //qDebug() << "removing ball at address " << body;

    updateIndex(index);

    World->DestroyBody(body);

    //qDebug() << "Here";

    spawnBallAt(row, index);

    delay(delayVal);
}

void MainWindow::removeBallAt(int index, int delayVal)
{

    b2Body *body = _objects.at(index).body;
    //qDebug() << "removing ball at address " << body;

    updateIndex(index);

    World->DestroyBody(body);

    //qDebug() << "Here";

    int row = index % 8;

    //spawnBallAt(row, index);

    delay(delayVal);
}

void MainWindow::spawnBallAt(float32 column, int index)
{
    //qDebug() << "COLUMN " << column;
    //incorrect index
   // qDebug() << "removing";

    _objects.removeAt(column);



    _objects.insert(column, createBall(b2Vec2((column * 70) + 95, 10.0f ), radius, column));
}

void MainWindow::spawnBallAt(float32 column, float32 row)
{
    MathNode mn = game.grid[row][column];
    qDebug() << "mn value " << mn.value;
    _objects.removeAt(column);
    _objects.insert(column, createBall(b2Vec2((column * 70) + 95, 10.0f ), radius, column,mn));
}


int MainWindow::getIndex(int column, int row)
{
    int index = column * 8 + row;
    return index;
}

//for trickling down
void MainWindow::updateIndex(int index)
{
    //current ball
    int current = index;
    //ball right above it
    int previous = current - 8;

    while(current >= 8)
    {

        // qDebug() << "Object Previous to be Moved " << _objects.at(previous).index;

        Object temp = _objects.at(previous);

        _objects.removeAt(current);

        _objects.insert(current, temp);
        //qDebug() << "Object Now At " << current << " is: " <<  _objects.at(current).index;
        //qDebug() << "Should be Same as First Object Now At " << previous << " is: " <<  _objects.at(previous).index;

        current = current - 8;
        previous = previous - 8;
    }
    //qDebug() << "done updating";
}




void MainWindow::on_shuffleButton_pressed()
{
    foreach(Object o, _objects)
    {
        World->DestroyBody(o.body);
    }

    while(!_objects.isEmpty())
    {
        _objects.removeFirst();
    }

    int offsetX = 95.0f;
    int offsetY = 90.0f;
    int dropheight = 10.0f;
    for (int i=7; i>-1; i--)
    {
        for (int j=7; j>-1; j--)
        {
            int dx = offsetX + j*70;
            int dy = offsetY + i*70;
            _objects.prepend(createBall(b2Vec2(dx, dropheight), radius));

            delay(50);
        }
        dropheight = dropheight - 70.0f;
    }
}

void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::dealWithInvalidFormula()
{
    qDebug() << "invalid";
}

void MainWindow::dealWithCompletedLevel()
{

}

void MainWindow::gameOver()
{

}

void MainWindow::nextMove(int movesRemaining, int currentNum)
{
    ui->dynTotalLabel->setText(QString::number(currentNum));
}

void MainWindow::dealWithNewBubble(int column, int row)
{
    int index = getIndex(column, row);


    MathNode mn = game.grid[column][row];

    spawnBallAt(row, index, mn);

//    for(int j = 0; j < game.grid[i].size(); j++)
//    {
//        int dy = offsetY + i * 70;
//        int dx = offsetX + j * 70;

//        MathNode mn = game.grid[i][j];
//        _objects.append(createBall(b2Vec2(dx, dy), radius, index, mn));
//        index++;
//    }



}

void MainWindow::spawnBallAt(float32 column, int index, MathNode mn)
{
    _objects.removeAt(column);
    //qDebug() <<"removebub";

    _objects.insert(column, createBall(b2Vec2((column * 70) + 95, 10.0f ), radius, column, mn));
}

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
        removeBallAt(ballsToRemove2.at(i), 100);
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

        _objects.append(createBall(b2Vec2(dx, dropheight), radius, index, mn));
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
}

void MainWindow::displayFormulaResult(int toDisplay)
{
    ui->dynAnswerLabel->setText(QString::number(toDisplay));
}
